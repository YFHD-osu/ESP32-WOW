function safariHacks() {
    let windowsVH = window.innerHeight / 100;
    document.querySelector('.wrap').style.setProperty('--vh', windowsVH + 'px');
    window.addEventListener('resize', function() {
        document.querySelector('.wrap').style.setProperty('--vh', windowsVH + 'px');
    });
}

function sendWakeUp(deviceID) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", `/wake?device=${deviceID}`, true); 
    xhr.send();
}

function logout() {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", "/wake?device=1", true); 
    xhr.send();
    window.open("/logout","_self");
}

function refresh() {
    var xhr = new XMLHttpRequest();
    return;
    xhr.open("GET", "/wake?device=1", true); 
    xhr.send();
    window.open("/logout","_self");
}

safariHacks();