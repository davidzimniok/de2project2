# Project 2

Precise generation of several PWM channels. Application of two (or more) Servo motors SG90.

## Instructions

The goal of the project is cooperation in pairs, further study of the topic, design of own solutions, implementation, creation of project documentation and presentation of results. The division of roles and tasks within the team is up to their members.

* Students work on a project in the labs during the 12th and 13th weeks of the semester.

* Through the BUT e-learning, students submit a link to the GitHub repository, which contains the complete project in PlatfomIO and a descriptive README file. The submission deadline is the day before the next laboratory, i.e. in 2 weeks.

* The source code for the AVR must be written in C and/or Assembly and must be implementable on Arduino Uno board using toolchains from the semester, ie PlatformIO and not in the Arduino-style. No other development tools are allowed.

## Recommended GitHub repository structure

```c
   project2        // PlatfomIO project
   ├── include         // Included files
   │   └── timer.h
   ├── lib             // Libraries
   │   └── gpio
   │      └── gpio.c
   │      └── gpio.h
   │   └── lcd
   │      └── lcd.c
   │      └── lcd.h
   │      └── lcd_definitions.h
   ├── src             // Source file(s)
   │   └── main.c
   ├── platformio.ini  // Project Configuration File
   └── README.md       // Report of this project
   ```


### Team members

* Vojtěch Vídeňský (zodopovědný za github repozitář, video, testování aplikace)
* David Zimniok (zodpovědný za zdrojový kód)

## Hardware description

Insert descriptive text and schematic(s) of your implementation.

## Software description

Put flowchats of your algorithm(s). Write descriptive text of your libraries and source files. Put direct links to these files in `src` or `lib` folders.

## Video

Insert a link to a short video with your practical implementation example (1-3 minutes, e.g. on YouTube).

## References

1. Arduino UNO pinout: [https://docs.arduino.cc/retired/boards/arduino-uno-rev3-with-long-pins](https://docs.arduino.cc/retired/boards/arduino-uno-rev3-with-long-pins)
2. Práce s PWM a se servo motorky: [https://www.teachmemicro.com/arduino-servo-motor-tutorial/](https://www.teachmemicro.com/arduino-servo-motor-tutorial/)
3. Datasheet ATmega 328P: [https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)