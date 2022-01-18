#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

WebServer web_server(80);

const char *host = "esp32fs";

File fsUploadFile;


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

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
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

void button_save_click()
{
  Serial.println(F("Saving configuration..."));
  saveConfiguration(filename, config);
  web_server.send(200, "text/html", ""); //посылка ответа сервера
  Serial.println(F("Loading configuration..."));
  loadConfiguration(filename, config);
}


void button_reboot_click()
{
  Serial.println(F("Rebooting ESP32..."));
  web_server.send(200, "text/html", ""); //посылка ответа сервера
  delay(3000);
  ESP.restart();

}

void setup_server_web(void)
{
  SPIFFS.begin();
  {
    listDir(SPIFFS, "/", 0);
    Serial.printf("\n");
  }

  while (!SPIFFS.begin())
  {
    Serial.println(F("Failed to initialize SD library"));
    delay(1000);
  }

  
  

  Serial.println(F("Print config file..."));
  printFile(filename);

  MDNS.begin(host);
  Serial.print("Open http://");
  Serial.print(host);
  Serial.println(".local/edit to see the file browser");

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

  web_server.on("/all", HTTP_GET, []()
                {
                  String
                      json = "{\n";

                  json += "\"mode_wifi\":" + String("\"") + String(config.MODE_WIFI) + String("\", \n");
                  json += "\"client_tcp\":" + String("\"") + String(Client_Connected) + String("\", \n");
                  json += "\"wifi_rssi\":" + String("\"") + String(WiFi.RSSI()) + String("\", \n");


if(String(config.MODE_WIFI) == "STA")
{
json += "\"ip_addr\":" + String("\"") + String(ip2Str(WiFi.localIP())) + String("\", \n");
json += "\"mask_addr\":" + String("\"") + String(ip2Str(WiFi.subnetMask())) + String("\", \n");
}
if(String(config.MODE_WIFI) == "AP")
{
json += "\"ip_addr\":" + String("\"") + String(ip2Str(WiFi.softAPIP())) + String("\", \n");
json += "\"mask_addr\":" + String("\"") + String(ip2Str(WiFi.softAPSubnetCIDR())) + String("\", \n");
}
                  

                  
                  json += "\"gataway_addr\":" + String("\"") + String(ip2Str(WiFi.gatewayIP())) + String("\", \n");
                  json += "\"free_ram\":" + String("\"") + String(ESP.getFreeHeap()) + String("\", \n");

                  json += "\"ssid\":" + String("\"") + String(config.SSID_CONFIG) + String("\", \n");
                  json += "\"pass\":" + String("\"") + String(config.PASS_CONFIG) + String("\", \n");
                  json += "\"ssid_ap\":" + String("\"") + String(config.SSID_AP_CONFIG) + String("\", \n");
                  json += "\"pass_ap\":" + String("\"") + String(config.PASS_AP_CONFIG) + String("\", \n");
                  json += "\"port_tcp\":" + String("\"") + String(config.PORT_TCP) + String("\"");

                  json += "\n}";

                  web_server.send(200, "text/json", json);
                  json = String();
                });


  web_server.on("/reboot_esp_set", button_reboot_click);
  web_server.on("/save_config_set", button_save_click);

  web_server.begin();
  Serial.println("HTTP web_server started");
}

void loop_server_web(void)
{
  web_server.handleClient();
}