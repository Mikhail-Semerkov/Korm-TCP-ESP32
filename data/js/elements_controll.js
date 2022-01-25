var request = new XMLHttpRequest();

var settings_mode_wifi;
var settings_mode_ap = document.getElementById("settings_mode_ap");
var settings_mode_sta = document.getElementById("settings_mode_sta");
var radio_button_sta = document.getElementById("radio_sta");
var radio_button_ap = document.getElementById("radio_ap");
var label_ip_addr = document.getElementById("label_ip_addr");

var dhcp_mode = document.getElementById("dhcp_mode");

function handleChange(src) {
  if (src.value == "WIFI_STA") {
    settings_mode_wifi = "WIFI_STA";
    settings_mode_sta.style = "";
    settings_mode_ap.style = "display:none";
  } else {
    settings_mode_wifi = "WIFI_AP";
    settings_mode_sta.style = "display:none";
    settings_mode_ap.style = "";
  }
}

function handleCheckBox(src) {
  if (src.checked) {
    dhcp_mode.style = "";
    settings_dhcp.value = 1;
  } else {
    dhcp_mode.style = "display:none";
    settings_dhcp.value = 0;
  }
}

//Reboot
function button_default_reset() {
  console.log("Default Setting");
  request.open("GET", "/default_settings_esp_set", true);
  request.send();
}

//Scan WiFi
function wi_wi_scan_esp_click() {
  console.log("Scan Wi-Fi");
  request.open("GET", "/wi_wi_scan_esp_set", true);
  request.send();
}

function all_save_config() {
  var GET_Server =
    "save_web_config_set?" +
    "mode_wifi=" +
    settings_mode_wifi +
    "&" +
    "wifi_ssid=" +
    settings_wifi_ssid.value +
    "&" +
    "wifi_pass=" +
    settings_wifi_pass.value +
    "&" +
    "port_tcp=" +
    settings_port_tcp.value +
    "&" +
    "mode_serial=" +
    settings_mode_serial.value +
    "&" +
    "serial_baund=" +
    settings_serial_baund.value +
    "&" +
    "dhcp=" +
    settings_dhcp.value +
    "&" +
    "static_ip=" +
    settings_static_ip.value +
    "&" +
    "static_mask=" +
    settings_static_mask.value +
    "&" +
    "static_gataway=" +
    settings_static_gataway.value;

  request.open("GET", GET_Server, true);
  request.send();
}

function reboot_esp() {
  console.log("Reboot ESP32");
  request.open("GET", "/reboot_esp_set", true);
  request.send();

  if ((settings_static_ip.value != "") & (settings_dhcp.value == 1)) {

        let delay = 7000;

    let timerId = setTimeout(function request() {
      setTimeout(() => {
        document.location.href = "http://" + settings_static_ip.value;
      }, 5000);

      timerId = setTimeout(request, delay);
    }, delay);
    
  }

  if (settings_mode_wifi == "WIFI_AP") {
    let delay = 7000;

    let timerId = setTimeout(function request() {
      setTimeout(() => {
        document.location.href = "http://192.168.1.1";
      }, 5000);

      timerId = setTimeout(request, delay);
    }, delay);
  }
}

function home_click() {
  document.location.reload();
}
