#pragma once

// ==== PIN ====
#define GPIO_PIN_LIGHT_SENSOR 14 // 照度センサ
#define GPIO_PIN_WHITE_LED    -1 // 白色LED
#define GPIO_PIN_VIBE_MOTOR   13 // 振動モータ
#define GPIO_PIN_BUTTON       23 // ボタン
#define GPIO_PIN_BUZZER       25
#define GPIO_PIN_LED_R  16
#define GPIO_PIN_LED_G  17
#define GPIO_PIN_LED_B  18

// ==== ブザー ====
#define LEDC_CH_BUZZER 0

// ==== 9軸センサ ==== 
#define IMU_MPU6050 1
#define IMU_TYPE IMU_MPU6050
#define PORT_SDA 21
#define PORT_SCL 22


// ==== BLE ====
#define BLE_SERVICE_UUID "6b15c601-e705-4470-8570-d8fef29552b2"

// デバイス→アプリ通知
#define FALL_DOWN_CHARACTERISTIC_UUID 		"50ef87b6-2e1d-482e-ad7b-ef9aa8d7f7fd"//転倒状態
#define SOS_CHARACTERISTIC_UUID			"ed87309f-7835-4a92-95a0-79b74f69532c"//SOS状態

//アプリ→デバイス通知
#define CAMERA_CHARACTERISTIC_UUID              "5469b9c6-ac52-435d-9dd6-292cb6e95c71"//カメラ画像取得状態遷移
#define GO_OUT_CHARACTERISTIC_UUID		"2a2e67f7-58e6-44c3-a7fa-144979e60bcb"//お出かけ状態遷移
#define HELP_CHARACTERISTIC_UUID		"29a202ee-75c7-4e95-a9de-73bd7c54f60f"//困り状態遷移
#define SAFETY_CHARACTERISTIC_UUID        	"d3e818a5-8317-459e-9f3d-c61617eea769"//安全確認通知
#define APPROACH_CHARACTERISTIC_UUID  		"5b271fcb-7926-40c1-be82-dbcd55572cc2"//接近デバイス
#define DANGEROUS_CHARACTERISTIC_UUID           "29df2990-8f85-44b8-914a-7a65ad821f38"//危険ポイント
#define TIME_VALUE_CHARACTERISTIC_UUID          "2b6e0c77-aa69-475d-bfd8-c40a3ad9a6eb"//現在時刻設定
#define SOS_FROM_APP_CHARACTERISTIC_UUID        "d4eef61a-db5e-48f6-9551-fe86b33f2965"//SOS状態通知

