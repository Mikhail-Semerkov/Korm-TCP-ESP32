void loadConfiguration(const char *filename, Config &config)
{
  while (!SPIFFS.begin())
  {
    Serial.println(F("Failed to initialize SD library"));
    delay(1000);
  }
  File file = SPIFFS.open(filename);
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(file);

  if (!root.success())
    Serial.println(F("Failed to read file, using default configuration"));

  config._mode_serial = root["mode_serial"] | "SERIAL_8E1";
  config._serial_baund = root["serial_baund"] | "9600";
  config._mode_wifi = root["mode_wifi"] | "WIFI_AP";
  config._wifi_ssid = root["wifi_ssid"] | "";
  config._wifi_pass = root["wifi_pass"] | "";
  config._wifi_ssid_ap = root["wifi_ssid_ap"] | "Korm TCP Server";
  config._wifi_pass_ap = root["wifi_pass_ap"] | "12345678";
  config._port_tcp = root["port_tcp"] | "4001";
  config._dhcp = root["dhcp"] | "0";
  config._static_ip = root["static_ip"] | "10.200.1.30";
  config._static_mask = root["static_mask"] | "255.255.252.0";
  config._static_gataway = root["static_gataway"] | "10.200.1.1";

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

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();

  root["mode_serial"] = config._mode_serial;
  root["serial_baund"] = config._serial_baund;
  root["mode_wifi"] = config._mode_wifi;
  root["client_tcp"] = config._client_tcp;
  root["wifi_rssi"] = config._wifi_rssi;
  root["ip_addr"] = config._ip_addr;
  root["mask_addr"] = config._mask_addr;
  root["gataway_addr"] = config._gataway_addr;
  root["free_ram"] = config._free_ram;
  root["wifi_ssid"] = config._wifi_ssid;
  root["wifi_pass"] = config._wifi_pass;
  root["wifi_ssid_ap"] = config._wifi_ssid_ap;
  root["wifi_pass_ap"] = config._wifi_pass_ap;
  root["port_tcp"] = config._port_tcp;
  root["dhcp"] = config._dhcp;
  root["static_ip"] = config._static_ip;
  root["static_mask"] = config._static_mask;
  root["static_gataway"] = config._static_gataway;

  if (root.printTo(file) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }

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
    if (t + 7500 < millis())
    {
      Serial.println("Reset ESP32");
      Serial.println("Read default config");
      SPIFFS.remove("/config.json");
      ESP.restart();
    }
  }
}
