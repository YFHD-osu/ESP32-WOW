var lastList = [];

String.prototype.hashCode = function() {
  var hash = 0,
    i, chr;
  if (this.length === 0) return hash;
  for (i = 0; i < this.length; i++) {
    chr = this.charCodeAt(i);
    hash = ((hash << 5) - hash) + chr;
    hash |= 0; // Convert to 32bit integer
  }
  return hash;
}

function reqWakeUp(deviceID) {
  var xhr = new XMLHttpRequest();
  xhr.open("POST", `/wake?device=${deviceID}`, true); 
  xhr.send();
}

function logout() {
  window.open("/logout", "_self");
}

function setShadow(el) {
  const isScrollable = el.scrollHeight > el.clientHeight;
  
  // GUARD: If element is not scrollable, remove all classes
  if (!isScrollable) {
    el.classList.remove('is-bottom-overflowing', 'is-top-overflowing');
    return;
  }
  
  // Otherwise, the element is overflowing!
  // Now we just need to find out which direction it is overflowing to (can be both).
  // One pixel is added to the height to account for non-integer heights.
  const isScrolledToBottom = el.scrollHeight < el.clientHeight + el.scrollTop + 1;
  const isScrolledToTop = isScrolledToBottom ? false : el.scrollTop === 0;
  el.classList.toggle('is-bottom-overflowing', !isScrolledToBottom);
  el.classList.toggle('is-top-overflowing', !isScrolledToTop);
}

function getButton({res} = {}) {
  var element = document.createElement('div');
  var isOnline = parseInt(res.st) == 1;
  var color = isOnline ? "#68bd5a" : "#f36356";
  element.innerHTML = `
  <button class="card-wrapper" onclick="reqWakeUp(${res.id})">
    <img src="${res.im}" alt="Custom wake on lan device logo">
    <span id="name"> ${res.na} </span>
    <span id="ip"> ${res.ip} </span>
    <span name="dot" id="status-dev-${res.id}" style="background-color: ${color}">
  </button>`;
  
  return element
}

function onReqList(xhr) {
  if (xhr.readyState != XMLHttpRequest.DONE) return;
  if (xhr.status != 200) return;

  const jsonData = JSON.parse(xhr.responseText);
  var viewPort = document.querySelector("#list-viewport");
  if (lastList == viewPort.innerHTML) {

  }

  for (var i = 0; i < jsonData.length; i++) {
    var html = getButton({res: jsonData[i]})
    if (html.innerHTML != (lastList[i] == null) ? "" : lastList[i].innerHTML) {
      if (viewPort.children[i] == null) {
        viewPort.appendChild(html);
      } else {
        viewPort.children[i] = html;
      }
      lastList[i] = html;
    }
    
    viewPort.insertBefore(html)
  }

  lastList = viewPort.innerHTML;

  // Draw shadow for column
  setShadow(document.querySelector('.list-wrapper'));
}

function main() {
  // Listen to scroll view to add shadow
  document.querySelector('.list-wrapper')
  .addEventListener('scroll', (e) => { 
    const el = e.currentTarget;
    setShadow(el);
  });
  
  setInterval(refresh, 6000); // Refresh device status every 6s
  refresh();

  testFunction()
}

function refresh() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = () => onReqList(xhr);
  xhr.open("GET", "/list", true);
  xhr.send();
}

function testFunction() {
  if (location.hostname != "127.0.0.1") return;
  console.log("Localhost detected! start test mode");

}

window.onload = main;