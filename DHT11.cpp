#include "DHT11.h"
#include "Delay.h"
#include <stdexcept>

DHT11Sensor::DHT11Sensor(const std::string &chipName, int pin)
		: m_chipName(chipName), m_pin(pin)
{
	m_latestData = {0, 0, false, std::chrono::steady_clock::now()};
}

DHT11Sensor::~DHT11Sensor()
{
	stopMonitoring();
}

bool DHT11Sensor::initialize()
{
	try
	{
		m_chip = std::make_unique<gpiod::chip>(m_chipName);
		m_dataLine = std::make_unique<gpiod::line>(m_chip->get_line(m_pin));
		return true;
	}
	catch (const std::exception &e)
	{
		if (m_errorCallback)
		{
			m_errorCallback("Failed to initialize DHT11: " + std::string(e.what()));
		}
		return false;
	}
}

void DHT11Sensor::startMonitoring(int intervalMs)
{
	if (m_monitoring.load())
	{
		return; // Already monitoring
	}
	if (!m_chip || !m_dataLine)
	{
		if (!initialize())
		{
			return;
		}
	}
	m_monitoring.store(true);
	m_monitorThread = std::make_unique<std::thread>(&DHT11Sensor::monitoringThread, this, intervalMs);
}

void DHT11Sensor::stopMonitoring()
{
	m_monitoring.store(false);
	if (m_monitorThread && m_monitorThread->joinable())
	{
		m_monitorThread->join();
	}
	m_monitorThread.reset();
}

DHT11Sensor::SensorData DHT11Sensor::getLatestReading() const
{
	std::lock_guard<std::mutex> lock(m_dataMutex);
	return m_latestData;
}

void DHT11Sensor::registerDataCallback(SensorDataCallback callback)
{
	m_dataCallback = callback;
}

void DHT11Sensor::registerErrorCallback(ErrorCallback callback)
{
	m_errorCallback = callback;
}

bool DHT11Sensor::isMonitoring() const
{
	return m_monitoring.load();
}

void DHT11Sensor::monitoringThread(int intervalMs)
{
	while (m_monitoring.load())
	{
		try
		{
			auto rawData = readRawData();
			bool isValid = validateChecksum(rawData);

			if (isValid)
			{
				int humidity = rawData[0];
				int temperature = rawData[2];

				{
					std::lock_guard<std::mutex> lock(m_dataMutex);
					m_latestData = {temperature, humidity, true, std::chrono::steady_clock::now()};
				}

				if (m_dataCallback)
				{
					m_dataCallback(temperature, humidity, true);
				}
			}
			else
			{
				{
					std::lock_guard<std::mutex> lock(m_dataMutex);
					m_latestData.isValid = false;
					m_latestData.timestamp = std::chrono::steady_clock::now();
				}

				if (m_dataCallback)
				{
					m_dataCallback(0, 0, false);
				}

				if (m_errorCallback)
				{
					m_errorCallback("DHT11 checksum validation failed");
				}
			}
		}
		catch (const std::exception &e)
		{
			if (m_errorCallback)
			{
				m_errorCallback("DHT11 reading error: " + std::string(e.what()));
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
	}
}

std::array<uint8_t, 5> DHT11Sensor::readRawData()
{
	std::array<uint8_t, 5> data = {0};
	if (!sendStartSignal())
	{
		throw std::runtime_error("Failed to send start signal to DHT11");
	}

	if (!waitForResponse())
	{
		throw std::runtime_error("DHT11 did not respond");
	}
	// Read 40 bits of data
	for (int byteIdx = 0; byteIdx < 5; byteIdx++)
	{
		uint8_t byte = 0;
		for (int bitIdx = 7; bitIdx >= 0; bitIdx--)
		{
			int bit = readBit();
			if (bit < 0)
			{
				throw std::runtime_error("Failed to read bit from DHT11");
			}
			byte |= (bit << bitIdx);
		}
		data[byteIdx] = byte;
	}
	return data;
}

bool DHT11Sensor::validateChecksum(const std::array<uint8_t, 5> &data) const
{
	uint16_t sum = data[0] + data[1] + data[2] + data[3];
	return (sum & 0xFF) == data[4];
}

bool DHT11Sensor::sendStartSignal()
{
	try
	{
		// Set pin as output and pull low
		m_dataLine->request({"DHT11", gpiod::line_request::DIRECTION_OUTPUT, 0});
		delay_ms(18);
		// Pull high
		m_dataLine->set_value(1);
		delay_us(30);
		m_dataLine->release();
		m_dataLine->request({"DHT11", gpiod::line_request::DIRECTION_INPUT});
		return true;
	}
	catch (const std::exception &)
	{
		return false;
	}
}

bool DHT11Sensor::waitForResponse()
{
	// Wait for DHT11 to pull line low
	auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);
	while (m_dataLine->get_value() == 1)
	{
		if (std::chrono::steady_clock::now() > timeout)
		{
			return false;
		}
	}
	// Wait for DHT11 to pull line high
	timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);
	while (m_dataLine->get_value() == 0)
	{
		if (std::chrono::steady_clock::now() > timeout)
		{
			return false;
		}
	}
	// Wait for DHT11 to pull line low
	timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);
	while (m_dataLine->get_value() == 1)
	{
		if (std::chrono::steady_clock::now() > timeout)
		{
			return false;
		}
	}

	return true;
}

int DHT11Sensor::readBit()
{
	// Wait for line to go high
	auto timeout = std::chrono::steady_clock::now() + std::chrono::microseconds(100);
	while (m_dataLine->get_value() == 0)
	{
		if (std::chrono::steady_clock::now() > timeout)
		{
			return -1;
		}
	}
	// Measure how long line stays high
	auto start = std::chrono::steady_clock::now();
	timeout = start + std::chrono::microseconds(100);
	while (m_dataLine->get_value() == 1)
	{
		if (std::chrono::steady_clock::now() > timeout)
		{
			break;
		}
	}
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	return (duration.count() > 40) ? 1 : 0;
}
