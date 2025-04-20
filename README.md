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

g++ -std=c++17 -o test /home/pi/main.cpp Delay.cpp DHT11.cpp Key.cpp  -lgpiodcxx // (already compiled, you can ignore and execute test directly)

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


Here's a link to our presentation and promotional video:  https://youtu.be/zzqQ1Vp2ANM
![1745175979635](https://github.com/user-attachments/assets/77649bca-f610-4e7a-a673-a43ac6213d58)
![1745175943880](https://github.com/user-attachments/assets/57872223-252a-43fc-bc87-5ea963f2a128)
![1745175830348](https://github.com/user-attachments/assets/c8e1efec-e4dd-49ef-a84f-3c88dbc6db82)
![1745175799089](https://github.com/user-attachments/assets/bafa43f0-10bc-4673-9af9-85b5fe0df92b)
![1745175684636](https://github.com/user-attachments/assets/92075178-1eab-4748-bea0-cd02e0954bdf)




