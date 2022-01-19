////////////////// GLOBAL SETTINGS //////////////////

#include <WiFi.h>

#define MAX_SRV_CLIENTS 4

int Client_Connected = 0;

///////////////// PARAM SETTINGS ////////////////////

bool DHCP = true;

int Port_TCP = 4001;

IPAddress local_IP(10, 200, 90, 31);
IPAddress gateway(10, 200, 90, 254);
IPAddress subnet(255, 255, 252, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

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
        Client_Connected = i;
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
  //WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  if (DHCP != true)
  {
    WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  }

  if (String(config._mode_wifi) == "STA")
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(config._ssid.c_str(), config._pass.c_str());

    Serial.print("\nConnecting to: ");
    Serial.println(config._ssid);
    uint8_t i = 0;

    while (WiFi.status() != WL_CONNECTED && i++ < 20)
      delay(500);

    if (i == 21)
    {
      Serial.print("Could not connect to");
      Serial.println(config._ssid);
      while (1)
        delay(500);
    }
  }
  else if (String(config._mode_wifi) == "AP")
  {
    IPAddress local_IP(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 254);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(config._ssid_ap.c_str(), config._pass_ap.c_str());
  }

  server.begin();
  server.setNoDelay(true);

  Serial.print("Ready! You are connected\n");
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  Serial.print(", Port:" + String(Port_TCP));
  Serial.println();
  Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
  Serial.println();
}

void loop_tcp_server()
{

  Check_Clients();
  Check_TCP();
  Check_UART();
}