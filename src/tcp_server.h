
#ifdef DEBUG_ENABLE
#define DEBUG(x) Serial.println(x)
#else
#define DEBUG(x)
#endif

#ifdef CONFIG_ENABLE
#define DEBUG(x) Serial.println(x)
#else
#define DEBUG(x)
#endif





#include <WiFi.h>

////////////////// GLOBAL SETTINGS //////////////////

//#define DEBUG_ENABLE


#define MAX_SRV_CLIENTS 6

int SERIAL_BAUND = 9600;

int Port_TCP = 4001;

int Client_Connected = 0;

const char* ssid = "indconf2002";

const char* password = "07031968200703196820123456";


//#define CONFIG_ENABLE
IPAddress local_IP(10, 170, 172, 172);
IPAddress gateway(10, 170, 175, 254);
IPAddress subnet(255, 255, 252, 0);
IPAddress primaryDNS(8, 8, 8, 8);   
IPAddress secondaryDNS(8, 8, 4, 4); 

///////////////////////////////////////////////////


WiFiServer server(Port_TCP);
WiFiClient serverClients[MAX_SRV_CLIENTS];


void Check_UART()
{
  uint8_t i;
  
  if(Serial1.available())
  {
    size_t len = Serial1.available();
    uint8_t sbuf[len];
    Serial1.readBytes(sbuf, len);
    
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
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
  
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
    
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Client_Connected = i+1;
        Serial.print("New client: "); Serial.print(Client_Connected);
        Serial.println();
        break;
      }
    }
    
    if ( i == MAX_SRV_CLIENTS) {
      WiFiClient serverClient = server.available();
      serverClient.stop();
        Serial.println("Connection rejected ");
    }
  }
}


void Check_TCP()
{
  uint8_t i;
    
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
       
        while(serverClients[i].available()) Serial1.write(serverClients[i].read());
      }
    }
  }
}

void setup_tcp_server() 
{

  Serial.begin(SERIAL_BAUND);
  Serial1.begin(SERIAL_BAUND, SERIAL_7E1);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);


#ifdef CONFIG_ENABLE
  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
#endif


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("\nConnecting to: "); Serial.println(ssid);
  uint8_t i = 0;

  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);

  if(i == 21)
  {
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }
  
  server.begin();
  server.setNoDelay(true);
 

  Serial.print("Ready! You are connected\n"); Serial.print("IP: "); Serial.print(WiFi.localIP()); Serial.print(", Port:" + String(Port_TCP));
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