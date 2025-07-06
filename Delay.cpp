#include <Delay.h>

void delay_us(int n)
{
	usleep(n);
}

void delay_ms(int n)
{
	usleep(n * 1000);
}

void delay_s(int n)
{
	sleep(n);
}
