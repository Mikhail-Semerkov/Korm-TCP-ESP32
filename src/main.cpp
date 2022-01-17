#include <Arduino.h>

#include <tcp_server.h>
#include <server_web.h>

void setup()
{
  setup_tcp_server();

  setup_server_web();
}

void loop()
{
  loop_tcp_server();

  loop_server_web();
}