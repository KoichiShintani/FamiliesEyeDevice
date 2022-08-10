#pragma once

#include <Arduino.h>
#include "TaskBase.h"

#define VMOTOR_TASK_PRIORITY    1
#define VMOTOR_TASK_STACK_SIZE  1024
#define VMOTOR_QUEUE_SIZE       5
#define CYCLE_MAX               1000  //(ms)

class Vmotor : TaskBase {
  public:
    Vmotor(int pin): TaskBase("Vmotor Task", VMOTOR_TASK_PRIORITY, VMOTOR_TASK_STACK_SIZE), pin(pin) {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
      this->pin = pin;
      create_task();
    }
    virtual ~Vmotor() {}
    void set_cycle(uint32_t cycle) {
      if(cycle > CYCLE_MAX){
        cycle = CYCLE_MAX;
      }
      this->cycle = cycle;
    }
  private:
    int pin;
    uint32_t cycle;
    void task() {
      while (1) {
        if(cycle == 0){
          digitalWrite(pin, LOW);
        }
        else{
          digitalWrite(pin, HIGH);
          vTaskDelay(cycle / portTICK_RATE_MS);
          digitalWrite(pin, LOW);
          vTaskDelay(cycle / portTICK_RATE_MS);
        }
        delay(1);
      }
    }
};
