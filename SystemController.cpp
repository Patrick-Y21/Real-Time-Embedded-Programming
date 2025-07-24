#include "SystemController.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <ctime>

SystemController::SystemController(const SystemConfig &config)
		: m_config(config)
{
}

SystemController::~SystemController()
{
	stop();
}

bool SystemController::initialize()
{
	std::cout << "[SystemController] Initializing system..." << std::endl;
	if (!initializeGPIO())
	{
		std::cerr << "[SystemController] Failed to initialize GPIO" << std::endl;
		return false;
	}
	if (!initializeSensors())
	{
		std::cerr << "[SystemController] Failed to initialize sensors" << std::endl;
		return false;
	}
	if (!initializeKeypad())
	{
		std::cerr << "[SystemController] Failed to initialize keypad" << std::endl;
		return false;
	}
	if (!initializeBluetooth())
	{
		std::cout << "[SystemController] Warning: Bluetooth initialization failed, continuing without it" << std::endl;
	}
	std::cout << "[SystemController] System initialized successfully" << std::endl;
	return true;
}

void SystemController::start()
{
	if (m_running.load())
	{
		return;
	}
	std::cout << "[SystemController] Starting system..." << std::endl;
	m_running.store(true);
	// Start sensor monitoring
	if (m_dht11Sensor)
	{
		m_dht11Sensor->startMonitoring(m_config.sensorReadInterval);
	}
	// Start keypad scanning
	if (m_keypad)
	{
		m_keypad->startScanning(m_config.keypadScanInterval);
	}
	// Start alarm monitoring
	m_alarmThread = std::make_unique<std::thread>(&SystemController::alarmMonitoringThread, this);
	// Start Bluetooth communication if available
	if (m_bluetoothFd >= 0)
	{
		m_bluetoothThread = std::make_unique<std::thread>(&SystemController::bluetoothReceiverThread, this);
	}

	std::cout << "[SystemController] System started successfully" << std::endl;
}

void SystemController::stop()
{
	if (!m_running.load())
	{
		return;
	}
	std::cout << "[SystemController] Stopping system..." << std::endl;
	m_running.store(false);
	// Stop components
	if (m_dht11Sensor)
	{
		m_dht11Sensor->stopMonitoring();
	}
	if (m_keypad)
	{
		m_keypad->stopScanning();
	}
	// Stop threads
	if (m_alarmThread && m_alarmThread->joinable())
	{
		m_alarmThread->join();
	}
	m_alarmThread.reset();
	if (m_bluetoothThread && m_bluetoothThread->joinable())
	{
		m_bluetoothThread->join();
	}
	m_bluetoothThread.reset();
	// Close Bluetooth connection
	if (m_bluetoothFd >= 0)
	{
		close(m_bluetoothFd);
		m_bluetoothFd = -1;
	}
	// Turn off buzzer
	setBuzzer(false);

	std::cout << "[SystemController] System stopped successfully" << std::endl;
}

bool SystemController::isRunning() const
{
	return m_running.load();
}

SystemController::SystemState SystemController::getSystemState() const
{
	return m_systemState.load();
}

SystemController::CurtainState SystemController::getCurtainState() const
{
	return m_curtainState.load();
}

DHT11Sensor::SensorData SystemController::getLatestSensorData() const
{
	if (m_dht11Sensor)
	{
		return m_dht11Sensor->getLatestReading();
	}
	return {0, 0, false, std::chrono::steady_clock::now()};
}

void SystemController::setAlarmTime(int hours, int minutes)
{
	std::lock_guard<std::mutex> lock(m_alarmMutex);
	m_alarmHour = hours;
	m_alarmMinute = minutes;
	m_alarmEnabled = true;
	std::cout << "[SystemController] Alarm set for " << hours << ":" << minutes << std::endl;
}

void SystemController::clearAlarm()
{
	std::lock_guard<std::mutex> lock(m_alarmMutex);
	m_alarmEnabled = false;
	setBuzzer(false);
	std::cout << "[SystemController] Alarm cleared" << std::endl;
}

bool SystemController::initializeGPIO()
{
	try
	{
		m_gpioChip = std::make_unique<gpiod::chip>(m_config.gpioChipName);
		// Initialize buzzer
		m_buzzerLine = std::make_unique<gpiod::line>(m_gpioChip->get_line(m_config.buzzerPin));
		m_buzzerLine->request({"buzzer", gpiod::line_request::DIRECTION_OUTPUT, 0});

		return true;
	}
	catch (const std::exception &e)
	{
		std::cerr << "[SystemController] GPIO initialization error: " << e.what() << std::endl;
		return false;
	}
}

bool SystemController::initializeSensors()
{
	try
	{
		m_dht11Sensor = std::make_unique<DHT11Sensor>(m_config.gpioChipName, m_config.dht11Pin);
		// Register sensor callback
		m_dht11Sensor->registerDataCallback(
				[this](int temp, int hum, bool valid)
				{
					handleSensorData(temp, hum, valid);
				});
		// Register error callback
		m_dht11Sensor->registerErrorCallback(
				[this](const std::string &error)
				{
					handleError("DHT11 Error: " + error);
				});

		return m_dht11Sensor->initialize();
	}
	catch (const std::exception &e)
	{
		std::cerr << "[SystemController] Sensor initialization error: " << e.what() << std::endl;
		return false;
	}
}

bool SystemController::initializeKeypad()
{
	try
	{
		m_keypad = std::make_unique<MatrixKeypad>(m_config.gpioChipName,
																							m_config.keypadCols,
																							m_config.keypadRows);
		// Register keypad callback
		m_keypad->registerKeyPressCallback(
				[this](int row, int col, char key)
				{
					handleKeypadInput(row, col, key);
				});
		// Register error callback
		m_keypad->registerErrorCallback(
				[this](const std::string &error)
				{
					handleError("Keypad Error: " + error);
				});
		return m_keypad->initialize();
	}
	catch (const std::exception &e)
	{
		std::cerr << "[SystemController] Keypad initialization error: " << e.what() << std::endl;
		return false;
	}
}

bool SystemController::initializeBluetooth()
{
	try
	{
		m_bluetoothFd = open("/dev/rfcomm0", O_RDWR | O_NOCTTY | O_NONBLOCK);
		return (m_bluetoothFd >= 0);
	}
	catch (...)
	{
		return false;
	}
}

void SystemController::handleSensorData(int temperature, int humidity, bool isValid)
{
	if (!isValid)
	{
		std::cout << "[SystemController] Invalid sensor data received" << std::endl;
		return;
	}
	std::cout << "[SystemController] Sensor data: " << temperature << "°C, " << humidity << "%" << std::endl;
	// Temperature-based buzzer control
	if (temperature > m_config.tempThreshold)
	{
		setBuzzer(true);
		std::cout << "[SystemController] High temperature alert!" << std::endl;
	}
	else
	{
		setBuzzer(false);
	}
	// Auto mode evaluation
	if (m_systemState.load() == SystemState::AUTO_MODE)
	{
		evaluateAutoMode(temperature, humidity);
	}
}

void SystemController::handleKeypadInput(int row, int col, char key)
{
	std::cout << "[SystemController] Key pressed: " << key << " (row=" << row << ", col=" << col << ")" << std::endl;

	switch (key)
	{
	case '1': // Manual mode
		m_systemState.store(SystemState::MANUAL_MODE);
		std::cout << "[SystemController] Switched to manual mode" << std::endl;
		break;

	case '2': // Manual close
		if (m_systemState.load() == SystemState::MANUAL_MODE)
		{
			setCurtainState(CurtainState::CLOSED);
			std::cout << "[SystemController] Manual close curtain" << std::endl;
		}
		break;

	case '3': // Manual open
		if (m_systemState.load() == SystemState::MANUAL_MODE)
		{
			setCurtainState(CurtainState::OPEN);
			std::cout << "[SystemController] Manual open curtain" << std::endl;
		}
		break;

	case '4': // Auto mode
	{
		m_systemState.store(SystemState::AUTO_MODE);
		std::cout << "[SystemController] Switched to auto mode" << std::endl;
		// Immediately evaluate conditions
		auto sensorData = getLatestSensorData();
		if (sensorData.isValid)
		{
			evaluateAutoMode(sensorData.temperature, sensorData.humidity);
		}
	}
	break;

	case '5': // Alarm hour +
	{
		std::lock_guard<std::mutex> lock(m_alarmMutex);
		m_alarmHour = (m_alarmHour + 1) % 24;
		std::cout << "[SystemController] Alarm time: " << m_alarmHour << ":" << m_alarmMinute << std::endl;
	}
	break;

	case '6': // Alarm minute + 30
	{
		std::lock_guard<std::mutex> lock(m_alarmMutex);
		m_alarmMinute = (m_alarmMinute + 30) % 60;
		std::cout << "[SystemController] Alarm time: " << m_alarmHour << ":" << m_alarmMinute << std::endl;
	}
	break;

	case '7': // Set alarm
	{
		std::time_t now = std::time(nullptr);
		std::tm *localTime = std::localtime(&now);
		int currentHour = localTime->tm_hour;
		int currentMinute = localTime->tm_min;

		std::lock_guard<std::mutex> lock(m_alarmMutex);
		int finalHour = (currentHour + m_alarmHour + (currentMinute + m_alarmMinute) / 60) % 24;
		int finalMinute = (currentMinute + m_alarmMinute) % 60;
		setAlarmTime(finalHour, finalMinute);
	}
	break;

	case '8': // Clear alarm
		clearAlarm();
		break;

	default:
		std::cout << "[SystemController] Unhandled key: " << key << std::endl;
		break;
	}
}

void SystemController::handleBluetoothCommand(char command)
{
	std::cout << "[SystemController] Bluetooth command: " << (int)command << std::endl;

	switch (command)
	{
	case 0:
		setCurtainState(CurtainState::CLOSED);
		break;
	case 1:
		setCurtainState(CurtainState::OPEN);
		break;
	default:
		std::cout << "[SystemController] Unknown Bluetooth command: " << (int)command << std::endl;
		break;
	}
}

void SystemController::setCurtainState(CurtainState newState)
{
	CurtainState currentState = m_curtainState.load();
	if (currentState != newState)
	{
		m_curtainState.store(newState);
		std::cout << "[SystemController] Curtain state changed to: "
							<< (newState == CurtainState::OPEN ? "OPEN" : "CLOSED") << std::endl;
	}
}

void SystemController::setBuzzer(bool enable)
{
	try
	{
		if (m_buzzerLine)
		{
			m_buzzerLine->set_value(enable ? 1 : 0);
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "[SystemController] Buzzer control error: " << e.what() << std::endl;
	}
}

void SystemController::evaluateAutoMode(int temperature, int humidity)
{
	// Auto mode logic: Open curtain if temperature > threshold and humidity > threshold
	if (temperature > 20 && humidity > m_config.humidityThreshold)
	{
		setCurtainState(CurtainState::OPEN);
		std::cout << "[SystemController] Auto mode: Opening curtain (T=" << temperature
							<< "°C, H=" << humidity << "%)" << std::endl;
	}
	else
	{
		setCurtainState(CurtainState::CLOSED);
		std::cout << "[SystemController] Auto mode: Closing curtain (T=" << temperature
							<< "°C, H=" << humidity << "%)" << std::endl;
	}
}

void SystemController::bluetoothReceiverThread()
{
	char buffer[256];
	while (m_running.load())
	{
		if (m_bluetoothFd >= 0)
		{
			int len = read(m_bluetoothFd, buffer, sizeof(buffer) - 1);
			if (len > 0)
			{
				buffer[len] = '\0';
				handleBluetoothCommand(buffer[0]);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void SystemController::alarmMonitoringThread()
{
	while (m_running.load())
	{
		{
			std::lock_guard<std::mutex> lock(m_alarmMutex);
			if (m_alarmEnabled)
			{
				std::time_t now = std::time(nullptr);
				std::tm *localTime = std::localtime(&now);

				if (localTime->tm_hour == m_alarmHour && localTime->tm_min == m_alarmMinute)
				{
					std::cout << "[SystemController] Alarm triggered!" << std::endl;
					setBuzzer(true);
					m_alarmEnabled = false; // Disable alarm after triggering
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void SystemController::handleError(const std::string &error)
{
	std::cerr << "[SystemController] Error: " << error << std::endl;
}

