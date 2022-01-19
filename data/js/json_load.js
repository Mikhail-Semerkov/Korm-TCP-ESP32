request = new XMLHttpRequest();

function send_request(submit,server){
 request = new XMLHttpRequest();
 request.open("GET", server, true);
 request.send();
 save_status(submit,request);
}

var serial_config,
  serial_baund,
  mode_wifi,
  client_tcp,
  wifi_rssi,
  ip_addr,
  mask_addr,
  gataway_addr,
  free_ram,
  ssid,
  pass,
  ssid_ap,
  pass_ap,
  port_tcp;

var reloadPeriod = 1000;
var running = false;
var Setting_Load = true;

function loadValues() {
  if (!running) return;
  var xh = new XMLHttpRequest();
  xh.onreadystatechange = function () {
    if (xh.readyState == 4) {
      if (xh.status == 200) {
        var res = JSON.parse(xh.responseText);

        if (Setting_Load == true) {
          //Настройки серийного порта
          document.getElementById("select_serial_config").value =
            res.serial_config;
          document.getElementById("select_serial_baund").value =
            res.serial_baund;

          Setting_Load = false;
        }

        document.getElementById("MODE_WIFI_RESULT").textContent =
          "Mode: " + res.mode_wifi;
        document.getElementById("IP_ADDR_RESULT").textContent = res.ip_addr;

        document.getElementById("CLIENT_TCP").textContent =
          "Число клиентов: " + res.client_tcp;
        document.getElementById("WIFI_RSSI").textContent =
          "Качество сигнала: " + res.wifi_rssi;
        document.getElementById("IP_ADDR").textContent =
          "IP адрес: " + res.ip_addr;
        document.getElementById("MASK_ADDR").textContent =
          "Маска: " + res.mask_addr;
        document.getElementById("GATAWAY_ADDR").textContent =
          "Шлюз: " + res.gataway_addr;

        document.getElementById("FREE_RAM").textContent =
          "RAM память: " + res.free_ram;
        document.getElementById("SSID").textContent = "Имя сети: " + res.ssid;
        document.getElementById("PASS").textContent = "Пароль: " + res.pass;
        document.getElementById("PORT_TCP").textContent =
          "Порт TCP: " + res.port_tcp;

        if (running) setTimeout(loadValues, reloadPeriod);
      } else running = false;
    }
  };
  xh.open("GET", "/all", true);
  xh.send(null);
}

function run() {
  if (!running) {
    running = true;
    loadValues();
  }
}

function onBodyLoad() {
  run();
}



