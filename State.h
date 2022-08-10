#pragma once

#include <Arduino.h>
#include "esp32io.h"
#include "TaskBase.h"
#include "Button.h"
#include "appIf.h"
#include "service/service_fall_detection.h"

#define DEVICESTATE_TASK_PRIORITY    1
#define DEVICESTATE_TASK_STACK_SIZE  1024

Button btn(GPIO_PIN_BUTTON);

class DeviceState : TaskBase{
  public:
    DeviceState(): TaskBase("DeviceState Task", DEVICESTATE_TASK_PRIORITY, DEVICESTATE_TASK_STACK_SIZE){
      state = WAIT;
      isChangeState = 0;
      create_task();
    }
    virtual ~DeviceState() {}
    enum State{
      WAIT,
      GO_OUT,
      HELP,
      FALL_DOWN,
      SOS,
      CAMERA,
    };
    State getState(){
      return state;
    }
    int isChangeState;
  private:
    State state;
    void task() {
      
      while (1) {
        switch (state) {
            case WAIT:
                if(apif::goOut()==1){
                  btn.pressed = 0;
                  isChangeState = 1;
                  state = GO_OUT;
                }
                else if(apif::camera() == 1){
                  btn.long_pressing_1 = 0;
                  isChangeState = 1;
                  state = CAMERA;
                  Serial.println("CAMERA");
                }
                break;
            case CAMERA:
                if(apif::camera()==0){
                  btn.pressed = 0;
                  isChangeState = 1;
                  state = WAIT;
                  Serial.println("WAIT");
                }
                break;
            case GO_OUT:
                if(apif::help()==1){
                  btn.pressed = 0;
                  isChangeState = 1;
                  state = HELP;
                  Serial.println("HELP");
                }
                else if(apif::goOut()==0){
                  isChangeState = 1;
                  state = WAIT;
                  Serial.println("WAIT");
                }
                else if(service_fall_detection::getState() == 1){
                  btn.long_pressing_1 = 0;
                  isChangeState = 1;
                  state = FALL_DOWN;
                  Serial.println("FALL_DOWN");
                }
                break;
            case HELP:
                if(apif::help() == 0){
                  btn.pressed = 0;
                  isChangeState = 1;
                  state = GO_OUT;
                  Serial.println("GO_OUT");
                }
                break;
            case FALL_DOWN:
                if(apif::safety() == 1){
                  btn.pressed = 0;
                  isChangeState = 1;
                  state = GO_OUT;
                  Serial.println("GO_OUT");
                }
                break;
            default:
              break;
        }
        if(btn.long_pressing_3 || apif::sos() == 1){
          state = SOS;
          isChangeState = 1;
          Serial.println("SOS");
        }
        delay(1);
      }
    }
};
