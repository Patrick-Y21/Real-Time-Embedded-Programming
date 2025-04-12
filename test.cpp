#include <iostream>
#include <functional>
#include <thread>
#include <chrono>

class CallbackTester {
public:
    using StateCallback = std::function<void(int)>;
    using SensorCallback = std::function<void(int, int)>;

    void onStateChange(StateCallback cb) {
        state_cb_ = cb;
    }

    void onSensorUpdate(SensorCallback cb) {
        sensor_cb_ = cb;
    }

    void triggerStateChange(int state) {
        if(state_cb_) state_cb_(state);
    }

    void triggerSensorUpdate(int temp, int hum) {
        if(sensor_cb_) sensor_cb_(temp, hum);
    }

private:
    StateCallback state_cb_;
    SensorCallback sensor_cb_;
};

int main() {
    CallbackTester tester;

    tester.onStateChange([](int state) {
        std::cout << "[TEST] state update: " << (state ? "Open" : "Close") << std::endl;
    });

    tester.onSensorUpdate([](int temp, int hum) {
        std::cout << "[TEST] sensor: " << temp << " C, " << hum << "%" << std::endl;
    });

    std::cout << "=== start test ===" << std::endl;
    tester.triggerStateChange(1);      // 
    tester.triggerSensorUpdate(25, 60);// 
    tester.triggerStateChange(0);      // 

   
    return 0;
}
