#include "board_config.h"
#include "camera_pins.h"
#include <uMotion.h>

uMotion motion;

void setup() {
  Serial.begin(115200);
  motion.begin(FRAMESIZE_QQVGA, PIXFORMAT_GRAYSCALE);
  motion.setMode(MODE_RAW);       //  Raw difference visualization mode
  motion.setThreshold(10);        //  Sensitivity threshold
  Serial.println("μMotion: RAW_DIFF mode started...");
}

void loop() {
  motion.update();  // This will print "." for changed pixels and " " for unchanged ones
  delay(100);       // Control frame rate for readability
}
