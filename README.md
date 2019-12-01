# AtteONE firmware

AtteONE is a mini rover developed by Team DIANA at Politecnico di Torino for educational purposes. This is the official repository of its firmware.

[![forthebadge](https://forthebadge.com/images/badges/ages-12.svg)](https://forthebadge.com)[![forthebadge](https://forthebadge.com/images/badges/powered-by-netflix.svg)](https://forthebadge.com)[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)

## Hardware

- Controller: it is designed to work on **ESP32 module**. (We used a Wemos Lolin32)
- Motor driver: a dual H-bridge (**L298N**).
- Power: it requires two **LiPo cells** (ex, two 18650 batteries).

## Software and setup

### Arduino IDE
Download the IDE from the official site:
[Arduino IDE](https://www.arduino.cc)

### ESPTOOL v2.6 or greater
Follow here the instruction to setup Arduino IDE:
[arduino-esp32](https://github.com/espressif/arduino-esp32)

### USB to UART Driver

Download the drivers from the official site: [CP210x USB to UART Bridge VCP Drivers](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)

### MQTT

Read the official page of PubSubClient library for API documentation: [PubSubClient](https://pubsubclient.knolleary.net)

## LED indicator

| LED Sequence  | Meaning  |
|---|---|
| Fast blink  | Rover operational   |  
| Slow blink  | Rover remotely disabled  |  
| _W_ (Morse) | Connecting to Wifi   | 
| _B_ (Morse) | Connecting to MQTT Broker  |  