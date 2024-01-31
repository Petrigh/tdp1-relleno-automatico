[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause) [![Build Status]


# Conveyor Belt and Hopper Control System

## Overview

This project involves the implementation of a sophisticated conveyor belt and hopper control system using the C programming language. The core of the system is powered by the EDU-CIAA-NXP microcontroller, providing a robust and reliable foundation for automation.

## Features

- **Conveyor Belt Control:** Precise management of the conveyor belt speed and direction to optimize material handling.
- **Hopper Automation:** Efficient filling of boxes through a well-coordinated hopper mechanism.
- **Weight Sensor:** Utilizes a weight sensor to check if the box is full, enhancing the accuracy of the filling process.
- **Infrared Sensor:** Stops the conveyor belt when needed, providing an additional layer of safety and control.
- **Peripheral Integration:** The project includes a custom-designed board that seamlessly connects and manages all peripheral devices, ensuring a streamlined communication process.
- **User Interface:** Intuitive controls and feedback mechanisms for easy monitoring and interaction.

## Hardware Components

- **Microcontroller:** EDU-CIAA-NXP serves as the brain of the system, offering ample computational power and versatile I/O capabilities.
- **Custom Board:** A dedicated board designed for seamless integration and communication between the microcontroller and various peripherals.
- **Conveyor Belt and Hopper:** Physical components responsible for material movement and box filling.
- **Sensors and Actuators:** Integrated sensors, including a weight sensor and an infrared sensor, for precise feedback and enhanced control.

## Code Structure

The C codebase is organized to ensure clarity, modularity, and maintainability. Key modules include:

- **Conveyor Control Module:** Implements functions to manage the conveyor belt's speed and direction.
- **Hopper Control Module:** Contains the logic for efficient operation of the hopper during box filling.
- **Peripheral Interface Module:** Facilitates communication between the microcontroller and the custom-designed board.
- **User Interface Module:** Handles user input and provides feedback on the system's status.

## Getting Started

1. Clone the repository to your development environment.
2. Set up the necessary development tools for the EDU-CIAA-NXP microcontroller.
3. Flash the compiled code onto the microcontroller.
4. Connect the custom-designed board and peripheral devices as per the provided documentation.
5. Power on the system and start enjoying the automated conveyor belt and hopper control.


## License

This project is licensed under the [3-Clause BSD License](https://opensource.org/licenses/BSD-3-Clause). Feel free to use, modify, and distribute the code for your specific needs.
