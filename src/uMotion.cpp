#include "uMotion.h"
#include "board_config.h"
#include "camera_pins.h"
#include <Arduino.h>

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
  config.frame_size = size;
  config.pixel_format = format;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("[μMotion] Camera init failed!");
    return false;
  }
  
  int frame_bytes = 160 * 120; // QQVGA assumed
  prevFrame = (uint8_t*)malloc(frame_bytes);
  if (!prevFrame) {
    Serial.println("[μMotion] Failed to allocate frame buffer!");
    return false;
  }
  memset(prevFrame, 0, frame_bytes);

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

  if (mode == MODE_RAW)
    printRawDiff();
  else
    calculateDifference();
  
  memcpy(prevFrame, fb->buf, fb->len);
  esp_camera_fb_return(fb);

  unsigned long t1 = micros();
  latency = t1 - t0;

  motion_ratio = (float)motion_pixels / (160.0f * 120.0f);
  motion = (motion_pixels > threshold);

  frame_count++;
  if (millis() - t_last_log > 5000) {
    float fps = frame_count / ((millis() - t_last_log) / 1000.0f);
    Serial.printf("[μMotion] fps=%.2f, latency=%lu us, heap=%u, psram=%u\n",
                  fps, latency, (unsigned)ESP.getFreeHeap(), (unsigned)ESP.getFreePsram());
    frame_count = 0;
    t_last_log = millis();
  }
}

void uMotion::calculateDifference() {
  for (int y = 0; y < 120; y++) {
    for (int x = 0; x < 160; x++) {
      int idx = y * 160 + x;
      if (abs(fb->buf[idx] - prevFrame[idx]) > threshold) {
        motion_pixels++;
      }
    }
  }

  Serial.printf("motion_pixels=%d, ratio=%.4f\n",
                motion_pixels,
                motion_ratio);
}

void uMotion::printRawDiff() {
  for (int y = 0; y < 120; y++) {
    for (int x = 0; x < 160; x++) {
      int idx = y * 160 + x;
      if (abs(fb->buf[idx] - prevFrame[idx]) > threshold)
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
