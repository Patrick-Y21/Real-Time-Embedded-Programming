BYJ.cpp
#include <iostream>
#include <chrono>
#include <vector>
#include <gpiod.hpp>
#include "/home/pi/Delay.h"
const std::vector<std::vector<int>> steps = {
 {1, 0, 0, 0}, 
 {1, 1, 0, 0}, 
 {0, 1, 0, 0}, 
 {0, 1, 1, 0}, 
 {0, 0, 1, 0}, 
 {0, 0, 1, 1}, 
 {0, 0, 0, 1}, 
 {1, 0, 0, 1} 
};
const std::vector<unsigned int> outputPins = {27, 22, 24, 25};
void debug_print_step(int step_idx) {
 std::cout << "Step " << step_idx << ": ";
 for (auto val : steps[step_idx]) {
 std::cout << val << " ";
 }
 std::cout << std::endl;
}
int main() {
 try {
 gpiod::chip chip("gpiochip0");
 std::cout << "GPIO chip opened successfully" << std::endl;
 auto lines = chip.get_lines(outputPins);
 gpiod::line_request config;
 config.consumer = "stepper";
 config.request_type = gpiod::line_request::DIRECTION_OUTPUT;
 
 lines.request(config);
 std::cout << "GPIO lines requested" << std::endl;
 std::cout << "\nTesting single step..." << std::endl;
 lines.set_values({1, 0, 0, 0});
delay_ms(1); 
 std::cout << "\nStarting main loop..." << std::endl;
 while (true) {
 std::cout << "--- Clockwise ---" << std::endl;
 for (int i = 0; i < 512; ++i) { 
 int step_idx = i % 8;
 debug_print_step(step_idx);
 lines.set_values(steps[step_idx]);
 delay_ms(1);
 }
 std::cout << "--- Counter-clockwise ---" << std::endl;
 for (int i = 0; i < 512; ++i) {
 int step_idx = 7 - (i % 8);
 debug_print_step(step_idx);
 lines.set_values(steps[step_idx]);
delay_ms(1); }
 }
 } catch (const std::exception& e) {
 std::cerr << "ERROR: " << e.what() << std::endl;
 return 1;
 }
 return 0;
}
