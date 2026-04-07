# Smart USB Charging Analytics System

## Overview

This project is an ESP32-based smart charging analytics system designed to monitor mobile phone charging behavior in real time. It measures charging voltage, current, power, cumulative energy consumption, and charging mode using sensor-based acquisition and cloud logging.

## Objective

The objective of this project is to analyze smartphone charging characteristics during different charging phases such as fast charging, taper charging, and slow charging.

## Hardware Components Used

* ESP32 Development Board
* INA219 Current Sensor
* SSD1306 OLED Display
* USB Charging Cable
* Voltage Divider Circuit
* Mobile Charger and Smartphone

## Software and Platforms Used

* Arduino IDE
* Blynk IoT Dashboard
* Google Sheets
* GitHub

## Working Principle

The positive charging line of the USB cable is passed through the INA219 sensor to measure current. Voltage is measured using an ESP32 ADC pin through a voltage divider circuit. ESP32 processes the sensor data and calculates:

* Voltage
* Current
* Power
* Energy Consumed

The processed values are displayed locally on the OLED display and simultaneously sent to Blynk cloud and Google Sheets for remote monitoring and storage.

## Charging Mode Detection

The system identifies charging phases using voltage and current values:

* Plug in Mobile
* Fast Charging
* Taper Charging
* Slow Charging

## Energy Calculation

Energy consumption is calculated continuously using:

E = P × t

where:

* E = Energy in Wh
* P = Power in W
* t = time interval in hours

## Features

* Real-time voltage measurement
* Real-time current measurement
* Power calculation
* Energy consumption tracking
* Charging mode classification
* OLED live display
* Blynk cloud monitoring
* Google Sheets data logging
* Charging graph generation

## Experimental Observation

During testing, the charging current remained nearly constant during the fast charging phase and gradually decreased during taper charging. The graph clearly showed current reduction as battery percentage increased.

## Result

The project successfully captured charging current variation over time and demonstrated real smartphone charging behavior using low-cost embedded hardware.

## Future Scope

* Charger efficiency comparison
* Battery health estimation
* Automatic charging completion alert
* Multiple charger analytics

## Repository Contents

* Source code
* Hardware images
* Block diagram
* OLED output images
* Charging graphs

## Author

Safal Singh
