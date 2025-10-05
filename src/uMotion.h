#ifndef UMOTION_H
#define UMOTION_H

#include "esp_camera.h"

enum uMotionMode {
  MODE_SUMMARY,
  MODE_RAW
};

class uMotion {
  public:
    bool begin(framesize_t size = FRAMESIZE_QQVGA, pixformat_t format = PIXFORMAT_GRAYSCALE);
    void setMode(uMotionMode mode);
    void setThreshold(uint8_t thresh);
    void update();
    bool motionDetected();
    int  getMotionPixels();
    float getMotionRatio();
    unsigned long getLatency();

  private:
    void captureFrame();
    void calculateDifference();
    void printRawDiff();

    uMotionMode mode = MODE_SUMMARY;
    uint8_t threshold = 10;
    bool motion = false;
    int motion_pixels = 0;
    float motion_ratio = 0.0f;
    unsigned long latency = 0;

    uint8_t *prevFrame = nullptr;
    camera_fb_t *fb = nullptr;
    unsigned long frame_count = 0;
    unsigned long t_last_log = 0;
};
#endif
