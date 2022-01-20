var request = new XMLHttpRequest();

var settings_mode_ap = document.getElementById("settings_mode_ap");
var settings_mode_sta = document.getElementById("settings_mode_sta");

var change_mode_wifi = document.getElementById("settings_mode_wifi");

change_mode_wifi.addEventListener("change", function () {
  if (change_mode_wifi.value == "WIFI_AP") {
    settings_mode_sta.style = "display:none";
    settings_mode_ap.style = "";
  }
  if (change_mode_wifi.value == "WIFI_STA") {
    settings_mode_sta.style = "";
    settings_mode_ap.style = "display:none";
  }
});

//Reboot
function button_default_reset() {
  var isAdmin = confirm("Сбросить все настройки?");
  if (isAdmin == true) {
    console.log("Default Setting");
    request.open("GET", "/default_settings_esp_set", true);
    request.send();
  }
}

//Scan WiFi
function wi_wi_scan_esp_click() {
  console.log("Scan Wi-Fi");
  request.open("GET", "/wi_wi_scan_esp_set", true);
  request.send();
}

function all_save_config() {
  var Save = confirm("Сохранить текущие натройки?");
  if (Save == true) {
    var GET_Server =
      "save_web_config_set?" +
      "mode_wifi=" +
      settings_mode_wifi.value +
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

    alert("Настройки сохранены!");

    var Reboot = confirm("Требуется перезагрузка! Перезагрузить сейчас?");
  }

  if (Reboot == true) {
    console.log("Reboot ESP32");
    request.open("GET", "/reboot_esp_set", true);
    request.send();
  }
}
