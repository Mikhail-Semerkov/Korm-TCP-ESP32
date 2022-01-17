
var client_tcp, wifi_rssi, ip_addr, mask_addr, gataway_addr, free_ram, ssid, pass, ssid_ap, pass_ap, port_tcp;

var reloadPeriod = 1000;
var running = false;

function loadValues() {
  if (!running) return;
  var xh = new XMLHttpRequest();
  xh.onreadystatechange = function () {
    if (xh.readyState == 4) {
      if (xh.status == 200) {
        var res = JSON.parse(xh.responseText);

        document.getElementById("CLIENT_TCP").textContent = "CLIENT_TCP: " + res.client_tcp;
        document.getElementById("WIFI_RSSI").textContent = "WIFI_RSSI: " + res.wifi_rssi;
        document.getElementById("IP_ADDR").textContent = "IP_ADDR: " + res.ip_addr;
        document.getElementById("MASK_ADDR").textContent = "MASK_ADDR: " + res.mask_addr;
        document.getElementById("GATAWAY_ADDR").textContent = "GATAWAY_ADDR: " + res.gataway_addr;

        document.getElementById("FREE_RAM").textContent = "FREE_RAM: " + res.free_ram;
        document.getElementById("SSID").textContent = "SSID: " + res.ssid;
        document.getElementById("PASS").textContent = "PASS: " + res.pass;
        document.getElementById("SSID_AP").textContent = "SSID_AP: " + res.ssid_ap;
        document.getElementById("PASS_AP").textContent = "PASS_AP: " + res.pass_ap;
        document.getElementById("PORT_TCP").textContent = "PORT_TCP: " + res.port_tcp;


        if (running) setTimeout(loadValues, reloadPeriod);
      } else running = false;
    }
  };
  xh.open("GET", "/all", true);
  xh.send(null);
};

function run() {
  if (!running) {
    running = true;
    loadValues();
  }
}

function onBodyLoad() {
  run();
}
