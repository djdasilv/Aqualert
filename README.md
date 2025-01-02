# Aqualert

## Overview

Aqualert is an innovative solution designed to tackle communication breakdowns during floods, enabling stranded individuals to efficiently communicate with rescue services while ensuring their safety at home. This repository contains all the code used to program the Aqualert device, facilitating both the main communication controller and the user interface.

## Repository Structure

This repository is organized into two main branches, each dedicated to different components of the Aqualert system:

### 1. `ESPMain` Branch

**Purpose:**  
Contains the code to program the main ESP32 microcontroller, which oversees all communication functionalities of the Aqualert device.

**Key Features:**
- Manages data transmission between emitters and receivers.
- Implements the LoRa protocol for reliable, low-power communication.
- Coordinates with other system components to ensure seamless operation during flood events.

**Getting Started:**
```bash
# Clone the repository and switch to the ESPMain branch
git clone https://github.com/yourusername/aqualert.git
cd aqualert
git checkout ESPMain

# Open the project in your preferred IDE (e.g., Arduino IDE, PlatformIO)
```

### 2. `ButtonTest` Branch

**Purpose:**  
Houses the code to control the LCD screen, which is managed by its own ESP32 microcontroller (CYD Monitor).

**Key Features:**
- Interfaces with the LCD to display real-time information and alerts.
- Handles user inputs via buttons to interact with the Aqualert system.
- Ensures the display remains clear and responsive during emergencies.

**Getting Started:**
```bash
# Clone the repository and switch to the ButtonTest branch
git clone https://github.com/yourusername/aqualert.git
cd aqualert
git checkout ButtonTest

# Open the project in your preferred IDE (e.g., Arduino IDE, PlatformIO)
```

## Installation

### Prerequisites

- **Hardware:**
  - ESP32 microcontrollers (main controller and CYD Monitor)
  - LCD screen
  - Buttons
  - LoRa modules

- **Software:**
  - Arduino IDE or PlatformIO
  - Necessary libraries for ESP32, LoRa, and LCD control

### Steps

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/yourusername/aqualert.git
   ```

2. **Switch to the Desired Branch:**
   ```bash
   git checkout ESPMain
   # or
   git checkout ButtonTest
   ```

3. **Open in IDE:**
   - Launch Arduino IDE or PlatformIO.
   - Open the project files from the respective branch.

4. **Install Dependencies:**
   - Ensure all required libraries are installed via the IDE’s library manager.

5. **Upload the Code:**
   - Connect your ESP32 device to your computer.
   - Select the appropriate board and port in the IDE.
   - Upload the code to the ESP32.

## Usage

- **Main Controller (`ESPMain`):**  
  Manages overall communication and coordination between devices during flood events.

- **CYD Monitor (`ButtonTest`):**  
  Controls the LCD display and handles user interactions, displaying critical information and alerts.
