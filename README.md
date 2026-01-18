# 🔥 Smart Fire Detection System

## 📂 Project Description
This project implements an IoT-based fire detection system integrated with Google Cloud Platform (GCP).
An IR flame sensor connected to a Cytron Maker Feather S3 (ESP32) detects fire events and publishes data using the MQTT protocol.
A Google Compute Engine (VM) subscribes to MQTT messages and stores fire alerts securely in Google Firestore.

---

## 🛠️ Development Environment Setup
 
### Hardware Requirements
  - Cytron Maker Feather S3 (ESP32)
  - IR Flame Sensor
    
### Tools
  - Google Compute Engine (VM)
  - Google Firestore
  - Arduino IDE

### Communication Protocol
  - MQTT

---

## 🚀 Installation & Usage Guide

### Step 1: Configure the Google Cloud VM Through SSH Terminal

1. Update the system:
   `sudo apt-get update`

2. Install pip:
   `sudo apt-get install python3-pip`

3. Install required Python libraries:
   `pip3 install google-cloud-firestore paho-mqtt`

4. Upload the middleware script to the VM:
   `mqtt_firestore_bridge.py`

5. Run the script:
   `python3 mqtt_firestore_bridge.py`

### Step 2: Configure Hardware

1. Assemble the ESP32 and IR Flame Sensor.
2. Open `fire_alarm.ino` in Arduino IDE.
3. Update the following configuration values:
   - WIFI_SSID
   - WIFI_PASSWORD
   - MQTT_SERVER (VM External IP)
   - MQTT_PORT
   - MQTT_TOPIC
4. Upload the firmware to the Cytron Maker Feather S3.

### Step 3: Test the System

1. Trigger the flame sensor using a heat source (e.g., lighter).
2. Observe MQTT messages in the SSH terminal.
3. Verify new data entries in the Google Firestore console.

---

## 📄 File Structure

`fire_alarm.ino`
- ESP32 firmware for flame detection and MQTT publishing

`mqtt_firestore_bridge.py`
- Google Cloud VM middleware for storing MQTT data in Firestore
