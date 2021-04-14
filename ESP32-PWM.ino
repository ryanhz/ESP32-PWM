/*
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
/* Generates PWM on Internal LED Pin GPIO 2 of ESP32*/

#include <driver/ledc.h>
#define LED_PIN 2 //On Board LED

// setting PWM properties
const int freq = 30000;
const int ledChannel = 0;
const int resolution = 11;
const int fadeduration = 2000; //200 ms total fade duration
volatile int brightness = 0;

unsigned long previousMillis = 0;
const long interval = 2500; 

//=======================================================================
//                    Power on setup
//=======================================================================
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN,OUTPUT);
  
  // configure LED PWM functionalitites
  //ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO2 to be controlled
  //ledcAttachPin(LED_PIN, ledChannel);
  
  ledc_timer_config_t ledc_timer;  
  ledc_timer.speed_mode   = LEDC_HIGH_SPEED_MODE;
  ledc_timer.timer_num    = LEDC_TIMER_1;
  ledc_timer.bit_num      = (ledc_timer_bit_t) resolution;
  ledc_timer.freq_hz      = freq;
  ledc_channel_config_t ledc_channel;
  ledc_channel.channel    = LEDC_CHANNEL_1;
  ledc_channel.gpio_num   = LED_PIN;
  ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_channel.timer_sel  = LEDC_TIMER_1;
  ledc_channel.duty       = 0;
  ledc_timer_config(&ledc_timer);
  ledc_channel_config(&ledc_channel);
  ledc_fade_func_install(ESP_INTR_FLAG_SHARED);
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if(brightness==0){
      brightness = 255;
    }
    else {
      brightness = 0;
    }

    //brightness = (brightness + 255) % 256;
    int fade = int(brightness * 8.027450980392157);
    Serial.println("Fading to " + String(fade));
    setfade(LEDC_CHANNEL_1, fade, fadeduration, LEDC_FADE_WAIT_DONE);
    //setfade2(LEDC_CHANNEL_1, fade, LEDC_FADE_WAIT_DONE);
  }
}
//=======================================================================

void setfade(ledc_channel_t channel, uint32_t fadeTo, int duration, ledc_fade_mode_t wait) {  
  ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE, channel, fadeTo, duration);
  ledc_fade_start(LEDC_HIGH_SPEED_MODE, channel, wait);
}

void setfade2(ledc_channel_t channel, uint32_t fadeTo, ledc_fade_mode_t wait) {  
  ledc_set_fade_with_step(LEDC_HIGH_SPEED_MODE, channel, fadeTo, 1, 1);
  ledc_fade_start(LEDC_HIGH_SPEED_MODE, channel, wait);
}
