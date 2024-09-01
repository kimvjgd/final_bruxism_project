// #include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEClient.h>
// #include <BLEScan.h>
// #include <BLEAdvertisedDevice.h>

// #define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"  // BLE 서버의 UUID
// #define CHARACTERISTIC_UUID "87654321-4321-4321-4321-cba987654321"  // BLE 특성 UUID

// BLERemoteCharacteristic* pRemoteCharacteristic;
// BLEAdvertisedDevice* myDevice;
// bool doConnect = false;
// bool connected = false;
// BLEScan* pBLEScan;

// class MyClientCallback : public BLEClientCallbacks {
//   void onConnect(BLEClient* pClient) {
//     Serial.println("Connected to BLE server.");
//   }

//   void onDisconnect(BLEClient* pClient) {
//     connected = false;
//     Serial.println("Disconnected from BLE server.");
//   }
// };

// void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
//   String receivedData = String((char*)pData);
//   Serial.print("Received data: ");
//   Serial.println(receivedData);

//   int commaIndex = receivedData.indexOf(',');
//   if (commaIndex > 0) {
//     String value1 = receivedData.substring(0, commaIndex);
//     String value2 = receivedData.substring(commaIndex + 1);
//     Serial.print("Potentiometer 1 Value: ");
//     Serial.println(value1);
//     Serial.print("Potentiometer 2 Value: ");
//     Serial.println(value2);
//   }
// }

// class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
//   void onResult(BLEAdvertisedDevice advertisedDevice) {
//     if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
//       Serial.print("Found BLE server: ");
//       Serial.println(advertisedDevice.toString().c_str());
//       myDevice = new BLEAdvertisedDevice(advertisedDevice);
//       doConnect = true;
//       pBLEScan->stop();
//     }
//   }
// };

// void setup() {
//   Serial.begin(115200);
//   Serial.println("Starting BLE Client...");

//   BLEDevice::init("");
//   pBLEScan = BLEDevice::getScan();
//   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//   pBLEScan->setInterval(200);
//   pBLEScan->setWindow(199);
//   pBLEScan->setActiveScan(true);
//   pBLEScan->start(30, false);  // 30초 동안 스캔

//   Serial.println("Scanning for BLE devices...");
// }

// void loop() {
//   if (doConnect) {
//     BLEClient* pClient = BLEDevice::createClient();
//     pClient->setClientCallbacks(new MyClientCallback());

//     if (pClient->connect(myDevice)) {
//       Serial.println("Connected to server");

//       BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
//       if (pRemoteService == nullptr) {
//         Serial.print("Failed to find service UUID: ");
//         Serial.println(SERVICE_UUID);
//         pClient->disconnect();
//         return;
//       }

//       pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
//       if (pRemoteCharacteristic == nullptr) {
//         Serial.print("Failed to find characteristic UUID: ");
//         Serial.println(CHARACTERISTIC_UUID);
//         pClient->disconnect();
//         return;
//       }

//       if (pRemoteCharacteristic->canNotify()) {
//         pRemoteCharacteristic->registerForNotify(notifyCallback);
//         Serial.println("Notification registered");
//       }

//       connected = true;
//     } else {
//       Serial.println("Failed to connect to server");
//     }

//     doConnect = false;
//   }

//   delay(1000);
// }


/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include "BLEDevice.h"
//#include "BLEScan.h"

// #define BLEUUID        "12345678-1234-1234-1234-123456789abc"  // BLE 서버의 UUID
// #define BLEUUID "87654321-4321-4321-4321-cba987654321"  // BLE 특성 UUID
// // The remote service we wish to connect to.
static BLEUUID serviceUUID("12345678-1234-1234-1234-123456789abc");
// // The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("87654321-4321-4321-4321-cba987654321");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    /*
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    */
    Serial.print("Receved data: ");
    Serial.println((char*)pData);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    /*
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }
    */

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  /*
  if (connected) {
    String newValue = "Time since boot: " + String(millis()/1000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
    //pRemoteCharacteristic->writeValue("5678");
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  */
  
  delay(1000); // Delay a second between loops.
} // End of loop