DHT11.cpp
#include "/home/pi/Delay.h"
#include "/home/pi/DHT11.h"
int data[5] = {0, 0, 0, 0, 0};
int* DHT11_read(const char* chip_str, int pin)
{
 gpiod::chip chip(chip_str);
 gpiod::line led = chip.get_line(pin);
 led.request({"blink_demo", gpiod::line_request::DIRECTION_OUTPUT, 0});
 
 delay_ms(25);
 
 led.set_value(1); 
 usleep(25);
 //std::cout << "init done" << std::endl;
 
 led.release();
 led.request({"blink_demo", gpiod::line_request::DIRECTION_INPUT});
 //std::cout << "IO state input" << std::endl;
 
 while(led.get_value() == 0);
 
 while(led.get_value() == 1);
 
 
 for(int m = 0; m<5; m++)
 {
 int data_now = 0;
 for(int i = 0; i<8; i++)
 {
 auto start = std::chrono::high_resolution_clock::now();
 while(led.get_value() == 0);
 auto end = std::chrono::high_resolution_clock::now();
 auto duration = end - start; // duration 
 double duration_in_us = duration_cast<nanoseconds>(duration).count()
 /1e3;
 //std::cout << "DHT 11 Data Start" << duration_in_us << "us" <<
std::endl;
 
 start = std::chrono::high_resolution_clock::now();
 while(led.get_value() == 1)
 {
 
 auto mid = std::chrono::high_resolution_clock::now();
 duration = mid - start; // duration 
 duration_in_us =
duration_cast<nanoseconds>(duration).count() /1e3;
 if(duration_in_us>120)
 {
 break;
 }
 }
 end = std::chrono::high_resolution_clock::now();
 duration = end - start; // duration 
 duration_in_us = duration_cast<nanoseconds>(duration).count() /1e3;
 //std::cout << "DHT 11 Data" << duration_in_us << "us" << std::endl;
 
 
 if((duration_in_us) > 40)
 {
 data_now = (data_now<<1) + 1;
 }
 else
 {
 data_now = (data_now<<1) + 0;
 }
 }
 data[m] = data_now;
 //std::cout << "hum_h" << data[0] << std::endl;
 //std::cout << "data_now" << data_now << std::endl;
 }
 //std::cout << "hum_h" << data[0] << std::endl;
 uint16_t temp = data[0] + data[1] + data[2] + data[3];
 if((temp & 0xFF) == data[4])
 {
 //std::cout << "Check OK!!!" << std::endl;
 
 return data;
 }
 else
 {
 std::cout << "Check Fail!!!" << std::endl;
 //std::cout << "hum_h" << data[0] << std::endl;
 return data;
 }
 
}
