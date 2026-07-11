# μMotion

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
---
**How μMotion is Different from OpenCV Motion Detection**


OpenCV is a general-purpose computer vision library.
μMotion is a motion intelligence engine purpose-built for embedded vision, real-time performance, and AI-assisted decision systems.
Built for the edge. Faster than OpenCV.

Open for contributions and test !

---

## Citations
The Work is Published in the IEEE Conference, You can access it:
```bash
@INPROCEEDINGS{11594222,
  author={Maheshwari, Sumit and Kumar, Surendra and Chauhan, Nitin Kumar and Jain, Ankit},
  booktitle={2026 IEEE 18th International Conference on Computational Intelligence and Communication Networks (CICN)}, 
  title={A Resource-Constrained Implementation and Characterization of Per-Pixel Motion Detection on Low-Power Embedded Vision Systems}, 
  year={2026},
  volume={},
  number={},
  pages={927-931},
  keywords={Memory;Error analysis;Lighting;Printing;Testing;Timing;Pixel;Cameras;Motion detection;Algorithms;Embedded vision;Frame differencing;ESP32-S3;Motion detection;Edge computing;Resource constraints},
  doi={10.1109/CICN70047.2026.11594222}}
```
