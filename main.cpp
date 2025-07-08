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
		config.tempThreshold = 27;				// 27°C
		config.humidityThreshold = 40;		// 40%

		// Create and initialize system controller
		g_systemController = std::make_unique<SystemController>(config);
		if (!g_systemController->initialize())
		{
			std::cerr << "[Main] Failed to initialize system controller" << std::endl;
			return -1;
		}
