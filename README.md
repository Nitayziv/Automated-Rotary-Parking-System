# Automated Rotary Parking System

## 📌 Project Overview
An embedded control system for an automated rotary parking model, developed in C. The system manages vehicle entry, parking spot allocation, and exit using a combination of sensors, actuators, and a central microcontroller. It demonstrates hardware-software integration, real-time control, and peripheral communication protocols.

## ⚙️ Hardware Stack
* **Microcontroller:** Nuvoton NU-LB-NUC140 (ARM Cortex-M0)
* **Actuators:** * Stepper Motor (Rotary carousel positioning)
    * SG90 Micro Servo (Gate control via PWM)
* **Sensors & Modules:**
    * MFRC522 RFID Reader (Spot scanning via SPI)
    * HC-SR04 Ultrasonic Sensors (Vehicle detection at entry/exit)
    * Matrix Keypad & LCD Display (User interface)

## 💻 Software & Protocols
* **Language:** C
* **IDE:** Keil uVision
* **Protocols Used:** * **SPI:** MFRC522 RFID and LCD communication.
    * **UART:** Ultrasonic sensor data acquisition and Bluetooth (BT) communication.
    * **PWM:** Precise servo motor control for the gate.
    * **GPIO / Interrupts:** Matrix keypad input and stepper motor sequencing.

## 🚀 Key Features
1.  **Automated Entry:** Ultrasonic sensor detects an arriving vehicle. System identifies the user (via BT/Keypad) and opens the entry gate using a Servo motor.
2.  **Smart Allocation:** The rotary carousel (driven by a Stepper motor) aligns an available parking spot to the entrance.
3.  **RFID Verification:** Each parking spot is equipped with an RFID tag. The RFID reader scans the spot to ensure it is vacant before allocation.
4.  **Automated Exit:** System detects exiting vehicles, calculates the correct position of the user's vehicle, and rotates the carousel to retrieve it.

## 📁 Repository Structure
* `EXE9.c` / `users.c`: Main application logic, state machine, and feature implementations.
* `SPI_RC522.c / .h`: SPI drivers for the RFID module.
* `Driver_PWM_Servo.c / .h`: PWM control logic for the entry gate.
* `Smpl_GPIO_StepMotor.uvproj`: Keil uVision project file.

## 📸 Media & Schematics
<img width="412" height="545" alt="image" src="https://github.com/user-attachments/assets/54f00bf3-ea88-445e-890d-5221a4ce00a1" />
<img width="402" height="533" alt="image" src="https://github.com/user-attachments/assets/7bbd0386-9dd5-473b-b5a4-a05265f58243" />

https://youtu.be/P687uW1HrHI

<img width="1195" height="1113" alt="finalproject drawio (1)" src="https://github.com/user-attachments/assets/d1c99e1b-6d20-48bd-a6ea-55bfe0725518" />
<img width="289" height="920" alt="flowchart drawio (1)" src="https://github.com/user-attachments/assets/15ee2999-06eb-4a3d-afe1-6f8d23e205fe" />

*Developed as a laboratory project in Embedded Systems Design.*
