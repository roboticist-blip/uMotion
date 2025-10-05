/*
  μMotion Library Example
  -----------------------
  Demonstrates both RAW_DIFF (pixel difference) and MOTION_COUNT (total motion pixel count)
  modes of the μMotion library.
*/

#include <uMotion.h>

uMotion motion

void setup() {
  Serial.begin(115200);
  Serial.println("μMotion Example Starting...");
  
  // Initialize μMotion library
  motion.begin();

  // Set detection mode and threshold
  // Modes: RAW_DIFF or MOTION_COUNT
  motion.setMode(MOTION_COUNT);
  motion.setThreshold(25);

  Serial.println("Camera Initialized. Starting motion processing...");
}

void loop() {
  // Process motion depending on selected mode
  motion.process();

  // Optional delay (can be tuned for speed vs. stability)
  delay(500);
}
