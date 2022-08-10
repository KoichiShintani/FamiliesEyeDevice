#pragma once

#include <Arduino.h>
#include "TaskBase.h"
#include "config.h"


#define BUTTON_SAMPLING_MS        20

#define BUTTON_PRESS_LEVEL        1
#define BUTTON_LONG_PRESS_LEVEL_1 100 //20*100 = 200ms
#define BUTTON_LONG_PRESS_LEVEL_2 200 //20*200 = 400ms 
#define BUTTON_LONG_PRESS_LEVEL_3 300 //20*300 = 600ms

#define BUTTON_TASK_PRIORITY      2
#define BUTTON_STACK_SIZE         1024
#include <stdio.h>

class Button: TaskBase {
  public:
    Button(int set_pin) : TaskBase("Button Task", BUTTON_TASK_PRIORITY, BUTTON_STACK_SIZE) {
      pinMode(set_pin, INPUT_PULLUP);
      flags = 0x00;
      printf("btn create %d\n",set_pin);
      pin=set_pin;
      create_task();
    }
    virtual ~Button() {}

    union {
      uint8_t flags;                 /**< all flags             */
      struct {
        uint8_t pressed : 1;         /**< pressed               */
        uint8_t long_pressed_1 : 1;  /**< long-pressed level 1  */
        uint8_t long_pressed_2 : 1;  /**< long-pressed level 2  */
        uint8_t long_pressed_3 : 1;  /**< long-pressed level 3  */
        uint8_t pressing : 1;        /**< pressing              */
        uint8_t long_pressing_1 : 1; /**< long-pressing level 1 */
        uint8_t long_pressing_2 : 1; /**< long-pressing level 2 */
        uint8_t long_pressing_3 : 1; /**< long-pressing level 3 */
      };
    };
  private:
    int pin;
    int counter;

    virtual void task() {
      portTickType xLastWakeTime;
      xLastWakeTime = xTaskGetTickCount();
      while (1) {
        vTaskDelayUntil(&xLastWakeTime, BUTTON_SAMPLING_MS / portTICK_RATE_MS);
        
        if (digitalRead(pin) == LOW) {
          if (counter < BUTTON_LONG_PRESS_LEVEL_3 + 1)
            counter++;
          if (counter == BUTTON_LONG_PRESS_LEVEL_3)
            long_pressing_3 = 1;
          if (counter == BUTTON_LONG_PRESS_LEVEL_2)
            long_pressing_2 = 1;
          if (counter == BUTTON_LONG_PRESS_LEVEL_1)
            long_pressing_1 = 1;
          if (counter == BUTTON_PRESS_LEVEL)
            pressing = 1;
        } else {
          if (counter >= BUTTON_LONG_PRESS_LEVEL_3)
            long_pressed_3 = 1;
          else if (counter >= BUTTON_LONG_PRESS_LEVEL_2)
            long_pressed_2 = 1;
          else if (counter >= BUTTON_LONG_PRESS_LEVEL_1)
            long_pressed_1 = 1;
          else if (counter >= BUTTON_PRESS_LEVEL)
            pressed = 1;
          counter = 0;
          flags &= 0x0F;
        }
      }
    }
};
