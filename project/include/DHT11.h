#ifndef DHT11_H
#define DHT11_H

#include <gpiod.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <cstdio>
#include <unistd.h>
using namespace std::chrono;

int* DHT11_read(const char* chip_str, int pin);




#endif 


