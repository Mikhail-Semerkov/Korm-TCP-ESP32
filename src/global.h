/////////////////                Include                   /////////////////
#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WiFi.h>

/////////////////              Load Config                 /////////////////

struct Config
{
    String _mode_serial, _serial_baund, _mode_wifi, _client_tcp, _wifi_rssi, _ip_addr, _mask_addr, _gataway_addr, _free_ram, _wifi_ssid, _wifi_pass, _wifi_ssid_ap, _wifi_pass_ap, _port_tcp, _dhcp, _static_ip, _static_mask, _static_gataway;
};

const char *filename = "/config.json";
Config config;

String Data_DS;
uint32_t Timer_Data_DS;

/////////////////              WEB Server                  /////////////////

WebServer web_server(80);

File fsUploadFile;

/////////////////              TCP Server                  /////////////////

#define MAX_SRV_CLIENTS 4

int Client_Connected = 0;

const char *Hostname = "KormTCPserver";

IPAddress local_IP_AP(192, 168, 1, 1);
IPAddress gateway_AP(192, 168, 1, 254);
IPAddress subnet_AP(255, 255, 255, 0);

bool DHCP;
int Port = 4001;

IPAddress local_IP, gateway, subnet, primaryDNS, secondaryDNS;
WiFiServer server(Port);
WiFiClient serverClients[MAX_SRV_CLIENTS];