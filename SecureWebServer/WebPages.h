const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>YFHD 網路喚醒</title>
  </head>

  <link rel="stylesheet" type="text/css" href="https://yfhd-osu.github.io/ESP32-WOW/WebUI/index.css"/> <!--   Load Style -->
  <script src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/index.js"></script>

  <body background="body" class="center">
    <div class="container">

      <div class="titleGrid" onclick="sendWakeUp(0)"> 
        <img class="titleIcon" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons\device.png">
        <h1 class="titleText" style="text-align: center; line-height: 0px; color: white;">可用的裝置</h1>
      </div>
      
      <button class="deviceContainer">
        <div class="deviceGrid"> 
          <img class="deviceLogo" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons\Windows11Logo.png">
          <p class="deviceName"> YFHD-PC </p>
          <p class="deviceIP"> 內網IP: 192.168.1.110 </p>
          <img class="deviceStatusImage" id="deviceImg0" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons\pendingDot.png">
          <p class="deviceStatusText" id="deviceStatus0"> 連線中... </p>
        </div>
      </button>

      <button class="deviceContainer" onclick="sendWakeUp(1)">
        <div class="deviceGrid"> 
          <img class="deviceLogo" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons\WindowsServerLogo.png">
          <p class="deviceName"> YFHD-SERVER </p>
          <p class="deviceIP"> 內網IP: 192.168.1.109 </p>
          <img class="deviceStatusImage" id="deviceImg1" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons\pendingDot.png">
          <p class="deviceStatusText" id="deviceStatus1"> 連線中... </p>
        </div>
      </button>

      <button class="deviceContainer" onclick="sendWakeUp(2)">
        <div class="deviceGrid"> 
          <img class="deviceLogo" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons\MacOsLogo.png">
          <p class="deviceName"> Maggie-Hackintosh </p>
          <p class="deviceIP"> 內網IP: 192.168.1.108 </p>
          <img class="deviceStatusImage" id="deviceImg2" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons\pendingDot.png">
          <p class="deviceStatusText" id="deviceStatus2"> 連線中... </p>
        </div>
      </button>

      <div class="functionContainer"> 
        <button onclick="refresh()" class="refreshButton">更新狀態</button>
        <button onclick="logout()" class="logoutButton">登出</button>
      </div>
    </div>
  </body>

</html>
)rawliteral";

const char login_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>YFHD 網路喚醒 - 登入</title>
  </head>

  <link rel="stylesheet" type="text/css" href="https://yfhd-osu.github.io/ESP32-WOW/WebUI/login.css"/> <!--   Load Style -->

  <body background="body" class="center">
      <div class="container">

        <div class="loginTitleGrid"> 
          <img class="loginImage" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons\LoginArrowDark.png" width="60px" height="60px" alt="登入以繼續">
          <h1 class="loginText">登入以繼續</h1>
        </div>

        <div class="loginFormGrid">
          <img class="arduinoLogo" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons\ArduinoLogo.png" alt="一張圖片">
          <form name="loginForm" class="loginForm" action="/login" method="post">
            <input name="username" placeholder="帳號" class="usernameInput" autocomplete="off" autocorrect="off" spellcheck="false" aria-required="true" required="required" id="usernameInputBox"/>
            <input name="password" placeholder="密碼" class="passwordInput" autocomplete="off" autocorrect="off" spellcheck="false" aria-required="true" required="required" type="password" id="passwordInputBox"/>
            <input name="submitButton" type="submit" value="登入" class="loginButton" title="登入" /> <!-- onclick="submitForm()" -->
          </form>
          <a href="https://google.com" class="dynamicSecurity"> 安全受 Dynamic Security™ 保障</a>
        </div>

      </div>
  </body>

</html>
)rawliteral";