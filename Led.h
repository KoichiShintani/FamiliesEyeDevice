#pragma once

#include <Arduino.h>
#include "TaskBase.h"

#define LED_TASK_PRIORITY    1
#define LED_TASK_STACK_SIZE  1024
#define LED_QUEUE_SIZE       5

#define LED_MAX_BLINK_TIME   1000

class Led_RGB : TaskBase {
  public:
    Led_RGB(int pin_r, int pin_g, int pin_b): TaskBase("LED Task", LED_TASK_PRIORITY, LED_TASK_STACK_SIZE), pin_r(pin_r), pin_g(pin_g), pin_b(pin_b){
      digitalWrite(pin_r, HIGH);//OFF(Hi-Z)
      digitalWrite(pin_g, HIGH);//OFF(Hi-Z)
      digitalWrite(pin_b, HIGH);//OFF(Hi-Z)
      pinMode(pin_r, OUTPUT_OPEN_DRAIN);
      pinMode(pin_g, OUTPUT_OPEN_DRAIN);
      pinMode(pin_b, OUTPUT_OPEN_DRAIN);
      this->pin_r = pin_r;
      this->pin_g = pin_g;
      this->pin_b = pin_b;
      blinkTime = -1;
      create_task();
    }
    virtual ~Led_RGB() {}
	enum LedState {
      OFF,
      RED,
      GREEN,
      BLUE,
      PURPLE,
      YELLOW,
      WHITE,
    };
    void setLedColour(const enum LedState ledState) {
      state = ledState;
    }
    void setLedBlinkTimeMs(int blinkTime){
      this->blinkTime = blinkTime;
      if(this->blinkTime > LED_MAX_BLINK_TIME){
        this->blinkTime = LED_MAX_BLINK_TIME;
      }
    }
  private:
    int pin_r;
    int pin_g;
    int pin_b;
    int blinkTime;
    enum LedState state;

    void task() {
      while (1) {
        switch (state) {
          case OFF:
            digitalWrite(pin_r, HIGH);
            digitalWrite(pin_g, HIGH);
            digitalWrite(pin_b, HIGH);
            break;
          case RED:
            digitalWrite(pin_r, LOW);
            digitalWrite(pin_g, HIGH);
            digitalWrite(pin_b, HIGH);
          break;
          case GREEN:
            digitalWrite(pin_r, HIGH);
            digitalWrite(pin_g, LOW);
            digitalWrite(pin_b, HIGH);
          break;
          case BLUE:
            digitalWrite(pin_r, HIGH);
            digitalWrite(pin_g, HIGH);
            digitalWrite(pin_b, LOW);
          break;
          case WHITE:
            digitalWrite(pin_r, LOW);
            digitalWrite(pin_g, LOW);
            digitalWrite(pin_b, LOW);
          break;
          case PURPLE:
            digitalWrite(pin_r, LOW);
            digitalWrite(pin_g, HIGH);
            digitalWrite(pin_b, LOW);
          break;
          case YELLOW:
            digitalWrite(pin_r, LOW);
            digitalWrite(pin_g, LOW);
            digitalWrite(pin_b, HIGH);
          break;
          default:
          break;
        }
        if(blinkTime > 1){
          portTickType wakeupTime = xTaskGetTickCount();
          vTaskDelayUntil( &wakeupTime, blinkTime);
          digitalWrite(pin_r, HIGH);
          digitalWrite(pin_g, HIGH);
          digitalWrite(pin_b, HIGH);
          wakeupTime = xTaskGetTickCount();
          vTaskDelayUntil( &wakeupTime, blinkTime);
        }
        delay(1);
      }
    }
};
