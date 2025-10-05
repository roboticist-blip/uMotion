#include "board_config.h"
#include "camera_pins.h"
#include <uMotion.h>

uMotion motion;

void setup() {
  Serial.begin(115200);
  motion.begin(FRAMESIZE_QQVGA, PIXFORMAT_GRAYSCALE);
  motion.setMode(MODE_SUMMARY);
  motion.setThreshold(10);
}

void loop() {
  motion.update();

  if (motion.motionDetected()) {
    Serial.printf("[Motion Detected] Pixels: %d | Ratio: %.4f\n",
                  motion.getMotionPixels(),
                  motion.getMotionRatio());
  }

  delay(10);
}
