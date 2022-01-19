var select_serial_config = document.getElementById("select_serial_config");
//Reboot сохранить
function reboot_esp_click() {
  var isAdmin = confirm("Подтвердите перезагрузку?");
  if (isAdmin == true) {
    console.log("Reboot ESP32");
    request.open("GET", "/reboot_esp_set", true);
    request.send();
  }
}

//Scan WiFi
function wi_wi_scan_esp_click() {
  console.log("Scan Wi-Fi");
  request.open("GET", "/wi_wi_scan_esp_set", true);
  request.send();
}


function all_save_config()
{
    select_serial_config.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            select_serial_config.innerHTML = this.responseText;
        }
    };
    request.open("GET", "web_settings_set?serial_baund=" + select_serial_config.value + "&apikey=12345", true);
    request.send();
  console.log("Select Effect Front: " + select_serial_config.value);
  

  
    
}









