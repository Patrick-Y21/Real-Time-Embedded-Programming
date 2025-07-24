#include "SystemController.h"
#include <iostream>
#include <csignal>
#include <memory>

// Global system controller for signal handling
std::unique_ptr<SystemController> g_systemController;

/**
 * @brief Signal handler for graceful shutdown
 * @param signal Signal number
 */
void signalHandler(int signal)
{
	std::cout << "\n[Main] Received signal " << signal << ", shutting down gracefully..." << std::endl;
	if (g_systemController)
	{
		g_systemController->stop();
	}
	exit(0);
}

/**
 * @brief Main function
 */
int main()
{
	std::cout << "[Main] Smart Curtain Control System Starting..." << std::endl;
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	try
	{
		// System configuration
		SystemController::SystemConfig config;
		config.gpioChipName = "gpiochip0";
		config.dht11Pin = 17;
		config.buzzerPin = 18;
		config.keypadCols = {{26, 19, 13, 6}};
		config.keypadRows = {{21, 20, 16, 12}};
		config.sensorReadInterval = 2000; // 2 seconds
		config.keypadScanInterval = 50;		// 50ms
		config.tempThreshold = 27;				// 27掳C
		config.humidityThreshold = 40;		// 40%

		// Create and initialize system controller
		g_systemController = std::make_unique<SystemController>(config);
		if (!g_systemController->initialize())
		{
			std::cerr << "[Main] Failed to initialize system controller" << std::endl;
			return -1;
		}
		// Start the system
		g_systemController->start();
		std::cout << "[Main] System running. Press Ctrl+C to stop." << std::endl;
		// Main event loop
		while (g_systemController->isRunning())
		{
			// Display system status periodically
			auto sensorData = g_systemController->getLatestSensorData();
			if (sensorData.isValid)
			{
				std::cout << "[Main] Status - Temp: " << sensorData.temperature
									<< "掳C, Humidity: " << sensorData.humidity << "%, "
									<< "Curtain: " << (g_systemController->getCurtainState() == SystemController::CurtainState::OPEN ? "OPEN" : "CLOSED")
									<< ", Mode: ";

				switch (g_systemController->getSystemState())
				{
				case SystemController::SystemState::MANUAL_MODE:
					std::cout << "MANUAL";
					break;
				case SystemController::SystemState::AUTO_MODE:
					std::cout << "AUTO";
					break;
				case SystemController::SystemState::ALARM_MODE:
					std::cout << "ALARM";
					break;
				}
				std::cout << std::endl;
			}
			// Sleep to reduce CPU usage
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "[Main] Exception: " << e.what() << std::endl;
		return -1;
	}

	std::cout << "[Main] System shutdown complete." << std::endl;
	return 0;
}
