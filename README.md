1.Writting

Matrix keyboard R1 - 4 connects to 21 20 16 12 C1 - 4 connects to 26 19 13 6 (see Key.cpp)

Photosensitive AO DO DO output output---23 vcc 5v

Temperature and humidity bus----17 vcc 5v

Buzzer level pin 18

Stepper motor IN1–IN4 connects to 27 22 24 25

2.Step

Start Bluetooth scanning, Bluetooth debugging assistant link

sudo rfcomm watch hci0

After the connection is completed, use the command to compile and execute

g++ -std=c++17 -o test /home/pi/main.cpp Delay.cpp DHT11.cpp Key.cpp BYJ.cpp -lgpiodcxx // (already compiled, you can ignore and execute test directly)

./test //Execute command

The callback interface can be seen in the execution window to update the temperature and humidity, status value [callback style output]

3.Control

From top to bottom, from left to right, there are eight buttons in total

1-Manual mode

2and3-Control status switch

4-Automatic mode (in this mode, the switch is judged by temperature and humidity) condition if(tem>20 && hum>40) open. At this time, when the temperature exceeds 27 degrees Celsius, the buzzer sounds and the curtain starts to close automatically.

5-Clock setting hours (n hours delayed from the current time)

6-Clock setting half hours (n half hours delayed from the current time, n>=2 automatically carries to hours)

7-Clock confirmation setting, output the set clock time, and the alarm will sound when the time is up (buzzer sounds)

8-Buzzer off

In addition, the temperature is too high and the buzzer is on. Bluetooth sends 0 off, 1 on (hex)
