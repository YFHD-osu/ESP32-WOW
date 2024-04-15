#ifndef WEB_PAGES_INCLUDE
#define WEB_PAGES_INCLUDE

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>YFHD 網路喚醒</title>
    
    <meta name="theme-color" content="#000000">
    <script src="https://yfhd-osu.github.io/ESP32-WOW/web/index.js"></script>
    <link rel="stylesheet" type="text/css" href="https://yfhd-osu.github.io/ESP32-WOW/web/index.css"/>
    <link rel="shortcut icon" type="image/png" href="https://yfhd-osu.github.io/ESP32-WOW/web/assets/Icon.png"/>
  </head>

  <body background="body" class="center" wi>
    <div class="container">

      <div class="title-wrapper"> 
        <img class="titleIcon" src="https://yfhd-osu.github.io/ESP32-WOW/web/assets/device.svg">
        <h1 class="titleText">可用的裝置</h1>
      </div>

      <div class="list-wrapper">
        <div id="list-viewport"> </div>
      </div>

      <div class="function-wrapper"> 
        <button onclick="refresh()" id="refresh">更新狀態</button>
        <button onclick="logout()" id="logout">登出</button>
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
    <link rel="shortcut icon" type="image/png" href="https://yfhd-osu.github.io/ESP32-WOW/web/assets/favicon.png"/>
  </head>

  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.2/css/all.min.css" integrity="sha512-SnH5WK+bZxgPHs44uWIX+LLJAJ9/2PkPKZ5QiAj6Ta86w+fsb2TkcmfRyVX3pBnMFcV7oQPJkl9QevSCWr3W6A==" crossorigin="anonymous" referrerpolicy="no-referrer" />
  <link rel="stylesheet" type="text/css" href="https://yfhd-osu.github.io/ESP32-WOW/web/login.css"/>

  <meta name="theme-color" content="#000000">

  <body>
    <div class="container">

      <div class="title-warpper">
        <img src="https://yfhd-osu.github.io/ESP32-WOW/web/assets/login_arrow.svg" width="60px" height="60px" alt="登入以繼續">
        <h1>登入以繼續</h1>
      </div>
      
      <img class="logo-wrapper" src="https://yfhd-osu.github.io/ESP32-WOW/web/assets/ArduinoLogo.png" alt="一張圖片">

      <form name="loginForm" class="form-wrapper" action="/login" method="post">
        <div>
          <input name="username" required/>
          <span id="usr"> 使用者帳號 </span>
        </div>
        
        <div>
          <!-- <input name="password" required type="password"/> -->
          <input id="password-field" type="password" class="form-control" name="password" required>
          <span id="pwd"> 密碼 </span>
        </div>
        
        <button> <i class="fa-regular fa-circle-right"></i> </button>
      </form>
      <br> <br> <br>
      <a target="_blank" href="https://github.com/YFHD-osu/ESP32-WOW"> 安全受 Dynamic Security™ 保障</a>
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
