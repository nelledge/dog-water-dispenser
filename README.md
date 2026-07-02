# Dog Water Dispenser

A small smart water dispenser project for a dog bowl. It uses an ESP32 to control a 12V motor or pump through a relay, with Home Assistant managing the operation over MQTT. The system can be turned on and off manually from Home Assistant or triggered from an iPhone shortcut.

## Project overview

This project was built to automate the filling of a dog water bowl. The main idea is simple:

- A 12V power source powers the pump motor.
- The 12V supply is stepped down to 5V for the ESP32.
- The ESP32 connects to Home Assistant through MQTT.
- A relay module switches the pump on and off.
- Home Assistant can control the pump manually or through automations.

The result is a reliable and flexible setup that lets the pump be controlled remotely without needing to interact with the hardware directly.

## System layout

The system works in a few simple steps:

1. A 12V supply powers the pump and the rest of the hardware.
2. A 5V rail is taken from the 12V supply to power the ESP32.
3. The ESP32 listens for MQTT commands.
4. When a command arrives, the ESP32 turns the relay on or off.
5. Home Assistant sends those MQTT commands and runs automations based on a boolean helper.

## Hardware used

- ESP32 microcontroller
- 12V motor or pump
- Relay module
- 12V power supply
- 5V power conversion for the ESP32
- Wiring for power and control signals

## How it works

- The ESP32 connects to Wi-Fi and an MQTT broker.
- It subscribes to an MQTT topic for relay control.
- When it receives the message ON, it turns the relay on and powers the pump.
- When it receives the message OFF, it turns the relay off and stops the pump.
- Home Assistant can trigger these commands through automations and scripts.

## Home Assistant

Home Assistant is the main control layer for the project.

### What Home Assistant does

- Sends MQTT commands to the ESP32
- Runs automations for turning the pump on and off
- Uses a boolean helper for manual control
- Allows the system to be controlled from the Home Assistant interface

### Manual control logic

A boolean helper is used to decide which automation should run:

- When the helper is turned on, the MQTT automation for turning the motor on is triggered.
- When the helper is turned off, the automation for turning the motor off is triggered.

This makes it possible to use a simple Home Assistant button or switch to control the motor manually.

## YAML automations

The YAML files in this repository are Home Assistant automations.

### Files

- mqttMotorOn.yaml
  - Sends an MQTT message with the payload ON
- mqttMotorOff.yaml
  - Sends an MQTT message with the payload OFF

These automations are used to control the relay through Home Assistant and MQTT.

## Apple Shortcut

An Apple Shortcut on iPhone is used to trigger the system from the Home Assistant app.

### Shortcut flow

- The shortcut runs from the iPhone.
- It opens the Home Assistant app.
- It triggers a Home Assistant script.
- The script sends an MQTT command to turn the motor on.
- The system waits 60 seconds.
- A second MQTT command turns the motor off.

This provides a simple one-tap way to run the dispenser without opening the full automation interface.

## Arduino firmware

The Arduino code in this repository is written for the ESP32 and handles the relay control.

### What the firmware does

- Connects to Wi-Fi
- Connects to an MQTT broker
- Subscribes to the relay control topic
- Receives ON and OFF messages
- Switches the relay accordingly

### Important note

The relay logic is set so that the pump is turned off at startup and only activated when a valid MQTT command is received.

## Project structure

- Arduino/
  - Contains the ESP32 firmware source file
- yaml/
  - Contains the Home Assistant automation YAML files
- README.md
  - Project documentation

## Photos and setup images

Add your photos here as the project is completed.

### Full project setup

[Insert photo of the complete dispenser setup here]

### Wiring close-up

[Insert photo of the ESP32, relay, and power wiring here]

### Home Assistant interface

[Insert screenshot of the Home Assistant controls here]

### Apple Shortcut idea

[Insert photo or screenshot of the shortcut workflow here]

## Notes

This project is a practical example of combining embedded hardware, MQTT, and Home Assistant into a simple smart home solution. It is a good starting point for anyone interested in automation, home control, or IoT devices.
