#ifndef DHT11_H
#define DHT11_H

#include <gpiod.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <functional>
#include <memory>
#include <array>
#include <mutex>
#include <atomic>
#include <string>

/**
 * @brief DHT11 Temperature and Humidity Sensor Class
 * Provides real-time sensor data with callback-based event handling
 */
class DHT11Sensor
{
public:
	// Callback types for sensor events
	using SensorDataCallback = std::function<void(int temperature, int humidity, bool valid)>;
	using ErrorCallback = std::function<void(const std::string &error)>;

	// Sensor data structure
	struct SensorData
	{
		int temperature;
		int humidity;
		bool isValid;
		std::chrono::steady_clock::time_point timestamp;
	};

	/**
	 * @brief Constructor
	 * @param chipName GPIO chip name
	 * @param pin GPIO pin number for DHT11 data line
	 */
	DHT11Sensor(const std::string &chipName, int pin);

	/**
	 * @brief Destructor
	 */
	~DHT11Sensor();

	/**
	 * @brief Initialize the sensor
	 * @return true if initialization successful
	 */
	bool initialize();

	/**
	 * @brief Start continuous sensor monitoring
	 * @param intervalMs Measurement interval in milliseconds
	 */
	void startMonitoring(int intervalMs = 2000);

	/**
	 * @brief Stop sensor monitoring
	 */
	void stopMonitoring();

	/**
	 * @brief Get latest sensor reading
	 * @return SensorData structure with latest values
	 */
	SensorData getLatestReading() const;

	/**
	 * @brief Register callback for sensor data updates
	 * @param callback Function to call when new data is available
	 */
	void registerDataCallback(SensorDataCallback callback);

	/**
	 * @brief Register callback for error handling
	 * @param callback Function to call when errors occur
	 */
	void registerErrorCallback(ErrorCallback callback);

	/**
	 * @brief Check if sensor is currently monitoring
	 * @return true if monitoring is active
	 */
	bool isMonitoring() const;

private:
	std::string m_chipName;
	int m_pin;
	std::unique_ptr<gpiod::chip> m_chip;
	std::unique_ptr<gpiod::line> m_dataLine;

	mutable std::mutex m_dataMutex;
	SensorData m_latestData;

	std::atomic<bool> m_monitoring{false};
	std::unique_ptr<std::thread> m_monitorThread;

	SensorDataCallback m_dataCallback;
	ErrorCallback m_errorCallback;

	/**
	 * @brief Internal method to read sensor data
	 * @return Array containing [humidity_high, humidity_low, temp_high, temp_low, checksum]
	 */
	std::array<uint8_t, 5> readRawData();

	/**
	 * @brief Validate checksum of sensor data
	 * @param data Raw sensor data array
	 * @return true if checksum is valid
	 */
	bool validateChecksum(const std::array<uint8_t, 5> &data) const;

	/**
	 * @brief Background monitoring thread function
	 * @param intervalMs Measurement interval in milliseconds
	 */
	void monitoringThread(int intervalMs);

	/**
	 * @brief Send timing pulse to DHT11
	 */
	bool sendStartSignal();

	/**
	 * @brief Wait for DHT11 response
	 */
	bool waitForResponse();

	/**
	 * @brief Read one bit from DHT11
	 */
	int readBit();
};
#endif
