# ESPHome BLE Server Component

This is a Custom component for ESPHome that allows your device to act as a BLE GATT server.
By itself, this component only registers the device name and firmware version number.
It allows other components (or code) to register their own services and characteristics.

## Installation

Installation can be via [HACS](https://hacs.xyz) or by downloading and placing the 
[ble_server](custom_components/ble_server) folder inside your **esphome** custom_components folder.

## Configuration

```yaml
ble_server:
```

## Notes

- This component cannot be used with [esp32_ble_tracker](https://esphome.io/components/esp32_ble_tracker) as it
  conflicts with client/server mode.
