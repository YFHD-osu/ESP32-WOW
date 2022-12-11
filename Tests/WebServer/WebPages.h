const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>YFHD 網路喚醒</title>
  </head>

  <!-- 樣式 -->
  <style>
    body { 
      background-image: linear-gradient(to left, #090a09, #0c0d0d, #0f1010, #121212, #141414);
      background-attachment: fixed;
    }

    .container {
      background: rgb(66, 66, 66); 
      margin: 10px;
      display: inline-block;
      position: relative;
      border-radius:20px;
      margin: 10px;
      padding: 20px;
      min-height: auto;
      width: 300px;
      max-width: 80%;
    }

    .wakeButton {
      background-color: #4CAF50;
      color: white;
      padding: 5px 15px;
      text-align: center;
      font-size: 20px;
      margin: 4px 4px;
      cursor: pointer;
      border-radius:15px;
    }
    .wakeButton:active { background-color:green; }

    .logoutButton {
      background-color: red;
      color: white;
      border: 0px;
      padding: 5px 15px;
      text-align: center;
      font-size: 20px;
      margin: 4px 4px;
      cursor: pointer;
      border-radius:15px;
    }

    .center{
      display: flex;
      justify-content: center; 
      align-items: center;
      vertical-align: middle;
    }

    .tb{
      font: 1em sans-serif;
      text-align:center;
      border-radius: 10px;
      padding-left: 10px;
      padding-right: 10px;
      color: whitesmoke
    }

  </style>

  <body background="body" class="center">
      <div class="container">
        <h1 class="title" style="text-align: center; line-height: 0px; color: white;">可用的裝置</h1>
        <ul style="list-style-type: none; margin: 0px; padding: 0px;">
          <div class="center">
            <table cellspacing="10" cellpadding = "0">
              <colgroup style="background-color: grey;"></colgroup>
              <tr>
                <td class="tb"> YFHD-PC </td>
                <td> <li class="wakeButton" onclick="sendWakeUp(1)">喚醒</li> </td>
              </tr>
              <tr>
                <td class="tb"> YFHD-SERVER </td>
                <td> <li class="wakeButton" onclick="sendWakeUp(2)">喚醒</li> </td>
              </tr>
              <tr>
                <td class="tb"> Maggie-Hackintosh </td>
                <td> <li class="wakeButton" onclick="sendWakeUp(3)">喚醒</li> </td>
              </tr>
            </table>
          </div> 
        </ul>

        <div style="display: grid;"> 
          <button onclick="logoutButton()" class="logoutButton">登出</button>
        </div>

      </div>
    
    <script>
      function sendWakeUp(element) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/wake?device=1", true); 
        xhr.send();
      }
      
      function logoutButton() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/logout", true);
        xhr.send();
        setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
      }
    </script>

  </body>
</html>
)rawliteral";

const char login_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>YFHD 網路喚醒 - 登入</title>
  </head>

  <!-- 樣式 -->
  <style>
    body { 
      background-image: linear-gradient(to left, #090a09, #0c0d0d, #0f1010, #121212, #141414);
      background-attachment: fixed;
    }

    .container {
      background: rgb(66, 66, 66); 
      margin: 10px;
      display: inline-block;
      position: relative;
      border-radius:20px;
      margin: 10px;
      padding: 20px;
      min-height: auto;
      width: 300px;
      max-width: 80%;
    }

    .loginButton {
      background-color: green;
      color: white;
      border: 0px;
      padding: 5px 15px;
      text-align: center;
      font-size: 20px;
      margin: 4px 4px;
      cursor: pointer;
      border-radius:15px;
    }

    .center{
      display: flex;
      justify-content: center; 
      align-items: center;
      vertical-align: middle;
    }

    .input{
      font: 1em sans-serif;
      border-radius: 10px;
      border-radius: 5px;
      height: 25px;
      width: 136%;
      padding: 3px;
      margin: 0px;
      line-height: 0px;
      border: gray
    }

    .input:focus{
      outline: none;
    }

  </style>

  <body background="body" class="center">
      <div class="container">
        <h1 class="title" style="text-align: center; line-height: 0px; color: white;">登入以繼續</h1>
        
        <table cellspacing="10" cellpadding = "0">
          <tr> <th> <input name="username" placeholder="帳號" class="input" autocomplete="off"> </th> </tr>
          <tr> <th> <input name="password" placeholder="密碼" class="input" autocomplete="off" type="password"> </th> </tr>
        </table>

        <div style="display: grid;"> 
          <button onclick="loginButton()" class="loginButton">登入</button>
        </div>

      </div>
    
    <script>
      function sendWakeUp(element) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/wake?device=1", true); 
        xhr.send();
      }
      
      function loginButton() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/logout", true);
        xhr.send();
        setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
      }
    </script>

  </body>
</html>
)rawliteral";

const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <p>Logged out or <a href="/">return to homepage</a>.</p>
  <p><strong>Note:</strong> close all web browser tabs to complete the logout process.</p>
</body>
</html>
)rawliteral";
