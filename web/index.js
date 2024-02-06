function sendWakeUp(deviceID) {
  var xhr = new XMLHttpRequest();
  xhr.open("POST", `/wake?device=${deviceID}`, true); 
  xhr.send();
}

function logout() {
  window.open("/logout","_self");
}

function refresh() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/refresh?r=0", true); 
  xhr.timeout = 10000;
  xhr.withCredentials = true;
  xhr.send();

  xhr.onreadystatechange=()=>{
    if(xhr.readyState != XMLHttpRequest.DONE || xhr.status != 200) {return;}
    deviceStatus = xhr.response.split("");
    for (let i=0; i < deviceStatus.length; i++) {
      console.log(parseInt(deviceStatus[i]));
      var isOnline = parseInt(deviceStatus[i]) == 1;
      var dot = document.getElementById(`status_dev_${i}`);
      dot.style.backgroundColor = isOnline ? "#68bd5a" : "#f36356";

      document.getElementById(`deviceImg${i}`).src = 
      (parseInt(deviceStatus[i]) == 1) ? "https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons/onlineDot.png" : "https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons\\offlineDot.png";
    }
    return;
  }
}

window.onload = function() {
  refresh();
};

setInterval(function(){refresh()},6000);
