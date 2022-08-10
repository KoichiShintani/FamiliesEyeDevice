#pragma once

#include <Arduino.h>
#include "TaskBase.h"

#define BUZZER_TASK_PRIORITY    1
#define BUZZER_TASK_STACK_SIZE  2048
#define BUZZER_QUEUE_SIZE       5

class Buzzer : TaskBase {
  public:
    Buzzer(int pin, uint8_t ledc_channel): TaskBase("Buzzer Task", BUZZER_TASK_PRIORITY, BUZZER_TASK_STACK_SIZE), pin(pin), ledc_channel(ledc_channel) {
      ledcSetup(ledc_channel, 880, 4);
      ledcAttachPin(pin, ledc_channel);
      create_task();
      onceSount = OFF;
      music = MUTE;
    }
    virtual ~Buzzer() {}
    enum OnceSound{
      BOOT,
      SELECT,
      CONFIRM,
      CANCEL,
      COMPLETE,
      START,
      OFF,
    };
    enum Music {
      FALL_DOWN,
      SOS,
      HELP,
      ERROR,
      MUTE,
    };
    void play(const enum Music music) {
      this->music = music;
    }
    void once_sound(const enum OnceSound onceSount) {
      this->onceSount = onceSount;
    }
  private:
    int pin;
    uint8_t ledc_channel;
    Music music;
    OnceSound onceSount;
    
    void sound(const note_t note, uint8_t octave, uint32_t time_ms) {
      ledcWriteNote(ledc_channel, note, octave);
      vTaskDelay(time_ms / portTICK_RATE_MS);
    }
    void mute(uint32_t time_ms = 400) {
      ledcWrite(ledc_channel, 0);
      vTaskDelay(time_ms / portTICK_RATE_MS);
    }
    void task() {
      while (1) {
        if(onceSount != OFF){
          switch (onceSount) {
            case BOOT:
              sound(NOTE_B, 5, 200);
              sound(NOTE_E, 6, 400);
              sound(NOTE_Fs, 6, 200);
              sound(NOTE_B, 6, 600);
              mute();
              break;
            case SELECT:
              sound(NOTE_C, 6, 100);
              mute();
              break;
            case CONFIRM:
              sound(NOTE_C, 6, 100);
              sound(NOTE_E, 6, 100);
              mute();
              break;
            case CANCEL:
              sound(NOTE_E, 6, 100);
              sound(NOTE_C, 6, 100);
              mute();
              break;
            case COMPLETE:
              sound(NOTE_C, 6, 100);
              sound(NOTE_D, 6, 100);
              sound(NOTE_E, 6, 100);
              sound(NOTE_F, 6, 100);
              sound(NOTE_G, 6, 100);
              sound(NOTE_A, 6, 100);
              sound(NOTE_B, 6, 100);
              sound(NOTE_C, 7, 100);
              mute();
              break;
             case START:
              sound(NOTE_C, 6, 200);
              sound(NOTE_E, 6, 200);
              sound(NOTE_G, 6, 200);
              sound(NOTE_C, 7, 500);
              mute();
              break;
            case OFF:
            default:
              mute();
              break;
          }
          onceSount = OFF;
        }
        else{
          switch (music) {
             case FALL_DOWN:
              sound(NOTE_C, 6, 100);
              sound(NOTE_C, 7, 100);
              mute();
              break;
             case SOS:
              sound(NOTE_C, 7, 200);
              sound(NOTE_A, 6, 200);
              sound(NOTE_C, 7, 200);
              sound(NOTE_A, 6, 200);
              sound(NOTE_C, 7, 200);
              sound(NOTE_A, 6, 200);
              mute();
              break;
             case HELP:
              sound(NOTE_C, 7, 100);
              mute(100);
              sound(NOTE_C, 7, 100);
              mute(100);
              sound(NOTE_C, 7, 100);
              mute(100);
              sound(NOTE_C, 7, 100);
              mute(100);
              sound(NOTE_C, 7, 100);
              mute();
              break;
            case ERROR:
              for (int i = 0; i < 6; i++) {
                sound(NOTE_C, 7, 100);
                sound(NOTE_E, 7, 100);
              }
              mute();
              break;
            case MUTE:
            default:
              mute();
              break;
          }
        }
        delay(1);
      }
    }
};
