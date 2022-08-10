#pragma once
#include <cmath>
#include "Arduino.h"

/**
 * LightSensorクラス
 * [Usage]
 * 
 * LightSensor *pLightSensor;
 * void setup() {
 *     // 間引き間隔を1000ms測定
 *     pLightSensor = new LightSensor(PIN_NUM, 1000);
 *     pLightSensor->setThreshold(1200); // 明暗のしきい値(ルクス)を指定
 * }
 * 
 * void loop() {
 *     pLightSensor->update();
 *
 *     uint8_t isBright = pLightSensor->getState();
 *     if (isBright) {
 *         // 明るい時の処理
 *     }
 *     else {
 *         // 暗い時の処理
 *     }
 * 
 *     // 同じ状態が一定時間続いているかを判定する
 *     if (pLightSensor->hasPassedSinceSameState(2000)) {
 *         // 2秒以上継続
 *         isBright = pLightSensor->getState();
 *         if (isBright) {
 *             // 明るい時の処理
 *         }
 *         else {
 *             // 暗い時の処理
 *         }
 * 
 *     }
 * }
 */
class LightSensor {
private:
    uint8_t _pin; // pin number
    uint8_t _state; // 現在の状態(0: 暗い, 1: 明るい)
    uint8_t _lastState; // 1つ前の状態(0 or 1)
    uint8_t _changed; // 変化があったかどうか(0 or 1)
    uint32_t _threshold; // 明暗のしきい値 (型 仮設定)
    uint32_t _time; // 現在時間(ms)
    uint32_t _lastTime; // 前回読み取り時の時間(ms)
    uint32_t _changeTime; // 変化があった時間(ms)
    uint32_t _dbTime; // 間引き間隔の調整(ms)
    double _lux; // ルクス値

public:
    LightSensor(uint8_t pin, uint32_t dbTIme); // constructor
    void setThreshold(uint32_t threshold); // しきい値の設定
    void update(); // 状態の更新
    double pinValueToLux(int pinVal); // pinのanalog値をルクス値に変換
    uint8_t calcState();
    double getLux();
    uint8_t getState();// 明暗判定
    uint8_t hasPassedSinceSameState(uint32_t time); // 同じ状態で一定時間を経過しているか
};

// public
LightSensor::LightSensor(uint8_t pin, uint32_t dbTime):
    _pin(pin), _dbTime(dbTime), _threshold(0), _changed(0)
{
    pinMode(_pin, INPUT);
    _state = calcState();
    _lastState = _state;
    _time = millis();
    _lastTime = _time;
    _changeTime = _time;
}

inline void LightSensor::setThreshold(uint32_t threshold)
{
    _threshold = threshold;
}

// センサデータの更新
inline void LightSensor::update()
{   
    _time = millis();

    // 最後に値を計測した時間から_dbTimeミリ秒経過していない
    if (_time - _lastTime < _dbTime) {
        return;
    }

    // 最後に値を計測した時間から_dbTimeミリ秒経過している
    // 現在の値を更新
    _lastState = _state;
    _state = calcState();

    _lastTime = _time;

    if (_state != _lastState) {
        // 値に変化があった場合
        _changeTime = _time;
        _changed = 1;
        return;
    }

    // 値に変化がなかった場合
    _changed = 0;
}

// pinValをルクス値に変換
inline double LightSensor::pinValueToLux(int pinVal)
{
    double vcc = 3.3; // 最大電圧(V)
    double refResistanse = 10.0; // 分圧回路の抵抗部の抵抗値(kOhm)

    double volt = pinVal * vcc / 4096;

    // 照度センサ部分の抵抗値(kOhm)
    double outResistanse = (vcc - volt) * refResistanse / volt;

    // ルクス値を計算
    double lux = std::pow(outResistanse / 65.148, -1.429);

    return lux;
}

inline uint8_t LightSensor::calcState()
{
    uint32_t sum = 0;
    int count = 5;
    for (int i = 0; i < count; i++) {
        uint16_t pinVal = analogRead(_pin); // 0 - 4095
        sum += pinVal;
        vTaskDelay(5 / portTICK_RATE_MS);
    }

    uint32_t avgPinVal = sum / count;
    _lux = pinValueToLux(avgPinVal);

    return _lux > _threshold ? 1 : 0;
}

inline double LightSensor::getLux()
{
    return _lux;
}

inline uint8_t LightSensor::getState()
{
    return _state;
}

inline uint8_t LightSensor::hasPassedSinceSameState(uint32_t time)
{
    return (_time - _changeTime > time) ? 1 : 0; 
}
