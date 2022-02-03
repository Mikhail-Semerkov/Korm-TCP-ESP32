#include <global.h>

#include <config_load.h>
#include <tcp_server.h>
#include <server_web.h>

void setup()
{
  loadConfiguration(filename, config);

  Serial.begin(config._serial_baund.toInt());
  Serial1.begin(config._serial_baund.toInt());
  Serial2.begin(config._serial_baund.toInt());

  // Serial1.begin(config._serial_baund.toInt(), uint32_t(config._mode_serial.c_str()));

  setup_tcp_server();
  setup_server_web();
}

void check_dublore()
{
  if (Serial2.available() > 0)
  {
    Data_DS = Serial2.readStringUntil('\r');

    Data_DS.replace("\r", "");
    Data_DS.replace("\n", "");
  }

  if (millis() - Timer_Data_DS >= 100)
  {
    Timer_Data_DS = millis();
  }
}

void loop()
{

  btn_default();

  loop_tcp_server();

  loop_server_web();

  check_dublore();
}