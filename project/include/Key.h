#ifndef Key_H
#define Key_H

#include </home/pi/Delay.h>
#include <gpiod.hpp>
#include <iostream>
#include <vector>

void init_gpio(gpiod::chip& chip);

int* Key_scan();


#endif 


