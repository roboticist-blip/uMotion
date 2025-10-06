#include "board_config.h"
#include "camera_pins.h"
#include <uMotion.h>

uMotion motion;

// Select your board in board_config.h first //

void setup() {
  Serial.begin(115200);
  motion.begin(FRAMESIZE_QQVGA, PIXFORMAT_GRAYSCALE);
  motion.setMode(MODE_RAW);     
  motion.setThreshold(10);        //Sensitivity threshold
  Serial.println("μMotion: RAW_DIFF mode started...");
}

void loop() {
  motion.update();  
  delay(100);       
}
