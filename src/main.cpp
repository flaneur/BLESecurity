/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include "esp_bt_device.h"
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUID1 "beb5483e-36e1-4688-b7f5-ea07361b26a9"

class MySecurity : public BLESecurityCallbacks
{

  bool onConfirmPIN(uint32_t pin)
  {
    Serial.println("onConfirmPIN");
    return false;
  }

  uint32_t onPassKeyRequest()
  {
    ESP_LOGI(LOG_TAG, "PassKeyRequest");
    Serial.println("onPassKeyRequest");

    return 123456;
  }

  void onPassKeyNotify(uint32_t pass_key)
  {
    ESP_LOGI(LOG_TAG, "On passkey Notify number:%d", pass_key);
    Serial.println("onPassKeyNotify");
    Serial.println("pass_key");
    Serial.println(pass_key);
  }

  bool onSecurityRequest()
  {
    ESP_LOGI(LOG_TAG, "On Security Request");
    Serial.println("onSecurityRequest");
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl)
  {
    ESP_LOGI(LOG_TAG, "Starting BLE work!");
    Serial.println("onAuthenticationComplete");

    std::cout << "Peer device address type:" << std::endl;
    std::cout << cmpl.addr_type << std::endl;
    std::cout << "---------------------" << std::endl;

    std::cout << "Authentication mode:" << std::endl;
    std::cout << cmpl.auth_mode << std::endl;
    std::cout << "---------------------" << std::endl;

    std::cout << "BD address peer device:" << std::endl;
    std::cout << cmpl.bd_addr << std::endl;
    std::cout << "---------------------" << std::endl;

    std::cout << "Device type:" << std::endl;
    std::cout << cmpl.dev_type << std::endl;
    std::cout << "---------------------" << std::endl;

    std::cout << "The HCI reason/error code for when success=FALSE:" << std::endl;
    std::cout << cmpl.fail_reason << std::endl;
    std::cout << "---------------------" << std::endl;

    std::cout << "Link key associated with peer device:" << std::endl;
    std::cout << cmpl.key << std::endl;
    std::cout << "---------------------" << std::endl;

    std::cout << "Valid link key value in key element:" << std::endl;
    std::cout << cmpl.key_present << std::endl;
    std::cout << "---------------------" << std::endl;

    std::cout << "The type of Link Key:" << std::endl;
    std::cout << cmpl.key_type << std::endl;
    std::cout << "---------------------" << std::endl;

    std::cout << "Success State:" << std::endl;
    std::cout << cmpl.success << std::endl;
    std::cout << "---------------------" << std::endl;

    if (cmpl.success)
    {
      uint16_t length;
      Serial.println("onAuthenticationComplete:Success");
      esp_ble_gap_get_whitelist_size(&length);
      ESP_LOGD(LOG_TAG, "size: %d", length);
    }
  }
};
void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("ESP32");
  /*
     Required in authentication process to provide displaying and/or input passkey or yes/no butttons confirmation
  */

  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  BLECharacteristic *pCharacteristic1 = pService->createCharacteristic(
      CHARACTERISTIC_UUID1,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  pCharacteristic1->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

  pCharacteristic->setValue("Hello World says Neil");
  pCharacteristic1->setValue("Hello World says Neil1");
  class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
  {
    void onRead(BLECharacteristic *pCharacteristic)
    {
      Serial.print("New Values Has Been Read from Characteristic: ");
      // Serial.println( pCharacteristic->getUUID().toString());

      std::string rxValue = pCharacteristic->getValue();
      // Serial.println(rxValue);
    };
    void onWrite(BLECharacteristic *pCharacteristic)
    {
      std::string writtenCharacteristic = pCharacteristic->getUUID().toString();

      Serial.println("New Value Has Been Written From Different Characteristic");
      std::string rxValue = pCharacteristic->getValue();
      // Serial.println(rxValue);
    }
  };

  class MyServerCallbacks : public BLEServerCallbacks
  {
    void onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param)
    {
      Serial.println("Conenction Made");
      // std::cout << "Connected Device ID:";
      // std::cout << pServer->getConnId() << std::endl;
      // int a = param->read.bda[0];
      // std::cout<<a<<std::endl;
      // for(int i =1;i<6;i++){
      //   a=param->read.bda[i];
      //   std::cout << a<< std::endl;
      // }
      // std::cout<<param->read.bda[0] <<param->read.bda[1] << param->read.bda[2]<<param->read.bda[3]<<param->read.bda[4]<<param->read.bda[5]<<std::endl;
      // const uint8_t *point = esp_bt_dev_get_address();

      // for (int i = 0; i < 6; i++)
      // {

      //   char str[3];

      //   sprintf(str, "%02X", (int)point[i]);
      //   Serial.print(str);

      //   if (i < 5)
      //   {
      //     Serial.print(":");
      //   }

      // } 
      /*pMyDevice->deviceConnected = true;
            initialConnection = true;
            std::cout<<"Initial Connection Made"<<std::endl;*/
      // delay(5000);
      // for(int i =0;i<12;i++){
      // notifyUpdatedConfigurationCharacteristic(i,true);
      // delay(2000);
      // }
    };

    void onDisconnect(BLEServer *pServer)
    {
      /*pMyDevice->deviceConnected = false;
        initialConnection = false;*/
      Serial.println("Started Advertising");
      pServer->startAdvertising(); // restart advertising
    }
  };

  MyServerCallbacks *ServerCallBacks = new MyServerCallbacks();
  MyCharacteristicCallbacks *CharactericticCallBacks = new MyCharacteristicCallbacks();
  pServer->setCallbacks(ServerCallBacks);
  pCharacteristic1->setCallbacks(CharactericticCallBacks);
  pCharacteristic->setCallbacks(CharactericticCallBacks);

  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  // pAdvertising->setScanFilter(true,true);
  // pAdvertising->handleGAPEvent();
  // pAdvertising->setAdvertisementType();
  // pAdvertising->setDeviceAddress();
  // pAdvertising->setPrivateAddress();
  // pAdvertising->
  pAdvertising->start();

  BLESecurity *pSecurity = new BLESecurity();
  BLEDevice::setSecurityCallbacks(new MySecurity());

  pSecurity->setStaticPIN(123456);

  // set static passkey
  Serial.println("Characteristic defined");
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(2000);
}
