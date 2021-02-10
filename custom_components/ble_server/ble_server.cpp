#include "ble_server.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/core/version.h"

#ifdef ARDUINO_ARCH_ESP32

namespace esphome {
namespace esp32_ble_server {

static const char *TAG = "esp32_ble_server";

static const char *SERVICE_UUID  = "03774663-d5d6-49e8-8d38-377466300000";
static const char *VERSION_UUID  = "03774663-d5d6-49e8-8d38-377466300001";

ESP32BLEServer::ESP32BLEServer() { global_ble_server = this; }

void ESP32BLEServer::setup() {
  BLEDevice::init(App.get_name());
  ESP_LOGD(TAG, "Set BLE MTU result = %d", BLEDevice::setMTU(256));
  ESP_LOGD(TAG, "Get BLE MTU = %d", BLEDevice::getMTU());
  this->server_ = BLEDevice::createServer();
  this->server_->setCallbacks(new ESP32BLEServerCallback());

  BLEService *service = this->server_->createService(SERVICE_UUID);

  BLECharacteristic *version = service->createCharacteristic(
                                  VERSION_UUID,
                                  BLECharacteristic::PROPERTY_READ
                                );
  version->setValue(ESPHOME_VERSION);

  BLEDescriptor *version_descriptor = new BLE2902();
  version_descriptor->setValue("ESPHome Version");
  version->addDescriptor(version_descriptor);

  service->start();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);
  advertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  advertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void ESP32BLEServer::loop() {

}

BLEService* ESP32BLEServer::add_service(const char* uuid) {
  ESP_LOGD(TAG, "Adding new BLE service");
  BLEService *service = this->server_->createService(uuid);

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(uuid);

  return service;
}

float ESP32BLEServer::get_setup_priority() const { return setup_priority::HARDWARE; }

void ESP32BLEServer::dump_config() { ESP_LOGCONFIG(TAG, "ESP32 BLE Server:"); }

void ESP32BLEServerCallback::onConnect(BLEServer* pServer) {
  ESP_LOGD(TAG, "BLE Client connected");
}

void ESP32BLEServerCallback::onDisconnect(BLEServer* pServer) {
  ESP_LOGD(TAG, "BLE Client disconnected");
}

ESP32BLEServer *global_ble_server = nullptr;

}  // namespace esp32_ble_server
}  // namespace esphome

#endif
