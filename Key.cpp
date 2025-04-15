Key.cpp
#include </home/pi/Delay.h>
#include <gpiod.hpp>
#include <iostream>
#include <vector>
const int cols[4] = {26, 19, 13, 6};
const int raws[4] = {21, 20, 16, 12};
int temp[2] = {0, 0};
std::vector<gpiod::line> col_io, raw_io;
void init_gpio(gpiod::chip& chip) {
 for (int pin : cols) {
 auto line = chip.get_line(pin);
 line.request({"cols", gpiod::line_request::DIRECTION_OUTPUT, 0});
 line.set_value(0); // 
 col_io.push_back(line);
 }
 //
 for (int pin : raws) {
 auto line = chip.get_line(pin);
 line.request({"rows", gpiod::line_request::DIRECTION_INPUT});
 raw_io.push_back(line);
 }
}
int * Key_scan()
{
	for (int m = 0; m < 4; ++m) {
 col_io[m].set_value(1); // 
 delay_ms(1); //
 
 for (int n = 0; n < 4; ++n) {
 if (raw_io[n].get_value()) {
 std::cout << "Key: " << n << "," << m << std::endl;
 temp[0] = n+1; temp[1] = m+1;
 while (raw_io[n].get_value()) delay_ms(10); //
 }
 }
 col_io[m].set_value(0); // 
 }
 
 return temp;
}
