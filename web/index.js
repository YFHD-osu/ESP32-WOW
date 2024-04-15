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
  element.innerHTML = `
  <button class="card-wrapper" onclick="reqWakeUp(${res.id})">
    <img src="${res.image}" alt="Custom wake on lan device logo">
    <span id="name"> ${res.title} </span>
    <span id="ip"> ${res.lore} </span>
    <span name="dot" id="status-dev-${res.id}">
  </button>`;
  
  return element
}

function onReqList(xhr) {
  if (xhr.readyState != XMLHttpRequest.DONE) return;
  if (xhr.status != 200) return;

  const jsonData = JSON.parse(xhr.responseText);
  var viewPort = document.querySelector("#list-viewport");
  viewPort.innerHTML = "";
  for (var i = 0; i < jsonData.length; i++) {
    var html = getButton({res: jsonData[i]})
    var isOnline = parseInt(jsonData[i].st) == 1;
    var dot = html.getElementById(`status-dev-${i}`);
    dot.style.backgroundColor = isOnline ? "#68bd5a" : "#f36356";
    viewPort.appendChild(html)
  }

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