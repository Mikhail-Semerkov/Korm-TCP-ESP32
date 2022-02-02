#include <global.h>

#include <config_load.h>
#include <tcp_server.h>
#include <server_web.h>

uint32_t myTimer1;

void setup()
{
  loadConfiguration(filename, config);

  Serial.begin(config._serial_baund.toInt());
  Serial1.begin(config._serial_baund.toInt(), uint32_t(config._mode_serial.c_str()));

  setup_tcp_server();

  setup_server_web();

  Serial2.begin(9600);
}

void Check_UART2()
{
  if (Serial2.available() > 0)
  {
    Data_DS = Serial2.readStringUntil('\r');

    Data_DS.replace("\r", "");
    Data_DS.replace("\n", "");
  }
}

void loop()
{

  btn_default();

  loop_tcp_server();

  loop_server_web();

  Check_UART2();

  if (millis() - myTimer1 >= 100)
  {
    myTimer1 = millis();
    Serial.print(Data_DS);
  }
}