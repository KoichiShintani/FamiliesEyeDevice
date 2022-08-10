#pragma once
#include <BLEDevice.h>
#include <BLE2902.h>
#include <BluetoothSerial.h>
#include <esp_bt.h>
#include "esp32io.h"

// #include <esp_spp_api.h> 認証用

/**
 * The power level can be one of:
 *  ESP_PWR_LVL_N12 0 弱い
 *  ESP_PWR_LVL_N9  1
 *  ESP_PWR_LVL_N6  2
 *  ESP_PWR_LVL_N3  3
 *  ESP_PWR_LVL_N0  4
 *  ESP_PWR_LVL_P3  5 デフォルト
 *  ESP_PWR_LVL_P6  6
 *  ESP_PWR_LVL_P9  7 強い
 * 
 * 以下は後方互換のため存在
 *  ESP_PWR_LVL_N14  N12
 *  ESP_PWR_LVL_N11  N9
 *  ESP_PWR_LVL_N8 	 N6
 *  ESP_PWR_LVL_N5 	 N3
 *  ESP_PWR_LVL_N2 	 N0
 *  ESP_PWR_LVL_P1 	 P3
 *  ESP_PWR_LVL_P4 	 P6
 *  ESP_PWR_LVL_P7 	 P9
 */
#define TX_POWER ESP_PWR_LVL_N12

// スマートフォンアプリとのインターフェースとしての機能を持つ
//
namespace apif
{
    void init();
    void startBLEService();
    void setFallDown();
    void setSos();
    
    uint8_t camera();
    uint8_t goOut();
    uint8_t help();
    uint8_t safety();
    uint8_t approach();
    uint8_t dangerous();
    uint8_t sos();
    std::string TimeValue();
    
    
    static const std::string DEVICE_NAME = "HAKUJO_DEVICE";
    BLEServer *pServer;
    BLEService *pService;
    bool deviceConnected = false;
    
    BLECharacteristic *fallDownCharacteristic 		= NULL;
    BLECharacteristic *sosCharacteristic 		= NULL;
    BLECharacteristic *cameraCharacteristic 		= NULL;
    BLECharacteristic *goOutCharacteristic 		= NULL;
    BLECharacteristic *helpCharacteristic 		= NULL;
    BLECharacteristic *safetyCharacteristic 		= NULL;
    BLECharacteristic *approachCharacteristic 		= NULL;
    BLECharacteristic *dangerousCharacteristic 		= NULL;
    BLECharacteristic *sosFromAppCharacteristic 	= NULL;
    BLECharacteristic *timeValueCharacteristic 		= NULL;

    uint8_t camera_state;
    uint8_t go_out_state;
    uint8_t help_state;
    uint8_t safety_state;
    uint8_t approach_state;
    uint8_t dangerous_state;
    uint8_t sos_state;
    std::string time_value;//yyyymmddhhmmss
    
    
    class MyServerCallbacks : public BLEServerCallbacks
    {
        void onConnect(BLEServer *pServer)
        {
            Serial.println("Connect.");
            deviceConnected = true;
        }
        void onDisconnect(BLEServer *pServer)
        {
            Serial.println("Disconnect.");
            deviceConnected = false;
        }
    };

    class FallDownCallbacks : public BLECharacteristicCallbacks
    {
        void onRead(BLECharacteristic *pCharacteristic)
        {
            Serial.println("Fall Down Read value.");
        }
    };
    class SosCallbacks : public BLECharacteristicCallbacks
    {
        void onRead(BLECharacteristic *pCharacteristic)
        {
            Serial.println("SOS Read value.");
        }
    };
    class CameraCallbacks : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *characteristic)
        {
            Serial.println("CAMERA write ");
            std::string value = characteristic->getValue();
            if (value.length() > 0) {
                 camera_state = (uint8_t)value[0];
                 Serial.println(camera_state);
            }
        }
    };
    class GoOutCallbacks : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *characteristic)
        {
            Serial.println("GO OUT write ");
            std::string value = characteristic->getValue();
            if (value.length() > 0) {
                 go_out_state = (uint8_t)value[0];
                 Serial.println(go_out_state);
            }
        }
    };
    class HelpCallbacks : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *characteristic)
        {
            Serial.println("Help write ");
            std::string value = characteristic->getValue();
            if (value.length() > 0) {
                 help_state = (uint8_t)value[0];
                 Serial.println(help_state);
            }
        }
    };
    class SafetyCallbacks : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *characteristic)
        {
            Serial.println("Safety write ");
            std::string value = characteristic->getValue();
            if (value.length() > 0) {
                 safety_state = (uint8_t)value[0];
                 Serial.println(safety_state);
            }
        }
    };
    class ApproachCallbacks : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *characteristic)
        {
            Serial.println("Approach write ");
            std::string value = characteristic->getValue();
            if (value.length() > 0) {
                 approach_state = (uint8_t)value[0];
                 Serial.println(approach_state);
            }
        }
    };
    class DangerousCallbacks : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *characteristic)
        {
            Serial.println("Dangerous write ");
            std::string value = characteristic->getValue();
            if (value.length() > 0) {
                 dangerous_state = (uint8_t)value[0];
                 Serial.println(dangerous_state);
            }
        }
    };
    class SosFromAppCallbacks : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *characteristic)
        {
            Serial.println("Sos write ");
            std::string value = characteristic->getValue();
            if (value.length() > 0) {
                 sos_state = (uint8_t)value[0];
                 Serial.println(sos_state);
            }
        }
    };
    class TimeValueCallbacks : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *characteristic)
        {
            Serial.println("TimeValue write ");
            std::string value = characteristic->getValue();
            if (value.length() == 14) {
		 time_value = value;
		 for (int i = 0; i < value.length(); i++) Serial.print(time_value[i]);
                 Serial.println();
            }
        }
    };
    /**
     * BLE初期化処理 
     */
    void init()
    {
	Serial.println("init BLE ");
        BLEDevice::init(DEVICE_NAME);
        esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, TX_POWER);

        pServer = BLEDevice::createServer();
        pServer->setCallbacks(new MyServerCallbacks());
        pService = pServer->createService(BLEUUID(BLE_SERVICE_UUID), 20, 0);
        
        //// デバイス→アプリ 通知
        fallDownCharacteristic = pService->createCharacteristic(
            FALL_DOWN_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
        fallDownCharacteristic->setCallbacks(new FallDownCallbacks());
        fallDownCharacteristic->addDescriptor(new BLE2902());
        
        sosCharacteristic = pService->createCharacteristic(
            SOS_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
        sosCharacteristic->setCallbacks(new SosCallbacks());
        sosCharacteristic->addDescriptor(new BLE2902());
        
        //// アプリ→デバイス 通知
        cameraCharacteristic = pService->createCharacteristic(
            CAMERA_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
        cameraCharacteristic->setCallbacks(new CameraCallbacks());
        
        goOutCharacteristic = pService->createCharacteristic(
            GO_OUT_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
        goOutCharacteristic->setCallbacks(new GoOutCallbacks());
        
        helpCharacteristic = pService->createCharacteristic(
            HELP_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
        helpCharacteristic->setCallbacks(new HelpCallbacks());
        
        safetyCharacteristic = pService->createCharacteristic(
            SAFETY_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
        safetyCharacteristic->setCallbacks(new SafetyCallbacks());

#if 0//いったん封印
        approachCharacteristic = pService->createCharacteristic(
            APPROACH_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
        approachCharacteristic->setCallbacks(new ApproachCallbacks());
        
        dangerousCharacteristic = pService->createCharacteristic(
            DANGEROUS_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
        dangerousCharacteristic->setCallbacks(new DangerousCallbacks());
#endif

        timeValueCharacteristic = pService->createCharacteristic(
            TIME_VALUE_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
        timeValueCharacteristic->setCallbacks(new TimeValueCallbacks());
        
        sosFromAppCharacteristic = pService->createCharacteristic(
            SOS_FROM_APP_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
        sosFromAppCharacteristic->setCallbacks(new SosFromAppCallbacks());
        Serial.println("init BLE F");
    }

    void startBLEService()
    {
        pService->start();
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
	pAdvertising->start();
    }

    // 転倒通知
    void setFallDown()
    {
        Serial.println("fallDown Notice");
        uint8_t val = 0x01;
        size_t len = 1;
        fallDownCharacteristic->setValue(&val, len);
        fallDownCharacteristic->notify();
    }
    // SOS通知
    void setSos()
    {
        Serial.println("SOS Notice");
        uint8_t val = 0x01;
        size_t len = 1;
        sosCharacteristic->setValue(&val, len);
        sosCharacteristic->notify();
    }
    
    uint8_t camera(void)       {return   camera_state;}
    uint8_t goOut(void)        {return   go_out_state;}
    uint8_t help(void)         {return   help_state;}
    uint8_t safety(void)       {return   safety_state;}
    uint8_t approach(void)     {return   approach_state;}
    uint8_t dangerous(void)    {return   dangerous_state;}
    uint8_t sos(void)          {return   sos_state;}
    std::string TimeValue(void){return   time_value;}
} // namespace apif
