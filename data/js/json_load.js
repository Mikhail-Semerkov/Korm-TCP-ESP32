var reloadPeriod = 1000;
var running = false;
var setting_load = false;

var mode_serial,
  serial_baund,
  mode_wifi,
  client_tcp,
  wifi_rssi,
  ip_addr,
  mask_addr,
  gataway_addr,
  free_ram,
  wifi_ssid,
  wifi_pass,
  wifi_ssid_ap,
  wifi_pass_ap,
  port_tcp,
  dhcp,
  static_ip,
  static_mask,
  static_gataway;

var conf_mode_wifi;
var conf_ip_addr;
var conf_mode_serial;
var conf_serial_baund;
var conf_client_tcp;
var conf_wifi_rssi;
var conf_mask_addr;
var conf_gataway_addr;
var conf_free_ram;
var conf_wifi_ssid;
var conf_wifi_pass;
var conf_wifi_ssid_ap;
var conf_wifi_pass_ap;
var conf_port_tcp;

var settings_wifi_ssid;
var settings_wifi_pass;
var settings_port_tcp;
var settings_mode_serial;
var settings_serial_baund;

var settings_dhcp;
var settings_static_ip;
var settings_static_mask;
var settings_static_gataway;

var label_mode_wifi;

function loadValues() {
  if (!running) return;
  var xh = new XMLHttpRequest();
  xh.onreadystatechange = function () {
    if (xh.readyState == 4) {
      if (xh.status == 200) {
        var res = JSON.parse(xh.responseText);

        ////////////////////////////////////////////////////////////////////

        conf_mode_wifi = document.getElementById("ui_mode_wifi");
        conf_ip_addr = document.getElementById("ui_ip_addr");
        conf_mode_serial = document.getElementById("ui_mode_serial");
        conf_serial_baund = document.getElementById("ui_serial_baund");
        conf_client_tcp = document.getElementById("ui_client_tcp");
        conf_wifi_rssi = document.getElementById("ui_wifi_rssi");
        conf_mask_addr = document.getElementById("ui_mask_addr");
        conf_gataway_addr = document.getElementById("ui_gataway_addr");
        conf_free_ram = document.getElementById("ui_free_ram");
        conf_wifi_ssid = document.getElementById("ui_wifi_ssid");
        conf_wifi_pass = document.getElementById("ui_wifi_pass");
        conf_wifi_ssid_ap = document.getElementById("ui_wifi_ap_ssid");
        conf_wifi_pass_ap = document.getElementById("ui_wifi_ap_pass");
        conf_port_tcp = document.getElementById("ui_port_tcp");

        //////////////////////////////////////////////////////////////////

        settings_wifi_ssid = document.getElementById("settings_wifi_ssid");
        settings_wifi_pass = document.getElementById("settings_wifi_pass");
        settings_port_tcp = document.getElementById("settings_port_tcp");
        settings_mode_serial = document.getElementById("settings_mode_serial");
        settings_serial_baund = document.getElementById(
          "settings_serial_baund"
        );
        settings_dhcp = document.getElementById("DHCP");
        settings_static_ip = document.getElementById("settings_static_ip");
        settings_static_mask = document.getElementById("settings_static_mask");
        settings_static_gataway = document.getElementById(
          "settings_static_gataway"
        );

        ///////////////////////////////////////////////////////////////////

        label_mode_wifi = document.getElementById("label_mode_wifi");

        ///////////////////////////////////////////////////////////////////

        conf_mode_wifi.textContent = "MODE WIFI: " + res.mode_wifi;
        conf_ip_addr.textContent = "IP ADDR: " + res.ip_addr;
        conf_mode_serial.textContent = "MODE SERIAL: " + res.mode_serial;
        conf_serial_baund.textContent = "SERIAL BAUND: " + res.serial_baund;
        conf_client_tcp.textContent = "CLIENT TCP: " + res.client_tcp;
        conf_wifi_rssi.textContent = "WIFI RSSI: " + res.wifi_rssi;
        conf_mask_addr.textContent = "MASK ADDR: " + res.mask_addr;
        conf_gataway_addr.textContent = "GATAWAY ADDR: " + res.gataway_addr;
        conf_free_ram.textContent = "FREE RAM: " + res.free_ram;
        conf_wifi_ssid.textContent = "WIFI SSID: " + res.wifi_ssid;
        conf_wifi_pass.textContent = "WIFI PASS: " + res.wifi_pass;
        conf_wifi_ssid_ap.textContent = "WIFI SSID AP: " + res.wifi_ssid_ap;
        conf_wifi_pass_ap.textContent = "WIFI PASS AP: " + res.wifi_pass_ap;
        conf_port_tcp.textContent = "PORT TCP: " + res.port_tcp;




        var label_rssi_wifi = document.getElementById("label_rssi_wifi");

         if (res.mode_wifi == "WIFI_AP") {
           
           label_rssi_wifi.style = "display:none";
        }
        if (res.mode_wifi == "WIFI_STA") {
          label_rssi_wifi.textContent = conf_wifi_rssi.textContent;
            label_rssi_wifi.style = "";
      
          }

        if (setting_load == false) {

          settings_wifi_ssid.value = res.wifi_ssid;
          settings_wifi_pass.value = res.wifi_pass;
          settings_port_tcp.value = res.port_tcp;
          settings_mode_serial.value = res.mode_serial;
          settings_serial_baund.value = res.serial_baund;
          settings_dhcp.value = res.dhcp;
          settings_static_ip.value = res.static_ip;
          settings_static_mask.value = res.static_mask;
          settings_static_gataway.value = res.static_gataway;


          
          label_mode_wifi.textContent = res.mode_wifi + "_MODE";
          label_ip_addr.textContent = res.ip_addr;



          if (res.mode_wifi == "WIFI_AP") {
            settings_mode_wifi = "WIFI_AP";
            radio_button_ap.checked = true;
            settings_mode_sta.style = "display:none";
            settings_mode_ap.style = "";
          }
          if (res.mode_wifi == "WIFI_STA") {
            settings_mode_wifi = "WIFI_STA";
            radio_button_sta.checked = true;
            settings_mode_sta.style = "";
            settings_mode_ap.style = "display:none";
          }

          if (settings_dhcp.value == true) {
            dhcp_mode.style = "";
          } else {
            dhcp_mode.style = "display:none";
          }

          setting_load = true;
        }

        if (running) setTimeout(loadValues, reloadPeriod);
      } else running = false;
    }
  };
  xh.open("GET", "/json", true);
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
