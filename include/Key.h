#ifndef KEY_H
#define KEY_H

#include <Delay.h>
#include <gpiod.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include <array>
#include <mutex>
#include <chrono>

/**
 * @brief 4x4 Matrix Keypad Class
 * Provides real-time keypad scanning with callback-based event handling
 */
class MatrixKeypad
{
public:
	// Callback type for key press events
	using KeyPressCallback = std::function<void(int row, int col, char key)>;
	using ErrorCallback = std::function<void(const std::string &error)>;

	// Key data structure
	struct KeyData
	{
		int row;
		int col;
		char keyChar;
		std::chrono::steady_clock::time_point timestamp;
		bool isPressed;
	};

	/**
	 * @brief Constructor
	 * @param chipName GPIO chip name
	 * @param colPins Array of column pin numbers
	 * @param rowPins Array of row pin numbers
	 */
	MatrixKeypad(const std::string &chipName,
							 const std::array<int, 4> &colPins,
							 const std::array<int, 4> &rowPins);

	/**
	 * @brief Destructor
	 */
	~MatrixKeypad();

	/**
	 * @brief Initialize the keypad
	 * @return true if initialization successful
	 */
	bool initialize();

	/**
	 * @brief Start continuous keypad scanning
	 * @param scanIntervalMs Scan interval in milliseconds
	 */
	void startScanning(int scanIntervalMs = 50);

	/**
	 * @brief Stop keypad scanning
	 */
	void stopScanning();

	/**
	 * @brief Get last key press data
	 * @return KeyData structure with last key press information
	 */
	KeyData getLastKeyPress() const;

	/**
	 * @brief Register callback for key press events
	 * @param callback Function to call when key is pressed
	 */
	void registerKeyPressCallback(KeyPressCallback callback);

	/**
	 * @brief Register callback for error handling
	 * @param callback Function to call when errors occur
	 */
	void registerErrorCallback(ErrorCallback callback);

	/**
	 * @brief Check if keypad is currently scanning
	 * @return true if scanning is active
	 */
	bool isScanning() const;

	/**
	 * @brief Convert row/col to character
	 * @param row Row number (0-3)
	 * @param col Column number (0-3)
	 * @return Character representation of key
	 */
	char getKeyChar(int row, int col) const;

private:
	std::string m_chipName;
	std::array<int, 4> m_colPins;
	std::array<int, 4> m_rowPins;

	std::unique_ptr<gpiod::chip> m_chip;
	std::vector<std::unique_ptr<gpiod::line>> m_colLines;
	std::vector<std::unique_ptr<gpiod::line>> m_rowLines;

	mutable std::mutex m_dataMutex;
	KeyData m_lastKeyData;

	std::atomic<bool> m_scanning{false};
	std::unique_ptr<std::thread> m_scanThread;

	KeyPressCallback m_keyPressCallback;
	ErrorCallback m_errorCallback;

	// Keypad layout
	static constexpr std::array<std::array<char, 4>, 4> KEYPAD_LAYOUT = {{{{'1', '2', '3', 'A'}},
																																				{{'4', '5', '6', 'B'}},
																																				{{'7', '8', '9', 'C'}},
																																				{{'*', '0', '#', 'D'}}}};

	/**
	 * @brief Background scanning thread function
	 * @param scanIntervalMs Scan interval in milliseconds
	 */
	void scanningThread(int scanIntervalMs);

	/**
	 * @brief Scan the keypad matrix once
	 * @return KeyData if key pressed, empty KeyData if no key pressed
	 */
	KeyData scanMatrix();

	/**
	 * @brief Debounce key press
	 * @param row Row of key
	 * @param col Column of key
	 * @return true if key press is valid after debouncing
	 */
	bool debounceKey(int row, int col);
};

#endif
