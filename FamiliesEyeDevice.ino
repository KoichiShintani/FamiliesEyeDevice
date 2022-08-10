#include "esp32io.h"
#include "appIf.h"
#include "Buzzer.h"
#include "Led.h"
#include "light_sensor.h"
#include "VibMotor.h"
#include "State.h"
#include "service/service_fall_detection.h"

Buzzer bz(GPIO_PIN_BUZZER, LEDC_CH_BUZZER);
Led_RGB led(GPIO_PIN_LED_R,GPIO_PIN_LED_G,GPIO_PIN_LED_B);
LightSensor LightSensor(GPIO_PIN_LIGHT_SENSOR, 1000);
DeviceState ds;
Vmotor vm(GPIO_PIN_VIBE_MOTOR);

int imuStatus;  // 9軸センサーステータス

void setup() {
  Serial.begin(115200);
  bz.once_sound(Buzzer::BOOT);
  // 夜間照明
  LightSensor.setThreshold(25);
  
  // 転倒検知
  imuStatus = service_fall_detection::initService();
  Serial.print("imuStatus = ");
  Serial.println(imuStatus);
  if (imuStatus < 0) {
      Serial.println("IMU initialization unsuccessful.");
  }

  apif::init(); // BLE初期設定
  apif::startBLEService(); // BLE通信機能の起動
}

void loop() {
  uint32_t prev;
  
  //状態変化時にアクション
  if(ds.isChangeState == 1){
    ds.isChangeState = 0;
    if(ds.getState() == DeviceState::WAIT){
      Serial.println("WAIT");
      bz.once_sound(Buzzer::SELECT);
      bz.play(Buzzer::MUTE);
      led.setLedColour(Led_RGB::OFF);
      vm.set_cycle(0);
    }
    else if(ds.getState() == DeviceState::GO_OUT){
      Serial.println("GO_OUT");
      bz.once_sound(Buzzer::START);
      bz.play(Buzzer::MUTE);
      //LEDは明るさ判定で夜間照明コントロール
      vm.set_cycle(0);
    }
    else if(ds.getState() == DeviceState::HELP){
      Serial.println("HELP");
      bz.play(Buzzer::HELP);
      led.setLedBlinkTimeMs(100);
      led.setLedColour(Led_RGB::YELLOW);
      vm.set_cycle(0);
    }
    else if(ds.getState() == DeviceState::FALL_DOWN){
      Serial.println("FALL_DOWN");
      bz.play(Buzzer::FALL_DOWN);
      led.setLedBlinkTimeMs(100);
      led.setLedColour(Led_RGB::PURPLE);
      vm.set_cycle(0);
      apif::setFallDown();
    }
    else if(ds.getState() == DeviceState::SOS){
      Serial.println("SOS");
      bz.play(Buzzer::SOS);
      led.setLedBlinkTimeMs(100);
      led.setLedColour(Led_RGB::RED);
      vm.set_cycle(150);
      apif::setSos();
    }
    else if(ds.getState() == DeviceState::CAMERA){
      Serial.println("CAMERA");
      led.setLedBlinkTimeMs(0);
      led.setLedColour(Led_RGB::BLUE);
      vm.set_cycle(0);
    }
  }
  // 夜間判断
  if(ds.getState() == DeviceState::GO_OUT){
    LightSensor.update();
    uint32_t now = millis();
    if (now - prev >= 1000) {
      prev = now;
      // 同じ状態が一定時間続いているかを判定する
      if (LightSensor.hasPassedSinceSameState(2000)) {
        uint8_t isBright = LightSensor.getState();
        //Serial.println(LightSensor.getLux(),4);
        //Serial.println("LightSensor");
        //Serial.print(isBright);
        if(isBright){
          led.setLedColour(Led_RGB::OFF);
        }
        else{
          led.setLedBlinkTimeMs(0);
          led.setLedColour(Led_RGB::WHITE);
        }
      }
    }
  }
  
  // センサーデータの処理
  if (imuStatus >= 0) {
    service_fall_detection::executeService();
    
  }
  delay(1);
}
