#include </home/pi/Delay.h>
#include "/home/pi/DHT11.h"
#include "/home/pi/Key.h"
#include <ctime>
#include <thread>
#include <atomic>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <functional>
#include <vector>


using StateCallback = std::function<void(int)>; 
using SensorCallback = std::function<void(int, int)>; 


class CallbackManager {
public:
    void registerStateCallback(StateCallback cb) {
        state_callbacks.push_back(cb);
    }

    void registerSensorCallback(SensorCallback cb) {
        sensor_callbacks.push_back(cb);
    }

    void notifyStateChanged(int state) {
        for(auto& cb : state_callbacks) {
            cb(state);
        }
    }

    void notifySensorUpdated(int temp, int hum) {
        for(auto& cb : sensor_callbacks) {
            cb(temp, hum);
        }
    }

private:
    std::vector<StateCallback> state_callbacks;
    std::vector<SensorCallback> sensor_callbacks;
};


CallbackManager callbackManager;

int hum, tem;
int *data_head;
int *key;
int clock_hour=0, clock_hour_half=0;
int clock_hour_state=0, clock_hour_half_state=0;
int state = 0;
void receiverThread(int fd) {
    char buffer[256];
    while(1) {
        int len = read(fd, buffer, sizeof(buffer)-1);
        if (len > 0) {
            buffer[len] = '\0';
            if(buffer[0]==1)
            {
				state = 1;
			}
			else if(buffer[0]==0)
            {
				state = 0;
			}
			callbackManager.notifyStateChanged(state);
            //std::cout << "state: " << state << std::endl;
            
        }
        usleep(10000); // 10ms
    }
}
int main()
{
	int flag_dht = 1, flag_clock = 0;
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start; // duration 
	double duration_in_s = duration_cast<nanoseconds>(duration).count()  /1e3;
	gpiod::chip chip("gpiochip0");
	gpiod::line beep = chip.get_line(18);
	gpiod::line DO_sesor = chip.get_line(23);
	beep.request({"beep", gpiod::line_request::DIRECTION_OUTPUT, 0});
	DO_sesor.request({"DO_sesor", gpiod::line_request::DIRECTION_INPUT});
	init_gpio(chip);
	
	int fd = open("/dev/rfcomm0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        std::cerr << "" << std::endl;
        return -1;
    }
        callbackManager.registerStateCallback([](int state) {
        std::cout << "[callback]:state update " << (state ? "Open" : "Close") << std::endl;
    });

    callbackManager.registerSensorCallback([](int temp, int hum) {
        std::cout << "[callback] update: " << temp << " C, " << hum << " %" << std::endl;
        if(temp > 27) {
            std::cout << "[callback] tem high!" << std::endl;
        }
    });


    std::thread t(receiverThread, fd);
	while(1)
	{
		
		if (state==0)
		{
			//std::cout << "close" << std::endl;
		}
		else
		{
			//std::cout << "Open" << std::endl;
		}
		
		std::time_t now = std::time(nullptr);
    
		std::tm* local_time = std::localtime(&now);
    
		//std::cout << "time: " 
              //<< local_time->tm_year + 1900 << "-"  // 1900
              //<< local_time->tm_mon + 1 << "-"      // 
              //<< local_time->tm_mday << " "
              //<< local_time->tm_hour << ":"
              //<< local_time->tm_min << ":"
              //<< local_time->tm_sec << std::endl;
		//dht11

		if(flag_dht)
		{
			data_head = DHT11_read("gpiochip0", 17);
			std::cout << "hum:" << data_head[0] << " tem:" << data_head[2] << std::endl;
			tem = data_head[2];
			hum = data_head[0];
			callbackManager.notifySensorUpdated(tem, hum);
			start = std::chrono::high_resolution_clock::now();
						
			std::cout << "DO:" << DO_sesor.get_value() << std::endl;
						
			flag_dht = 0;
		}
		end = std::chrono::high_resolution_clock::now();
		duration = end - start; // duration 
		duration_in_s = duration_cast<nanoseconds>(duration).count()  /1e9;
		if(duration_in_s > 2.0)
		{
			flag_dht = 1;
		}
		if(tem>27)
		{
			beep.set_value(1);
		}
		else
		{
			beep.set_value(0);
		}
		
		key = Key_scan();
		//std::cout << "KEY" << key[0] << key[1] << std::endl;
		if(key[0] == 1 && key[1] == 1)
		{
			std::cout << "manual mode" << std::endl;
			key[0] = 0;
			key[1] = 0;
		}
		else if(key[0] == 1 && key[1] == 2)
		{
			state = 0;
			std::cout << "manual mode Close" << std::endl;
			key[0] = 0;
			key[1] = 0;
		}
		else if(key[0] == 1 && key[1] == 3)
		{
			state = 1;
			std::cout << "manual mode Open" << std::endl;
			key[0] = 0;
			key[1] = 0;
		}
		else if(key[0] == 1 && key[1] == 4)
		{
			std::cout << "auto mode" << std::endl;
			if(tem>20 && hum>40)
			{
				std::cout << "auto mode Open" << std::endl;
			}
			else
			{
				std::cout << "auto mode Close" << std::endl;
			}
		}
		if(key[0] == 2 && key[1] == 1)
		{
			clock_hour = clock_hour + 1;
			key[0] = 0;
			key[1] = 0;
		}
		else if(key[0] == 2 && key[1] == 2)
		{
			clock_hour_half  = clock_hour_half + 30;
			key[0] = 0;
			key[1] = 0;

		}
		else if(key[0] == 2 && key[1] == 3)
		{
			clock_hour = (local_time->tm_hour + clock_hour + (local_time->tm_min + clock_hour_half)/60)%24;
			clock_hour_half  = (local_time->tm_min + clock_hour_half)%60;
			key[0] = 0;
			key[1] = 0;
			flag_clock = 1;
			std::cout << "clock" << clock_hour << ":" << clock_hour_half << std::endl;
		}
		if(((clock_hour+clock_hour_half/60)%24 == local_time->tm_hour) && (clock_hour_half == local_time->tm_min) && flag_clock)
		{
			beep.set_value(1);
			clock_hour = 0;
			clock_hour_half = 0;
			flag_clock = 0;
		}
		if(key[0] == 2 && key[1] == 4)
		{
			beep.set_value(0);
			key[0] = 0;
			key[1] = 0;
		}
		//std::cout << "clock" << (clock_hour+clock_hour_half/60)%24 << ":" << clock_hour_half%60 << std::endl;
 	}
 	
 	t.join();
    close(fd);
	return 0;
}
