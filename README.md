ECE 425 - Microprocessor Systems, Fall 2024

Final Project: Pathfinder Robot

Lenny Marron,
Roger Xu

Instructor: Aaron Nanas


## **<u>Introduction:</u>**

This project fulfills the final project requirement for the following
classes at California State University, Northridge (CSUN):

### **<u>Project Goals</u>**

To design and assemble a small robot that involves the use of an
ultrasonic sensor, and Two DC geared motors to give the robot the
ability to follow a specified path while avoiding
obstacles/obstructions. Using software written in C with Keil as the
development tool, the software instructions are sent to the TM4C123GXL
hardware board to process the data. This project explores data
transmission and communication protocols like UART, motor control with
PWM, Timer, and main microcontroller-peripherals interactions.

## **<u>Background and Methodology:</u>**

We used UART module 1 to transmit and receive the distance measured by
the ultrasonic sensor to the microcontroller and we used UART module 0
to allow us to view the data seen by the microcontroller. Once the data
was confirmed to be correct we then commented out the code relating to
UART module 0.

The DC motors were driven by the DRV8833 breakout board which used 4
separate PWM inputs to allow the robot to move forward, backward, left,
and right. We based our motor control using logic in **Table 1** below.

**<u>Table 1 Motor Control Logic</u>**

<img src="media/image4.png" style="width:5.32435in;height:1.21868in" />

By using the DRV8833, it provided the ability to control 2 DC motors
with +3.3V input logic and a small footprint. The speed of the motors
was adjusted by changing the duty cycle of the PWM modules. When one
motor has a higher speed than the other, the car will turn in that
direction. When both motors have the same speed, the car will move
forward or backward.

Once the drivers for all 4 PWM signals and the 2 UART modules were
confirmed as working, a Timer Interrupt was created in order to
periodically check for any objects in front of the robots path. The
timer was set to count every 1 ms but would only check the distance in
front of it every 1 second. Whenever an object was less than 10 cm away
from the robot the robot would reverse and turn right, for all other
cases the robot would continue to drive forward.

We also 3D printed the ultrasonic sensor housing below in order to keep
the location of the US-100 sensor the same the entire time.

### **<u>CAD model of 3d Printed Parts</u>**

<img src="media/image1.png" style="width:3.65934in;height:2.97573in" /><img src="media/image7.png" style="width:3.09373in;height:2.83677in" />

## **<u>Block Diagram:</u>**

<img src="media/image2.png" style="width:6.49886in;height:4.07646in" />

## **<u>Table of Components:</u>**

| **Part** | **QTY** | **Picture** |
|----|----|----|
| 5 Channel IR Tracking sensor module | 1 | <img src="media/image11.jpg" style="width:2.21684in;height:1.02873in" /> |
| DC Geared Motors with wires | 1 | <img src="media/image12.jpg" style="width:2.42136in;height:1.90774in" /> |
| Adafruit DRV8833 DC/Stepper Motor Driver Breakout Board | 1 | <img src="media/image10.png" style="width:1.3776in;height:1.06076in" /> |
| EK-TM4C123GXL microcontroller | 1 | <img src="media/image8.png" style="width:2.49283in;height:1.66577in" /> |
| US-100 Ultrasonic sensor | 1 | <img src="media/image13.jpg" style="width:2.22396in;height:1.2778in" /> |
| +6V (AA) battery Pack | 1 | <img src="media/image6.png" style="width:1.72536in;height:1.66146in" /> |
| +5V Coast Rechargeable Battery | 1 | <img src="media/image9.png" style="width:2.21508in;height:0.82172in" /> |

## 

## **<u>Pinout Table :</u>**

<table>
<colgroup>
<col style="width: 25%" />
<col style="width: 25%" />
<col style="width: 25%" />
<col style="width: 25%" />
</colgroup>
<thead>
<tr>
<th style="text-align: center;"><strong>EK-TM4C123GXL
Microcontroller</strong></th>
<th style="text-align: center;"><strong>Function Used</strong></th>
<th style="text-align: center;"><p><strong>DRV8833</strong></p>
<p><strong>Breakout Board</strong></p></th>
<th style="text-align: center;"><p><strong>US-100</strong></p>
<p><strong>Ultrasonic Sensor</strong></p></th>
</tr>
<tr>
<th style="text-align: center;">PB6</th>
<th style="text-align: center;"><p>PWM0_0</p>
<p>(Right motor forward)</p></th>
<th style="text-align: center;">BIN1</th>
<th style="text-align: center;"></th>
</tr>
<tr>
<th style="text-align: center;">PB4</th>
<th style="text-align: center;"><p>PWM0_1</p>
<p>(Right motor backward)</p></th>
<th style="text-align: center;">BIN2</th>
<th style="text-align: center;"></th>
</tr>
<tr>
<th style="text-align: center;">PF2</th>
<th style="text-align: center;"><p>PWM1_3</p>
<p>(Left motor forward)</p></th>
<th style="text-align: center;">AIN2</th>
<th style="text-align: center;"></th>
</tr>
<tr>
<th style="text-align: center;">PA6</th>
<th style="text-align: center;"><p>PWM1_1</p>
<p>(Left motor back)</p></th>
<th style="text-align: center;">AIN1</th>
<th style="text-align: center;"></th>
</tr>
<tr>
<th style="text-align: center;">PA0 (U0RX)</th>
<th style="text-align: center;">UART0</th>
<th style="text-align: center;"></th>
<th style="text-align: center;"></th>
</tr>
<tr>
<th style="text-align: center;">PA1 (U0TX)</th>
<th style="text-align: center;">UART0</th>
<th style="text-align: center;"></th>
<th style="text-align: center;"></th>
</tr>
<tr>
<th style="text-align: center;">PB0 (U1RX)</th>
<th style="text-align: center;">UART1</th>
<th style="text-align: center;"></th>
<th style="text-align: center;">Echo/RX pin 3</th>
</tr>
<tr>
<th style="text-align: center;">PB1 (U1TX)</th>
<th style="text-align: center;">UART1</th>
<th style="text-align: center;"></th>
<th style="text-align: center;">Trig/TX pin 2</th>
</tr>
</thead>
<tbody>
</tbody>
</table>

### **<u>Schematic:</u>**

<img src="media/image5.png" style="width:7.80593in;height:5.08895in" />

## **<u>Analysis and Results :</u>**

One of the issues we ran into while working on this project was
implementing both the 5 channel IR tracking Sensors and the US-100
sensor at the same time. Although the interrupt function for the IR
sensor worked fine we always had errors when implementing both together.
Next time we will order an IR-sensor module that includes a datasheet
and comes from a reliable source. When troubleshooting this module using
a multimeter, we noticed the output voltages at ~ +1.7V when sensing the
black tape, which in turn caused the micro to read the input as a Logic
High, instead of Logic Low. We believe that if it weren’t due to the
lack of a datasheet we might have been able to fix this issue.

Another issue we faced is parsing the distance value measured by the
ultrasonic sensor to the microcontroller. We have had a lot of trouble
setting the microcontroller to measure the time it takes for the
ultrasonic sensor to receive its echo(which is how it measures
distance), we bypassed this issue by using UART as a communication
method instead of a direct pin connection. Aside from programming
obstacles, we have also experienced physical hardware roadblocks. The
ultrasonic sensor holder was not fitting properly, so we had to 3D print
new parts. However, despite all the setbacks, we were able to
successfully build a robot that scans its surroundings using sensors and
makes speed and direction changes based on its sensory inputs.

### **<u>Video Demonstration</u>**

Short video demonstration of collision avoidance:

- [<u>Video demo
  1</u>](https://drive.google.com/file/d/1N911rxExsTW1idhbEZjhIfGKM85qhewT/view?usp=drive_link)

- [<u>Video demo
  2</u>](https://drive.google.com/file/d/11KXWsSGZQswzX816z1DkIamCJ6btKC-2/view?usp=drive_link)

## **<u>Conclusion:</u>**

In conclusion, this project is the culmination of everything we have
learned in the class, from C programming to GPIO to GPTM to PWM to UART.
We connected different parts and peripherals to the microcontroller by
assigning and connecting them using GPIO pins. We applied communication
methods of UART to transfer distance data from the ultrasonic sensor to
the MCU by setting the appropriate baud rate. In addition we designed
the robot to periodically check its surroundings so as to not drain the
batteries as quickly. We learned how to use duty cycles to change the
speed of the motors by manipulating the duty cycle of the PWM, and thus
enabling us to adjust the speed of the motor. We also learned how to use
an H-bridge circuit to control the direction of our robot. We used
various C programming techniques to build up the control logic needed to
instruct the robot how to handle different situations. This project has
helped us gain a deeper understanding of MCUs and the crucial roles they
play in real life embedded systems.
