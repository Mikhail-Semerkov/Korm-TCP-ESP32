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

  File file = SPIFFS.open(filename);
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  config._serial_config = doc["serial_config"] | "SERIAL_7E1";
  config._serial_baund = doc["serial_baund"] | "9600";
  config._mode_wifi = doc["mode_wifi"] | "STA";
  config._ssid = doc["ssid"] | "indconf2002";
  config._pass = doc["pass"] | "07031968200703196820123456";
  config._ssid_ap = doc["ssid_ap"] | "Korm TCP Server";
  config._pass_ap = doc["pass_ap"] | "12345678";
  config._port_tcp = doc["port_tcp"] | "4001";

  file.close();
}

void saveConfiguration(const char *filename, const Config &config)
{
  SPIFFS.remove(filename);
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file)
  {
    Serial.println(F("Failed to create file"));
    return;
  }
  StaticJsonDocument<256> doc;

  doc["serial_config"] = config._serial_config;
  doc["serial_baund"] = config._serial_baund;
  doc["mode_wifi"] = config._mode_wifi;
  doc["ssid"] = config._ssid;
  doc["pass"] = config._pass_ap;
  doc["ssid_ap"] = config._ssid_ap;
  doc["pass_ap"] = config._pass_ap;
  doc["port_tcp"] = config._port_tcp;

  if (serializeJson(doc, file) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }
  file.close();
  Serial.println("Save Ok");
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
