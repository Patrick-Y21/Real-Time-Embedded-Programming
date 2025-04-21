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
a.Responsible for the overall division of labor of the project.
b.
c.


![1745175979635](https://github.com/user-attachments/assets/77649bca-f610-4e7a-a673-a43ac6213d58)
![1745175943880](https://github.com/user-attachments/assets/57872223-252a-43fc-bc87-5ea963f2a128)
![1745175830348](https://github.com/user-attachments/assets/c8e1efec-e4dd-49ef-a84f-3c88dbc6db82)
![1745175799089](https://github.com/user-attachments/assets/bafa43f0-10bc-4673-9af9-85b5fe0df92b)
![1745175684636](https://github.com/user-attachments/assets/92075178-1eab-4748-bea0-cd02e0954bdf)




