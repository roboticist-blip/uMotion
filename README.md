# uMotion

μMotion is a **lightweight, easy-to-use motion detection library** for Arduino boards using ESP32 cameras.  
It supports multiple detection modes, adjustable thresholds, and simplifies camera setup across different ESP32 board configurations.

## Features

- Motion detection on ESP32 camera modules (OV2640/OV3660/other supported ESP32-CAM cameras)
- Two detection modes:
  - **SUMMARY Mode** – Counts total pixels that changed between frames
  - **RAW_DIFF Mode** – Prints per-pixel differences as ASCII visualization
- Adjustable sensitivity threshold
- Automatic camera setup based on included `board_config.h` and `camera_pins.h`
- Minimal code required in user sketches
- Compatible with Arduino IDE

---

## Installation

1. Download or clone the repository into your Arduino `libraries` folder:

```bash
git clone https://github.com/roboticist-blip/μMotion
```
OpenCV is a general-purpose computer vision library.
uMotion is a motion intelligence engine purpose-built for embedded vision, real-time performance, and AI-assisted decision systems.
Built for the edge. Faster than OpenCV.

Open for contributions and test !
