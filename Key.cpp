Key.cpp
#include "Key.h"

// Define the static constexpr array
constexpr std::array<std::array<char, 4>, 4> MatrixKeypad::KEYPAD_LAYOUT;

MatrixKeypad::MatrixKeypad(const std::string &chipName,
													 const std::array<int, 4> &colPins,
													 const std::array<int, 4> &rowPins)
		: m_chipName(chipName), m_colPins(colPins), m_rowPins(rowPins)
{
	m_lastKeyData = {-1, -1, '\0', std::chrono::steady_clock::now(), false};
}

MatrixKeypad::~MatrixKeypad()
{
	stopScanning();
}

bool MatrixKeypad::initialize()
{
	try
	{
		m_chip = std::make_unique<gpiod::chip>(m_chipName);
		// Initialize column pins as outputs
		for (int i = 0; i < 4; ++i)
		{
auto line = std::make_unique<gpiod::line>(m_chip->get_line(m_colPins[i]));
			line->request({"keypad_col", gpiod::line_request::DIRECTION_OUTPUT, 0});
			line->set_value(0);
			m_colLines.push_back(std::move(line));
		}
// Initialize row pins as inputs
		for (int i = 0; i < 4; ++i)
		{
			auto line = std::make_unique<gpiod::line>(m_chip->get_line(m_rowPins[i]));
			line->request({"keypad_row", gpiod::line_request::DIRECTION_INPUT});
			m_rowLines.push_back(std::move(line));
		}
		return true;
	}
catch (const std::exception &e)
	{
		if (m_errorCallback)
		{
			m_errorCallback("Failed to initialize keypad: " + std::string(e.what()));
		}
		return false;
	}
}

void MatrixKeypad::startScanning(int scanIntervalMs)
{
	if (m_scanning.load())
	{
		return;
	}
	if (m_colLines.empty() || m_rowLines.empty())
	{
		if (!initialize())
		{
			return;
		}
	}
	m_scanning.store(true);
	m_scanThread = std::make_unique<std::thread>(&MatrixKeypad::scanningThread, this, scanIntervalMs);
}

void MatrixKeypad::stopScanning()
{
	m_scanning.store(false);
	if (m_scanThread && m_scanThread->joinable())
	{
		m_scanThread->join();
	}
	m_scanThread.reset();
}

MatrixKeypad::KeyData MatrixKeypad::getLastKeyPress() const
{
	std::lock_guard<std::mutex> lock(m_dataMutex);
	return m_lastKeyData;
}

void MatrixKeypad::registerKeyPressCallback(KeyPressCallback callback)
{
	m_keyPressCallback = callback;
}

void MatrixKeypad::registerErrorCallback(ErrorCallback callback)
{
	m_errorCallback = callback;
}

bool MatrixKeypad::isScanning() const
{
	return m_scanning.load();
}

char MatrixKeypad::getKeyChar(int row, int col) const
{
	if (row >= 0 && row < 4 && col >= 0 && col < 4)
	{
		return KEYPAD_LAYOUT[row][col];
	}
	return '\0';
}

void MatrixKeypad::scanningThread(int scanIntervalMs)
{
	while (m_scanning.load())
	{
		try
		{
			KeyData keyData = scanMatrix();
			if (keyData.isPressed && debounceKey(keyData.row, keyData.col))
			{
				{
					std::lock_guard<std::mutex> lock(m_dataMutex);
					m_lastKeyData = keyData;
				}

				if (m_keyPressCallback)
				{
					m_keyPressCallback(keyData.row, keyData.col, keyData.keyChar);
				}
			}
		}
		catch (const std::exception &e)
		{
			if (m_errorCallback)
			{
				m_errorCallback("Keypad scanning error: " + std::string(e.what()));
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(scanIntervalMs));
	}
}

MatrixKeypad::KeyData MatrixKeypad::scanMatrix()
{
	KeyData keyData = {-1, -1, '\0', std::chrono::steady_clock::now(), false};
	for (int col = 0; col < 4; ++col)
	{
		// Set current column high
		m_colLines[col]->set_value(1);
		delay_ms(1); // For signal propagation
		// Check all rows for this column
		for (int row = 0; row < 4; ++row)
		{
			if (m_rowLines[row]->get_value() == 1)
			{
				keyData.row = row;
				keyData.col = col;
				keyData.keyChar = getKeyChar(row, col);
				keyData.isPressed = true;
				keyData.timestamp = std::chrono::steady_clock::now();
				// Wait for key release
				while (m_rowLines[row]->get_value() == 1)
				{
					delay_ms(10);
				}
				// Set column to low before returning
				m_colLines[col]->set_value(0);
				return keyData;
			}
		}
		// Set column to low
		m_colLines[col]->set_value(0);
