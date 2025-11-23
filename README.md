# IOT-Enabled-smart-dustbin
Using esp32 make a smart dustbin. Works on principle of top layer distance detection using ultrasonic sensor.

]

1. Project Abstract

An IoT-based smart dustbin designed to make waste management easier and more efficient through real-time monitoring. An ultrasonic sensor detects the distance from the top of the bin to the waste level, while the ESP32 processes this data and acts as a lightweight web server, providing real-time updates to any connected client (web browser). This system provides facility managers with accurate, actionable data on fill levels, reducing manual checking and enabling smart, optimized waste collection scheduling.

2. Problem Statement

Traditional waste management relies on fixed collection schedules or inefficient manual inspection to determine if bins are full. This leads to several issues:

Wasted Resources: Collection vehicles travel to partially empty bins, increasing fuel consumption and costs.

Overflow and Hygiene: Bins often overflow before the scheduled collection, leading to unsanitary conditions, odor issues, and aesthetic degradation in public and private spaces.

Lack of Real-Time Data: Municipality or facility managers lack real-time visibility into waste levels, making dynamic route optimization impossible.

3. Solution Overview

The IoT based Smart Dustbin is a retrofit monitoring solution that transforms a standard bin into a smart, connected device. The physical device consists of a plastic enclosure housing the ESP32 and a top-mounted ultrasonic sensor for level detection. The focus is purely on collecting, processing, and visualizing data to enable smarter decision-making.

The device interacts with the cloud and user as follows:

Data Collection: The ultrasonic sensor continuously measures the distance from the top down to the trash. This distance is inverted and converted into a distance-from-bottom level, which is then mapped to a percentage-full level.

Edge Processing & Visualization: The ESP32 micro-controller cleans, aggregates, and calculates the percentage. It then acts as a web server (see Section 9, Appendix) to serve a live HTML page displaying the current fill percentage and distance in real-time.

User Access: Any device connected to the same Wi-Fi network can access the bin's status simply by navigating to the ESP32's assigned IP address.

4. Hardware & Technical Stack

4.1 Components List

Component

Function

Microcontroller

ESP32 (Used for its integrated Wi-Fi and robust processing capability)

Sensors

Ultrasonic Sensor (HC-SR04 or similar, for distance/fill-level measurement)

Power

Micro USB data cable/Power Supply

4.2 Communication Protocols

Device-to-User/Visualization: HTTP over Wi-Fi (ESP32 acting as a web server)
Device-to-Sensor: Digital I/O (GPIO)

5. Circuit/Architecture Diagram

Diagram Description

The Ultrasonic Sensor's Trig pin (responsible for sending the sound pulse) is connected to ESP32 GPIO 32, and the Echo pin (responsible for receiving the reflected pulse) is connected to ESP32 GPIO 35. The ESP32 is powered via the VCC and GND pins. The ESP32 processes the time difference to calculate the distance, and then sends the status level to the connected web page using its internal Wi-Fi capability.

6. Prototype & Implementation

Physical Build: The prototype was initially constructed on a standard breadboard for rapid development and testing. The ultrasonic sensor was mounted onto a simple test frame above a bin. For a final presentation, the components were housed in a small 3D-printed enclosure attached to the side of a standard household dustbin, ensuring the sensor pointed directly down.

Software: The firmware, detailed in the arduino_firmware.ino file in the Appendix, was written in C++ using the Arduino IDE. The core logic involves:

Connecting to a known Wi-Fi network.

Initializing the WiFiServer on port 80.

Continuously measuring the distance via the Ultrasonic sensor.

Serving a responsive HTML page with a fill-level bar and status text when a client connects.

7. Challenges & Solutions

Challenge

Solution Implemented

Sensor Accuracy

The ultrasonic sensor sometimes gave inconsistent readings due to the shape of the trash or uneven surfaces. Solution: Implemented firmware averaging (taking the median of 5 consecutive readings) to stabilize the output data.

Power Management

The Wi-Fi connection continuously drained the battery (for battery-powered scenarios). Solution: Implemented deep sleep mode on the ESP32, waking up only every 15 minutes to take a reading and transmit data, significantly extending battery life.

Web Server Latency

Serving the entire HTML page directly from the ESP32 on every request can be slow. Solution: The page includes a <meta http-equiv='refresh' content='5'> tag to automatically refresh every 5 seconds, ensuring the status remains updated without complex JavaScript implementation, making the solution simple and lightweight.

8. Scalability & Commercial Viability

Estimated Cost per Unit: $8 - $12 (Based on bulk pricing)

Mass Manufacturing Feasibility: High. All components are standard, widely available, and low-cost (commodity electronics). The design can be easily adapted as a simple, modular kit that attaches to existing bins. While the current implementation uses a local web server, for mass deployment, the data stream can easily be switched from serving HTML to pushing lightweight JSON data to a centralized cloud platform (like Google Cloud IoT Core or AWS IoT), allowing the system to scale horizontally to handle millions of devices. The primary commercial application is in smart city initiatives and large-scale industrial or corporate campus waste management.

9. Appendix: ESP32 Firmware Code

This C++ code runs on the ESP32, manages the sensor readings, and serves the live status page over Wi-Fi.
