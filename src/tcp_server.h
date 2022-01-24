////////////////// GLOBAL SETTINGS //////////////////

#include <WiFi.h>

#define MAX_SRV_CLIENTS 4

int Client_Connected = 0;

///////////////// PARAM SETTINGS ////////////////////

const char *Hostname = "KormTCPserver";

IPAddress local_IP_AP(192, 168, 1, 1);
IPAddress gateway_AP(192, 168, 1, 254);
IPAddress subnet_AP(255, 255, 255, 0);

bool DHCP;

int Port_TCP = 4001;

IPAddress local_IP, gateway, subnet, primaryDNS, secondaryDNS;

///////////////////////////////////////////////////

WiFiServer server(Port_TCP);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void Check_UART()
{

  uint8_t i;

  if (Serial1.available())
  {
    size_t len = Serial1.available();
    uint8_t sbuf[len];
    Serial1.readBytes(sbuf, len);

    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      if (serverClients[i] && serverClients[i].connected())
      {
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }
}

void Check_Clients()
{
  uint8_t i;

  if (server.hasClient())
  {
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {

      if (!serverClients[i] || !serverClients[i].connected())
      {
        if (serverClients[i])
          serverClients[i].stop();
        serverClients[i] = server.available();
        Client_Connected = i + 1;
        Serial.print("New client: ");
        Serial.print(Client_Connected);
        Serial.println();
        break;
      }
    }

    if (i == MAX_SRV_CLIENTS)
    {
      WiFiClient serverClient = server.available();
      serverClient.stop();
      Serial.println("Connection rejected ");
    }
  }
}

void Check_TCP()
{
  uint8_t i;

  for (i = 0; i < MAX_SRV_CLIENTS; i++)
  {
    if (serverClients[i] && serverClients[i].connected())
    {
      if (serverClients[i].available())
      {
        while (serverClients[i].available())
          Serial1.write(serverClients[i].read());
      }
    }
  }
}

void setup_tcp_server()
{

  DHCP = bool(config._dhcp.toInt());

  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  if (WiFi.getHostname() != Hostname)
  {
    WiFi.setHostname(Hostname);
  }

  if (DHCP == true)
  {
    Serial.println();
    local_IP.fromString(config._static_ip);
    gateway.fromString(config._static_gataway);
    subnet.fromString(config._static_mask);

    Serial.println("Static Local Ok");
    WiFi.config(local_IP, gateway, subnet);
  }

  if (String(config._mode_wifi) == "WIFI_STA")
  {

    Serial.println();
    Serial.println("MODE STA");

    WiFi.mode(WIFI_STA);
    WiFi.begin(config._wifi_ssid.c_str(), config._wifi_pass.c_str());

    Serial.print("\nConnecting to: ");
    Serial.println(config._wifi_ssid);
    uint8_t i = 0;

    while (WiFi.status() != WL_CONNECTED && i++ < 20)
      delay(500);

    if (i == 21)
    {
      Serial.print("Could not connect to");
      Serial.println(config._wifi_ssid);
      //while (1)
      delay(500);
    }

    Serial.print("Ready! You are connected\n");
    Serial.print("IP: ");
    Serial.print(WiFi.localIP());
    Serial.print(", Port:" + String(Port_TCP));
    Serial.println();
    Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
    Serial.println();
  }
  else if (String(config._mode_wifi) == "WIFI_AP")
  {

    Serial.println();
    Serial.println("MODE AP");

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_IP_AP, gateway_AP, subnet_AP);
    WiFi.softAP(config._wifi_ssid_ap.c_str(), config._wifi_pass_ap.c_str());

    Serial.print("Ready! You are connected\n");
    Serial.print("IP: ");
    Serial.print(local_IP_AP);
    Serial.print(", Port:" + String(Port_TCP));
    Serial.println();
  }

  server.begin();
  server.setNoDelay(true);
}

void loop_tcp_server()
{

  Check_Clients();
  Check_TCP();
  Check_UART();
}