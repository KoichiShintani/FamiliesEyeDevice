#pragma once
#include <cmath>
#include "Arduino.h"
#include "../lib/TinyMPU6050.h"

// 検知方法は暫定
// TODO: 加速度だけでなく角速度も考慮した判定をしたい
namespace service_fall_detection
{

    int initService();
    float calcSensorData(float x, float y, float z);
    void executeService();

    const float THRESHOLD = 0.8f;     // センサー閾値（暫定）
    const int MAX_COUNT = 20;

    int overCount = 0;
    int underCount= 0;
    int dataCount = 0;
    int maxDataCount = 20;
    float accXData[20] = {0.0f};
    float accYData[20] = {0.0f};
    float accZData[20] = {0.0f};
    float resultData[20] = {0.0f};
    uint32_t prevSendTime = 0;
    
    int state;
    
    MPU6050 mpu (Wire);                   // 9軸センサー制御用,オブジェクト
    int initService()
    {
        mpu.Initialize(PORT_SDA, PORT_SCL);
        //mpu.Calibrate();
        return 0;
    }

    float calcSensorData(float x, float y, float z)
    {
        // y軸とz軸方向で加速度ベクトルの長さを計算
        return std::pow(std::pow(x, 2) + std::pow(z, 2), 0.5);
    }
    int getState(){
        return state;
    }
    void executeService()
    {
        float accX = 0.0F;
        float accY = 0.0F;
        float accZ = 0.0F;

        float gyroX = 0.0F;
        float gyroY = 0.0F;
        float gyroZ = 0.0F;

        float magX = 0.0F;
        float magY = 0.0F;
        float magZ = 0.0F;

        // データの取得
        // MPU-6050用
        mpu.Execute();
        accX = mpu.GetAccX()*8;//16Gスケールで値をとると、2Gスケール設定での1/8の値になるのでここで合わせ込む。
        accY = mpu.GetAccY()*8;
        accZ = mpu.GetAccZ()*8;
        //printf("[%f %f %f]\n",accX,accY,accZ);
        
        accXData[dataCount] = accX;
        accYData[dataCount] = accY;
        accZData[dataCount] = accZ;

        dataCount++;
        if (dataCount == maxDataCount)
        {
            dataCount = 0;
        }

        // 直前20回分のデータを計算
        for (int i = 0; i < maxDataCount; i++)
        {
            resultData[i] = calcSensorData(accXData[i], accYData[i], accZData[i]);
            //printf("%f\n",resultData[i]);
        }

        // maxDataCount回の平均をとる
        float total = 0.0f;
        for (int i = 0; i < maxDataCount; i++)
        {
            total += resultData[i];
        }
        float sensorAvg = total / maxDataCount;
        //printf("(%f)\n",sensorAvg);
        
	    if (sensorAvg > THRESHOLD)
	    {
	        overCount++;
	    }
	    else{
		    underCount++;
		}

	    //printf("(%d) (%d)\n",overCount,underCount);
	    if (overCount == MAX_COUNT)
	    {
			state = 1;
            //Serial.println("Abnormal state!");
	        overCount = 0;
	        underCount = 0;
	    }
	    if(underCount == MAX_COUNT)
	    {
			state = 0;
            //Serial.println("Normal state!");
	        overCount = 0;
	        underCount = 0;
	    }
    }

}
