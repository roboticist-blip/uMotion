#include "uMotion.h"
#include "board_config.h"
#include "camera_pins.h"
#include <Arduino.h>

#define CAM_W   160
#define CAM_H   120

#define PROC_W   96
#define PROC_H   96

bool uMotion::begin(framesize_t size, pixformat_t format) {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = size;                 // still QQVGA
  config.pixel_format = format;             // grayscale
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("[μMotion] Camera init failed!");
    return false;
  }

  // Allocate previous processed frame
  prevFrame = (uint8_t*)malloc(PROC_W * PROC_H);
  if (!prevFrame) {
    Serial.println("[μMotion] Failed to allocate frame buffer!");
    return false;
  }
  memset(prevFrame, 0, PROC_W * PROC_H);

  frame_count = 0;
  t_last_log = millis();

  Serial.println("[μMotion] Camera initialized successfully.");
  return true;
}

void uMotion::setMode(uMotionMode m) { mode = m; }
void uMotion::setThreshold(uint8_t t) { threshold = t; }

void uMotion::update() {
  unsigned long t0 = micros();

  fb = esp_camera_fb_get();
  if (!fb) return;

  motion_pixels = 0;

  // ==========================
  // Downsample QQVGA → 96x96
  // ==========================
  for (int y = 0; y < PROC_H; y++) {
    int src_y = y * CAM_H / PROC_H;
    for (int x = 0; x < PROC_W; x++) {
      int src_x = x * CAM_W / PROC_W;
      this->procFrame[y * PROC_W + x] =
      fb->buf[src_y * CAM_W + src_x];
    }
  }

  if (mode == MODE_RAW)
    printRawDiff();
  else
    calculateDifference();

  memcpy(prevFrame, this->procFrame, PROC_W * PROC_H);
  esp_camera_fb_return(fb);

  unsigned long t1 = micros();
  latency = t1 - t0;

  motion_ratio = (float)motion_pixels / (PROC_W * PROC_H);
  motion = (motion_pixels > threshold);

  frame_count++;
  if (millis() - t_last_log > 5000) {
    float fps = frame_count / ((millis() - t_last_log) / 1000.0f);
    Serial.printf(
      "[μMotion] fps=%.2f, latency=%lu us, heap=%u, psram=%u\n",
      fps, latency,
      (unsigned)ESP.getFreeHeap(),
                  (unsigned)ESP.getFreePsram()
    );
    frame_count = 0;
    t_last_log = millis();
  }
}

void uMotion::calculateDifference() {
  for (int y = 0; y < PROC_H; y++) {
    for (int x = 0; x < PROC_W; x++) {
      int idx = y * PROC_W + x;
      if (abs(this->procFrame[idx] - prevFrame[idx]) > threshold) {
        motion_pixels++;
      }
    }
  }

  Serial.printf("motion_pixels=%d, ratio=%.4f\n",
                motion_pixels, motion_ratio);
}

void uMotion::printRawDiff() {
  for (int y = 0; y < PROC_H; y++) {
    for (int x = 0; x < PROC_W; x++) {
      int idx = y * PROC_W + x;
      if (abs(this->procFrame[idx] - prevFrame[idx]) > threshold)
        Serial.print(".");
      else
        Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println("-----------------------------");
}

bool uMotion::motionDetected() { return motion; }
int uMotion::getMotionPixels() { return motion_pixels; }
float uMotion::getMotionRatio() { return motion_ratio; }
unsigned long uMotion::getLatency() { return latency; }
