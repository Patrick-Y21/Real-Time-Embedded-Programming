#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "DHT11.h"
#include "Key.h"
#include <memory>
#include <atomic>
#include <functional>
#include <chrono>
#include <mutex>
#include <thread>
#include <gpiod.hpp>

/**
 * @brief Main System Controller Class
 */
class SystemController
{
public:
	// System states
	enum class SystemState
	{
		MANUAL_MODE,
		AUTO_MODE,
		ALARM_MODE
	};

	enum class CurtainState
	{
		CLOSED = 0,
		OPEN = 1
	};

	// System configuration
	struct SystemConfig
	{
		std::string gpioChipName;
		int dht11Pin;
		int buzzerPin;
		std::array<int, 4> keypadCols;
		std::array<int, 4> keypadRows;
		int sensorReadInterval; // ms
		int keypadScanInterval; // ms
		int tempThreshold;			// °C
		int humidityThreshold;	// %

		// Default constructor
		SystemConfig()
				: gpioChipName("gpiochip0"), dht11Pin(17), buzzerPin(18), keypadCols({{26, 19, 13, 6}}), keypadRows({{21, 20, 16, 12}}), sensorReadInterval(2000), keypadScanInterval(50), tempThreshold(27), humidityThreshold(40) {}
	};

	/**
	 * @brief Constructor
	 * @param config System configuration
	 */
	explicit SystemController(const SystemConfig &config = SystemConfig());

	/**
	 * @brief Destructor
	 */
	~SystemController();

	/**
	 * @brief Initialize the system
	 * @return true if initialization successful
	 */
	bool initialize();

	/**
	 * @brief Start the system operation
	 */
	void start();

	/**
	 * @brief Stop the system operation
	 */
	void stop();

	/**
	 * @brief Check if system is running
	 * @return true if system is running
	 */
	bool isRunning() const;

	/**
	 * @brief Get current system state
	 * @return Current SystemState
	 */
	SystemState getSystemState() const;

	/**
	 * @brief Get current curtain state
	 * @return Current CurtainState
	 */
	CurtainState getCurtainState() const;

	/**
	 * @brief Get latest sensor data
	 * @return DHT11 sensor data
	 */
	DHT11Sensor::SensorData getLatestSensorData() const;

	/**
	 * @brief Set alarm time
	 * @param hours Hour (0-23)
	 * @param minutes Minute (0-59)
	 */
	void setAlarmTime(int hours, int minutes);

	/**
	 * @brief Clear/disable alarm
	 */
	void clearAlarm();

private:
	SystemConfig m_config;

	// Hardware components
	std::unique_ptr<DHT11Sensor> m_dht11Sensor;
	std::unique_ptr<MatrixKeypad> m_keypad;
	std::unique_ptr<gpiod::chip> m_gpioChip;
	std::unique_ptr<gpiod::line> m_buzzerLine;

	// System state
	std::atomic<bool> m_running{false};
	std::atomic<SystemState> m_systemState{SystemState::MANUAL_MODE};
	std::atomic<CurtainState> m_curtainState{CurtainState::CLOSED};

	// Alarm system
	mutable std::mutex m_alarmMutex;
	bool m_alarmEnabled = false;
	int m_alarmHour = 0;
	int m_alarmMinute = 0;
	std::unique_ptr<std::thread> m_alarmThread;

	// Bluetooth communication
	int m_bluetoothFd = -1;
	std::unique_ptr<std::thread> m_bluetoothThread;

	/**
	 * @brief Initialize GPIO components
	 * @return true if successful
	 */
	bool initializeGPIO();

	/**
	 * @brief Initialize sensors
	 * @return true if successful
	 */
	bool initializeSensors();

	/**
	 * @brief Initialize keypad
	 * @return true if successful
	 */
	bool initializeKeypad();

	/**
	 * @brief Initialize Bluetooth communication
	 * @return true if successful
	 */
	bool initializeBluetooth();

	/**
	 * @brief Handle sensor data updates
	 * @param temperature Temperature reading
	 * @param humidity Humidity reading
	 * @param isValid Data validity
	 */
	void handleSensorData(int temperature, int humidity, bool isValid);

	/**
	 * @brief Handle keypad input
	 * @param row Key row
	 * @param col Key column
	 * @param key Key character
	 */
	void handleKeypadInput(int row, int col, char key);

	/**
	 * @brief Handle Bluetooth commands
	 * @param command Received command
	 */
	void handleBluetoothCommand(char command);

	/**
	 * @brief Control curtain state
	 * @param newState Desired curtain state
	 */
	void setCurtainState(CurtainState newState);

	/**
	 * @brief Control buzzer
	 * @param enable Enable/disable buzzer
	 */
	void setBuzzer(bool enable);

	/**
	 * @brief Evaluate auto mode conditions
	 * @param temperature Current temperature
	 * @param humidity Current humidity
	 */
	void evaluateAutoMode(int temperature, int humidity);

	/**
	 * @brief Bluetooth receiver thread
	 */
	void bluetoothReceiverThread();

	/**
	 * @brief Alarm monitoring thread
	 */
	void alarmMonitoringThread();

	/**
	 * @brief Handle system errors
	 * @param error Error message
	 */
	void handleError(const std::string &error);
};

#endif
