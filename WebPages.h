#ifndef WEB_PAGES_INCLUDE
#define WEB_PAGES_INCLUDE

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>YFHD 網路喚醒</title>
    <link rel="shortcut icon" type="image/png" href="favicon.png"/>
  </head>

  <link rel="stylesheet" type="text/css" href="https://yfhd-osu.github.io/ESP32-WOW/WebUI/index.css"/> <!--   Load Style -->
  <script src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/index.js"></script>
  <link rel="icon" href="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons/Icon.png" type="image/x-icon" />

  <body background="body" class="center" wi>
    <div class="container">

      <div class="titleDiv"> 
        <img class="titleIcon" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons/device.png">
        <h1 class="titleText">可用的裝置</h1>
      </div>

      <div class="deviceContainer">
        <button class="deviceCard" onclick="sendWakeUp(0)">
          <div class="deviceGrid"> 
            <img class="deviceLogo" src="Icons/Windows11Logo.png">
            <p class="deviceName"> YFHD-PC </p>
            <p class="deviceIP"> 內網: 192.168.1.110 </p>
            <span class="dot" id="status_dev_0">
          </div>
        </button>
        

        <button class="deviceCard" onclick="sendWakeUp(1)">
          <div class="deviceGrid"> 
            <img class="deviceLogo" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons/WindowsServerLogo.png">
            <p class="deviceName"> YFHD-SERVER </p>
            <p class="deviceIP"> 內網IP: 192.168.1.109 </p>
            <span class="dot" id="status_dev_1">
          </div>
        </button>

        <button class="deviceCard" onclick="sendWakeUp(2)">
          <div class="deviceGrid"> 
            <img class="deviceLogo" src="https://yfhd-osu.github.io/ESP32-WOW/WebUI/Icons/MacOsLogo.png">
            <p class="deviceName"> Maggie-Hackintosh </p>
            <p class="deviceIP"> 內網IP: 192.168.1.108 </p>
            <span class="dot" id="status_dev_2">
          </div>
        </button>

      </div>
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

const char not_found_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
  <html>
    <head><title>Not Found</title></head>
    <body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body>
  </html>
)rawliteral";

#endif