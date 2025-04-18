#include <gpiod.hpp>
#include <unistd.h>
#include <vector>
#include <iostream>

class Stepper {
public:
    Stepper(const std::vector<int>& gpio_pins, const std::string& chip_name = "gpiochip0")
        : pins(gpio_pins), chip(chip_name) {
        for (int pin : pins) {
            gpiod::line line = chip.get_line(pin);
            line.request({"stepper", gpiod::line_request::DIRECTION_OUTPUT, 0});
            lines.push_back(line);
        }
    }

    // Run motor forward (clockwise)
    void stepForward(int steps = 512, int delay_us = 2000) {
        for (int i = 0; i < steps; ++i) {
            for (int s = 0; s < 8; ++s) {
                applyStep(step_sequence[s]);
                usleep(delay_us);
            }
        }
        stop();
    }

    // Run motor backward (counter-clockwise)
    void stepBackward(int steps = 512, int delay_us = 2000) {
        for (int i = 0; i < steps; ++i) {
            for (int s = 7; s >= 0; --s) {
                applyStep(step_sequence[s]);
                usleep(delay_us);
            }
        }
        stop();
    }

    // Stop motor
    void stop() {
        for (auto& line : lines) {
            line.set_value(0);
        }
    }

    // Set manual direction (true: forward, false: backward)
    void setManualDirection(bool forward) {
        manual_forward = forward;
    }

    // Unified control logic based on different modes
    void controlByMode(bool manual, bool auto_mode, bool timer_mode, bool light_on, int temp, int hum, bool time_match) {
        if (manual) {
            if (manual_forward) {
                std::cout << "[Manual] Closing curtain" << std::endl;
                stepForward(512);
            } else {
                std::cout << "[Manual] Opening curtain" << std::endl;
                stepBackward(512);
            }
        }
        else if (auto_mode) {
            if (!light_on) {
                std::cout << "[Auto] No light - Closing curtain" << std::endl;
                stepForward(512);
            }
            else if (temp > 30) {
                std::cout << "[Auto] High temp - Closing curtain" << std::endl;
                stepForward(512);
            }
            else if (hum > 80) {
                std::cout << "[Auto] High humidity - Opening curtain" << std::endl;
                stepBackward(512);
            }
        }
        else if (timer_mode && time_match) {
            std::cout << "[Timer] Time reached - Closing curtain" << std::endl;
            stepForward(512);
        }
    }

private:
    gpiod::chip chip;
    std::vector<int> pins;
    std::vector<gpiod::line> lines;
    bool manual_forward = true;

    // Half-step sequence for 28BYJ-48
    const int step_sequence[8][4] = {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1}
    };

    // Apply one step to the motor
    void applyStep(const int seq[4]) {
        for (size_t i = 0; i < lines.size(); ++i) {
            lines[i].set_value(seq[i]);
        }
    }
};
