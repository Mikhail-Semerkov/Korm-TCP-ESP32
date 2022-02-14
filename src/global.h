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

int timer_default;
bool default_flag;

//////////////             Digi-Star Protocol                  /////////////

struct Digi_Star_Protocol
{

    //Значение Стандартов
    char SUB = ''; // Hex 1A
    char STX = ''; // Hex 02
    char ETX = ''; // Hex 03
    char ACK = ''; // Hex 06
    char NAK = ''; // Hex 15
    char CR = '';  // Hex 0D
    char LF = '';  // Hex 10
    char RS = '';  // Hex 1E
    char ESC = ''; // Hex 1B
    char EOT = ''; // Hex 04

    //////////////Основные кнопки/////////////////
    String MENU = "1700";
    String SBROS = "6600";
    String PRINT = "2300";
    String ZERO = "4300";
    String UP = "4400";
    String DOWN = "2400";
    String LEFT = "4200";
    String RIGHT = "2200";
    String ENTER = "3300";
    String CLEAR = "1700";
    String ON = "0800";
    String SELECT = "2700";
    String ID = "1200";
    String HELP = "1300";
    String FUNCTION = "3700";
    String PENS = "1100";
    String RECIPE = "3100";
    String NETGROSS = "1000";
    String TARE = "4000";
    String TIMER = "4700";
    String HOLD = "2000";

    //Кнопки цифр
    String N0 = "1600";
    String N1 = "3400";
    String N2 = "4500";
    String N3 = "3500";
    String N4 = "2500";
    String N5 = "1500";
    String N6 = "1400";
    String N7 = "4600";
    String N8 = "3600";
    String N9 = "2600";
};

Digi_Star_Protocol DS;
