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
	
