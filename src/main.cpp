#include <Arduino.h>

#include <config_load.h>
#include <tcp_server.h>
#include <server_web.h>

void setup()
{
  loadConfiguration(filename, config);

  Serial.begin(config._serial_baund.toInt());
  Serial1.begin(config._serial_baund.toInt(), SERIAL_7E1);

  setup_tcp_server();

  setup_server_web();
}

void loop()
{
  loop_tcp_server();

  loop_server_web();
}