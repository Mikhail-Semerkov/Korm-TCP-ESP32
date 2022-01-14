#include<Arduino.h>

#include<tcp_server.h>

#include<web_server.h>


void setup() 
{

  setup_tcp_server();
  setup_web_server();

}

void loop() 
{

  loop_tcp_server();
  loop_web_server();

}