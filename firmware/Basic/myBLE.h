#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID              "fb1e4001-54ae-4a28-9f74-dfccb248601d"
#define CHARACTERISTIC_UUID_RX    "fb1e4002-54ae-4a28-9f74-dfccb248601d"
#define CHARACTERISTIC_UUID_TX    "fb1e4003-54ae-4a28-9f74-dfccb248601d"

static BLECharacteristic* pCharacteristicTX;
static BLECharacteristic* pCharacteristicRX;

static bool deviceConnected = false;

void BLE_Send_Msg(char* msg)
{
    pCharacteristicTX->setValue(msg);
    pCharacteristicTX->notify();
}

void initBLE();
class MyServerCallbacks: public BLEServerCallbacks 
{
    void onConnect(BLEServer* pServer) 
    {
        deviceConnected = true;
        // Serial.print(curTilt);
        // Serial.print(", ");
        // Serial.print(curRoll);
        // Serial.print(", ");
        // Serial.println("Connected");
    }
    void onDisconnect(BLEServer* pServer) 
    {
        deviceConnected = false;
        // Serial.print(curTilt);
        // Serial.print(", ");
        // Serial.print(curRoll);
        // Serial.print(", ");
        // Serial.println("Disconnected");
        BLEDevice::startAdvertising();
    }
};



#define DELIMITER ","
//#define CMD_BUF_SIZE      256
//#define PARAM_BUF_SIZE    256
//#define PARAM_CMD

class MyCallbacks: public BLECharacteristicCallbacks 
{
    void onNotify(BLECharacteristic *pCharacteristic) 
    {
        uint8_t* pData;
        std::string value = pCharacteristic->getValue();
        int len = value.length();
        pData = pCharacteristic->getData();
        if (pData != NULL) 
        {
            Serial.print("TX : ");
            Serial.println((char*)pData);            
        }
    }

    void onWrite(BLECharacteristic *pCharacteristic) 
    {
        char* token;
        char* cmd;
        char* param;
        String svalue;
        
        uint8_t* pData;
        std::string value = pCharacteristic->getValue();
        int len = value.length();
        pData = pCharacteristic->getData();

        if (pData != NULL) 
        {
        }
    }
};

// extern int WEARABLE_DEVICE_NUM;
void initBLE()
{
    char data[256];
    // String str = "WEARABLE" + String(WEARABLE_DEVICE_NUM);
    String str = "Dongdong";
    str.toCharArray(data, str.length() + 1);

    BLEDevice::init(data);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristicTX = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY );
    pCharacteristicRX = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
    pCharacteristicRX->setCallbacks(new MyCallbacks());
    pCharacteristicTX->setCallbacks(new MyCallbacks());
    pCharacteristicTX->addDescriptor(new BLE2902());
    pCharacteristicTX->setNotifyProperty(true);
    pService->start();

    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
  
    Serial.println("Characteristic defined! Now you can read it in your phone!");
}

/******************************************************************/
/****************             Serial         ****************/
/******************************************************************/

#define STX   '?'
#define ETX1  0x0D
#define ETX2  0x0A

int com_state = 0;
int com_size = 0;
char com_read[256];
char com_cmd[256];
char com_para[256];

extern int led_state;

#define PCMD_FWV        "FWV"
#define PCMD_LED        "LED"

#define FWV   "FWV,Solar Louver Blind V0.1"

int para_num = 0;
void ExcuteCommand(char* cmd, char* para)
{
    if(strcmp((const char*)cmd, PCMD_FWV) == 0)
    {
        Serial.println(FWV);
    }
    else if(strcmp((const char*)cmd, PCMD_LED) == 0)
    {
        if(para == "")
        {
            Serial.println("Input Para");
            return;
        }
        para_num = atoi(para);
        led_state = para_num;
        Serial.print(PCMD_LED);
        Serial.print(",");
        Serial.println(led_state);
    }
//    else if(strcmp((const char*)cmd, PCMD_FAN) == 0)
//    {
//        if(strcmp((const char*)para, PARA_ON) == 0)
//        {
//            fan_state = true;
//            Serial.println("FAN ON");
//        }
//        else if(strcmp((const char*)para, PARA_OFF) == 0)
//        {
//            fan_state = false;
//            Serial.println("FAN OFF");
//        }
//    }    
}

bool PacketAnalysis(char* read)
{
    char* token;

    for(int i = 0; i < 256; i++)
    {
        com_para[i] = 0;
    }
    
    token = strtok((char*)read, DELIMITER);

    if(token != NULL)
    {
        if(strlen(token) < 256)
        {
            sprintf((char*)com_cmd, "%s", token);
        }
        else
        {
            
        }

        token = strtok(NULL, "");
        if(token != NULL)
        {
            sprintf((char*)com_para, "%s", token);
        }
        return true;
    }
    return false;
}
