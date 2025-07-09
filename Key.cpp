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

