#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

struct Config
{
  char MODE_WIFI[64];
  char SSID_CONFIG[64];
  char PASS_CONFIG[64];
  char SSID_AP_CONFIG[64];
  char PASS_AP_CONFIG[64];
  int PORT_TCP;
};

const char *filename = "/config.json";
Config config;

void loadConfiguration(const char *filename, Config &config)
{

  Serial.println(F("Loading configuration..."));

  File file = SPIFFS.open(filename);
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  strlcpy(config.MODE_WIFI,
          doc["mode_wifi"] | "STA",
          sizeof(config.MODE_WIFI));

  strlcpy(config.SSID_CONFIG,
          doc["ssid"] | "indconf2002",
          sizeof(config.SSID_CONFIG));

  strlcpy(config.PASS_CONFIG,
          doc["pass"] | "07031968200703196820123456",
          sizeof(config.PASS_CONFIG));

  strlcpy(config.SSID_AP_CONFIG,
          doc["ssid_ap"] | "Korm TCP Server",
          sizeof(config.SSID_AP_CONFIG));

  strlcpy(config.PASS_AP_CONFIG,
          doc["pass_ap"] | "12345678",
          sizeof(config.PASS_CONFIG));

  config.PORT_TCP = doc["port_tcp"] | 4001;

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

  doc["ssid"] = "Padavan 2.4";
  doc["pass"] = config.PASS_CONFIG;
  doc["ssid_ap"] = config.SSID_AP_CONFIG;
  doc["pass_ap"] = config.PASS_AP_CONFIG;
  doc["port_tcp"] = 4005;

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