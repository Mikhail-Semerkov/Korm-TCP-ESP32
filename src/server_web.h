#include <WiFiClient.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

WebServer web_server(80);

File fsUploadFile;

String translateEncryptionType(wifi_auth_mode_t encryptionType)
{
  switch (encryptionType)
  {
  case (WIFI_AUTH_OPEN):
    return "Open";
  case (WIFI_AUTH_WEP):
    return "WEP";
  case (WIFI_AUTH_WPA_PSK):
    return "WPA_PSK";
  case (WIFI_AUTH_WPA2_PSK):
    return "WPA2_PSK";
  case (WIFI_AUTH_WPA_WPA2_PSK):
    return "WPA_WPA2_PSK";
  case (WIFI_AUTH_WPA2_ENTERPRISE):
    return "WPA2_ENTERPRISE";
  }
}

void scanNetworks()
{
  int numberOfNetworks = WiFi.scanNetworks();

  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);

  for (int i = 0; i < numberOfNetworks; i++)
  {
    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));

    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));

    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));

    Serial.print("Encryption type: ");
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-----------------------");
  }
}



String formatBytes(size_t bytes)
{
  if (bytes < 1024)
  {
    return String(bytes) + "B";
  }
  else if (bytes < (1024 * 1024))
  {
    return String(bytes / 1024.0) + "KB";
  }
  else if (bytes < (1024 * 1024 * 1024))
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
  else
  {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename)
{
  if (web_server.hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  else if (filename.endsWith(".ttf"))
    return "font/ttf";
  return "text/plain";
}

bool handleFileRead(String path)
{
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/"))
    path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
  {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = web_server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handleFileUpload()
{
  if (web_server.uri() != "/edit")
    return;
  HTTPUpload &upload = web_server.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    String filename = upload.filename;
    if (!filename.startsWith("/"))
      filename = "/" + filename;
    Serial.print("handleFileUpload Name: ");
    Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    //Serial.print("handleFileUpload Data: "); Serial.println(upload.currentSize);
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (fsUploadFile)
      fsUploadFile.close();
    Serial.print("handleFileUpload Size: ");
    Serial.println(upload.totalSize);
  }
}

void handleFileDelete()
{
  if (web_server.args() == 0)
    return web_server.send(500, "text/plain", "BAD ARGS");
  String path = web_server.arg(0);
  Serial.println("handleFileDelete: " + path);
  if (path == "/")
    return web_server.send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return web_server.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  web_server.send(200, "text/plain", "");
  path = String();
}

void handleFileCreate()
{
  if (web_server.args() == 0)
    return web_server.send(500, "text/plain", "BAD ARGS");
  String path = web_server.arg(0);
  Serial.println("handleFileCreate: " + path);
  if (path == "/")
    return web_server.send(500, "text/plain", "BAD PATH");
  if (SPIFFS.exists(path))
    return web_server.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if (file)
    file.close();
  else
    return web_server.send(500, "text/plain", "CREATE FAILED");
  web_server.send(200, "text/plain", "");
  path = String();
}

void returnFail(String msg)
{
  web_server.send(500, "text/plain", msg + "\r\n");
}

void handleFileList()
{
  if (!web_server.hasArg("dir"))
  {
    returnFail("BAD ARGS");
    return;
  }
  String path = web_server.arg("dir");
  if (path != "/" && !SPIFFS.exists((char *)path.c_str()))
  {
    returnFail("BAD PATH");
    return;
  }
  File dir = SPIFFS.open((char *)path.c_str());
  path = String();
  if (!dir.isDirectory())
  {
    dir.close();
    returnFail("NOT DIR");
    return;
  }
  dir.rewindDirectory();

  String output = "[";
  for (int cnt = 0; true; ++cnt)
  {
    File entry = dir.openNextFile();
    if (!entry)
      break;

    if (cnt > 0)
      output += ',';

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += entry.name() + 1;
    output += "\"";
    output += "}";
    entry.close();
  }
  output += "]";
  web_server.send(200, "text/json", output);
  dir.close();
}

String ip2Str(IPAddress ip)
{
  String s = "";
  for (int i = 0; i < 4; i++)
  {
    s += i ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}

void button_reboot_click()
{
  Serial.println(F("Rebooting ESP32..."));
  web_server.send(200, "text/html", "");
  delay(3000);
  ESP.restart();
}

void wi_wi_scan_click()
{
  Serial.println(F("WiFi Scan ESP32..."));
  web_server.send(200, "text/html", "");
  scanNetworks();
}

void save_web_config()
{

  config._mode_wifi = web_server.arg("mode_wifi");
  config._wifi_ssid = web_server.arg("wifi_ssid");

  config._wifi_pass = web_server.arg("wifi_pass");
  config._port_tcp = web_server.arg("port_tcp");
  config._mode_serial = web_server.arg("mode_serial");
  config._serial_baund = web_server.arg("serial_baund");
  config._dhcp = web_server.arg("dhcp");
  config._static_ip = web_server.arg("static_ip");
  config._static_mask = web_server.arg("static_mask");
  config._static_gataway = web_server.arg("static_gataway");

  String Data = "mode_wifi: " + config._mode_wifi +
                ", wifi_ssid: " + config._wifi_ssid +
                ", wifi_pass: " + config._wifi_pass +
                ", port_tcp: " + config._port_tcp +
                ", mode_serial: " + config._mode_serial +
                ", serial_baund: " + config._serial_baund +

                ", dhcp: " + config._dhcp +
                ", static_ip: " + config._static_ip +
                ", static_mask: " + config._static_mask +
                ", static_gataway: " + config._static_gataway;

  web_server.send(200, "text/plane", "OK");

  Serial.println(F("Saving configuration..."));

  saveConfiguration(filename, config);

  Serial.println(Data);

  // Serial.println(F("Loading configuration..."));
  // loadConfiguration(filename, config);
}

void default_settings_esp()
{
  Serial.println("Reset ESP32");
  Serial.println("Read default config");
  SPIFFS.remove("/config.json");
  ESP.restart();
}

void setup_server_web(void)
{

  Serial.println(F("Print config file..."));
  printFile(filename);


  web_server.on("/list", HTTP_GET, handleFileList);
  web_server.on("/edit", HTTP_GET, []()
                {
                  if (!handleFileRead("/edit.htm"))
                    web_server.send(404, "text/plain", "FileNotFound");
                });
  web_server.on("/edit", HTTP_PUT, handleFileCreate);
  web_server.on("/edit", HTTP_DELETE, handleFileDelete);
  web_server.on(
      "/edit", HTTP_POST, []()
      { web_server.send(200, "text/plain", ""); },
      handleFileUpload);

  web_server.onNotFound([]()
                        {
                          if (!handleFileRead(web_server.uri()))
                            web_server.send(404, "text/plain", "FileNotFound");
                        });

  web_server.on("/json", HTTP_GET, []()
                {
                  String
                      json = "{\n";

                  json += "\"mode_serial\":" + String("\"") + config._mode_serial + String("\", \n");
                  json += "\"serial_baund\":" + String("\"") + config._serial_baund + String("\", \n");
                  json += "\"mode_wifi\":" + String("\"") + config._mode_wifi + String("\", \n");
                  json += "\"client_tcp\":" + String("\"") + String(Client_Connected) + String("\", \n");
                  json += "\"wifi_rssi\":" + String("\"") + String(WiFi.RSSI()) + String("\", \n");

                  if (config._mode_wifi == "WIFI_STA")
                  {
                    json += "\"ip_addr\":" + String("\"") + String(ip2Str(WiFi.localIP())) + String("\", \n");
                  }
                  if (config._mode_wifi == "WIFI_AP")
                  {
                    json += "\"ip_addr\":" + String("\"") + String(ip2Str(local_IP_AP)) + String("\", \n");
                  }

                  json += "\"mask_addr\":" + String("\"") + String(ip2Str(WiFi.subnetMask())) + String("\", \n");
                  json += "\"gataway_addr\":" + String("\"") + String(ip2Str(WiFi.gatewayIP())) + String("\", \n");
                  json += "\"free_ram\":" + String("\"") + String(ESP.getFreeHeap()) + String("\", \n");
                  json += "\"wifi_ssid\":" + String("\"") + config._wifi_ssid + String("\", \n");
                  json += "\"wifi_pass\":" + String("\"") + config._wifi_pass + String("\", \n");
                  json += "\"wifi_ssid_ap\":" + String("\"") + config._wifi_ssid_ap + String("\", \n");
                  json += "\"wifi_pass_ap\":" + String("\"") + config._wifi_pass_ap + String("\", \n");
                  json += "\"port_tcp\":" + String("\"") + config._port_tcp + String("\", \n");

                  json += "\"dhcp\":" + String("\"") + config._dhcp + String("\", \n");
                  json += "\"static_ip\":" + String("\"") + config._static_ip + String("\", \n");
                  json += "\"static_mask\":" + String("\"") + config._static_mask + String("\", \n");
                  json += "\"static_gataway\":" + String("\"") + config._static_gataway + String("\"");

                  json += "\n}";

                  web_server.send(200, "text/json", json);
                  json = String();
                });

  web_server.on("/reboot_esp_set", button_reboot_click);
  web_server.on("/wi_wi_scan_esp_set", wi_wi_scan_click);
  web_server.on("/save_web_config_set", save_web_config);
  web_server.on("/default_settings_esp_set", default_settings_esp);

  web_server.begin();

  Serial.println("HTTP web_server started");
}

void loop_server_web(void)
{
  web_server.handleClient();
}