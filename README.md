Smart Curtain Control System (Raspberry Pi)

This is a multi-functional smart curtain control project based on Raspberry Pi. It supports Bluetooth control, environmental sensing, scheduled control, and manual interaction. Hardware includes a stepper motor, buzzer, temperature & humidity sensor, light sensor, and matrix keypad to realize a full-featured automatic curtain system.



 📷 Demo Video

 [https://youtu.be/zzqQ1Vp2ANM](https://youtu.be/zzqQ1Vp2ANM)

---
 1. Wiring Instructions

- **Matrix Keypad**
  - Rows R1~R4 → GPIO 21, 20, 16, 12  
  - Columns C1~C4 → GPIO 26, 19, 13, 6  
  - See `Key.cpp`

- **Light Sensor**
  - AO: unused  
  - DO → GPIO 23  
  - VCC → 5V

- **DHT11 Temp/Humidity Sensor**
  - DATA → GPIO 17  
  - VCC → 5V

- **Buzzer**
  - Signal pin → GPIO 18

- **Stepper Motor (28BYJ-48 + ULN2003)**
  - IN1~IN4 → GPIO 27, 22, 24, 25

---

 2. Operation Steps

 Start Bluetooth Monitoring

Use a Bluetooth debug tool or mobile terminal to connect. Then run:

```bash
sudo rfcomm watch hci0
```

After successful connection, proceed to compile and run the code.

 Compile and Execute

If not yet compiled:

```bash
g++ -std=c++17 -o test /home/pi/main.cpp Delay.cpp DHT11.cpp Key.cpp -lgpiodcxx
```

To run:

```bash
./test
```

The terminal will show callback updates for temperature, humidity, and system state.

---

3. Keypad Control Overview

The matrix keypad has 8 functional buttons arranged top-down, left-right:

| Button No. | Function |
|------------|----------|
| ①          | Enter Manual Mode |
| ②③         | Control curtain open/close (Manual mode only) |
| ④          | Auto Mode: When temperature > 27°C, buzzer sounds and curtain closes |
| ⑤          | Set timer hours (add n hours from current time) |
| ⑥          | Set timer half-hours (add n × 30 min; auto carry to hour if ≥60 min) |
| ⑦          | Confirm time setting, outputs target time. When reached, buzzer sounds and curtain closes |
| ⑧          | Turn off buzzer manually |

---

 Bluetooth Control

- Send `01` (hex) → Turn ON buzzer  
- Send `00` (hex) → Turn OFF buzzer  

> Works via any serial Bluetooth terminal app.

---

 Auto Mode Logic

In auto mode, system decides curtain status based on sensor readings:

```cpp
if (tem > 20 && hum > 40)
    open curtain;

if (tem > 27)
{
    buzzer on;
    curtain close;
}
```

---

 File Structure

| File         | Description                        |
|--------------|------------------------------------|
| `main.cpp`   | Main logic and mode control        |
| `DHT11.cpp`  | DHT11 temperature/humidity reading |
| `Key.cpp`    | Matrix keypad scanning             |
| `Delay.cpp`  | Microsecond/millisecond delays     |
| `BYJ.cpp`    | Stepper motor control sequence     |
| `blueth.cpp` | Bluetooth input handling (optional)|

---
Team Contributions

Huatai Yu
·Student Number：2960703Y
·Contribution:
a. Responsible for the overall division of labor of the project.
b. Code writing for Bluetooth part and delay part.
c. Cmake callback part and source code upload.
d. Pay attention to the emergence of various problems and actively seek solutions.

Shuo Sun
·Student Number：2962190S
·Contribution:
a. Smart curtain overall architecture design.
b. Lead the hardware solution design and make selection suggestions.
c. Provide suggestions for later expansion, such as adding stepper motors.

Yihao Yang
·Student Number：2961277Y
·Contribution:
a. Responsible for component selection and procurement.
b. Design schematics and wiring diagrams and be responsible for GPIO pin planning.
c. Writing the code for the temperature and humidity sensor and the matrix keyboard.
d. Writing Comment Code.

Pengyu Wan
·Student Number：2960904W
·Contribution:
a. Main part of the code writing.
b. CMake build configuration script code writing.
c. Assist team members to write and integrate all codes.
d. Code writing for stepper motor test part.

Haoqing Li
·Student Number：2996440L
·Contribution:
a. Test part code writing.
b. Responsible for hardware and software testing and recording issues.
c. Responsible for subsequent video recording and promotion explanation.

## Key Improvements Made

### 1. **Classes & Encapsulation**

#### Before:
- Only one "callback manager" class 
- Everything else in C-style functions in main()
- Global variables everywhere
- Zero proper encapsulation

#### After:
- **3 well-designed C++ classes** with proper encapsulation:
  - `DHT11Sensor`: Complete sensor management with internal state
  - `MatrixKeypad`: Full keypad handling with debouncing and character mapping
  - `SystemController`: Main system orchestrator with state management
- **Private member variables** with public interfaces
- **Proper data hiding** using private/protected access specifiers
- **Method-based interfaces** instead of global functions

### 2. **Memory Management**

#### Before:
- Global variables and raw pointers
- Manual memory management
- Potential memory leaks

#### After:
- **RAII (Resource Acquisition Is Initialization)** pattern throughout
- **Smart pointers** (`std::unique_ptr`, `std::shared_ptr`) for automatic memory management
- **Automatic cleanup** in destructors
- **No memory leaks** - all resources automatically managed
- **Stack-based objects** where possible

### 3. **Real-time Event-Driven Architecture**

#### Before:
- Giant polling loop in main()
- Blocking delays and busy-waiting
- No real event handling

#### After:
- **Pure event-driven architecture** using callbacks
- **Background threads** for continuous monitoring
- **Non-blocking operations** with proper timing
- **Callback-based communication** between components
- **Minimal CPU usage** - no busy polling

### 4. **Real-time Code Structure**

#### Before:
- Everything in one big main() loop
- Mixed sensor reading, keypad scanning, and logic
- No separation of concerns

#### After:
- **Separate threads** for different real-time tasks:
  - Sensor monitoring thread (2-second intervals)
  - Keypad scanning thread (50ms intervals)  
  - Alarm monitoring thread (1-second intervals)
  - Bluetooth communication thread (10ms intervals)
- **Thread-safe communication** using mutexes and atomic variables
- **Configurable timing** for all real-time operations

## Architecture Overview

```
SystemController (Main Orchestrator)
├── DHT11Sensor (Temperature/Humidity)
│   ├── Background monitoring thread
│   ├── Data validation and checksum
│   └── Callback-based data delivery
├── MatrixKeypad (4x4 Keypad)
│   ├── Matrix scanning thread
│   ├── Debouncing logic
│   └── Character mapping
├── GPIO Management
│   ├── Buzzer control
│   └── Hardware abstraction
└── Bluetooth Communication
    ├── Non-blocking receiver thread
    └── Command processing
```

## New Features Added

1. **Comprehensive Error Handling**
   - Exception-safe code throughout
   - Error callbacks for component failures
   - Graceful degradation when hardware unavailable

2. **Thread-Safe Operations**
   - Mutex protection for shared data
   - Atomic variables for flags
   - Proper thread synchronization

3. **Configurable System**
   - `SystemConfig` struct for easy parameter adjustment
   - Runtime configuration without code changes

4. **Comprehensive Testing**
   - Unit tests for all major components
   - Memory management verification
   - Architecture compliance testing


## Usage

### Building the Project
```bash
cd project
mkdir build && cd build
cmake ..
make
```

### Running the System
```bash
./smart_curtain_system
```

### Running Tests
```bash
./test_comprehensive
```

## Hardware Requirements

- **Raspberry Pi** (or compatible ARM device)
- **DHT11** temperature/humidity sensor (GPIO 17)
- **4x4 Matrix Keypad** (configurable pins)
- **Buzzer** (GPIO 18)
- **Bluetooth module** (optional - /dev/rfcomm0)

## Control Interface

### Keypad Controls:
- **'1'**: Switch to Manual Mode
- **'2'**: Manual Close Curtain
- **'3'**: Manual Open Curtain  
- **'4'**: Switch to Auto Mode
- **'5'**: Increment Alarm Hour
- **'6'**: Increment Alarm Minutes (+30)
- **'7'**: Set Alarm
- **'8'**: Clear Alarm

### Auto Mode Logic:
- **Opens curtain** when: Temperature > 20°C AND Humidity > 40%
- **Closes curtain** otherwise

### Safety Features:
- **Temperature Alert**: Buzzer activates when temperature > 27°C
- **Alarm System**: Time-based alerts with buzzer
- **Graceful Shutdown**: Proper cleanup on SIGINT/SIGTERM


---

![1745175979635](https://github.com/user-attachments/assets/77649bca-f610-4e7a-a673-a43ac6213d58)
![1745175943880](https://github.com/user-attachments/assets/57872223-252a-43fc-bc87-5ea963f2a128)
![1745175830348](https://github.com/user-attachments/assets/c8e1efec-e4dd-49ef-a84f-3c88dbc6db82)
![1745175799089](https://github.com/user-attachments/assets/bafa43f0-10bc-4673-9af9-85b5fe0df92b)
![1745175684636](https://github.com/user-attachments/assets/92075178-1eab-4748-bea0-cd02e0954bdf)




