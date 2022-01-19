#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

struct Config
{
  String _serial_config, _serial_baund, _mode_wifi, _client_tcp, _wifi_rssi, _ip_addr, _mask_addr, _gataway_addr, _free_ram, _ssid, _pass, _ssid_ap, _pass_ap, _port_tcp;
};

const char *filename = "/config.json";
Config config;

void loadConfiguration(const char *filename, Config &config)
{
  while (!SPIFFS.begin())
  {
    Serial.println(F("Failed to initialize SD library"));
    delay(1000);
  }
  // Open file for reading
  File file = SPIFFS.open(filename);

  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<512> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.parseObject(file);

  if (!root.success())
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonObject to the Config
  config._serial_config = root["serial_config"] | "SERIAL_8E1";
  config._serial_baund = root["serial_baund"] | "9600";
  config._mode_wifi = root["mode_wifi"] | "STA";
  config._ssid = root["ssid"] | "Padavan 2.4";
  config._pass = root["pass"] | "46684668";
  config._ssid_ap = root["ssid_ap"] | "Korm TCP Server";
  config._pass_ap = root["pass_ap"] | "46684668";
  config._port_tcp = root["port_tcp"] | "4001";

  // Close the file (File's destructor doesn't close the file)
  file.close();
}

void saveConfiguration(const char *filename, const Config &config)
{
  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(filename);

  // Open file for writing
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file)
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate the memory pool on the stack
  // Don't forget to change the capacity to match your JSON document.
  // Use https://arduinojson.org/assistant/ to compute the capacity.
  StaticJsonBuffer<256> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.createObject();

  // Set the values
  root["serial_config"] = config._serial_config;
  root["serial_baund"] = config._serial_baund;
  root["mode_wifi"] = config._mode_wifi;
  root["client_tcp"] = config._client_tcp;
  root["wifi_rssi"] = config._wifi_rssi;
  root["ip_addr"] = config._ip_addr;
  root["mask_addr"] = config._mask_addr;
  root["gataway_addr"] = config._gataway_addr;
  root["free_ram"] = config._free_ram;
  root["ssid"] = config._ssid;
  root["pass"] = config._pass;
  root["ssid_ap"] = config._ssid_ap;
  root["pass_ap"] = config._pass_ap;
  root["port_tcp"] = config._port_tcp;

  // Serialize JSON to file
  if (root.printTo(file) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file (File's destructor doesn't close the file)
  file.close();
}

void printFile(const char *filename)
{

  File file = SPIFFS.open(filename);
  if (!file)
  {
    Serial.println(F("Failed to read file"));
    return;
  }

  while (file.available())
  {
    Serial.print((char)file.read());
  }
  Serial.println();
  file.close();
}

void btn_default()
{
  if (digitalRead(0))
    return;

  unsigned long t = millis();

  while (!digitalRead(0))
  {
    delay(1);
    if (t + 15000 < millis())
    {
      Serial.println("Reset ESP32");
      Serial.println("Read default config");
      SPIFFS.remove("/config.json");
      ESP.restart();
    }
  }
}
