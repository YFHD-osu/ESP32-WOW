function reqWakeUp(deviceID) {
  var xhr = new XMLHttpRequest();
  xhr.open("POST", `/wake?device=${deviceID}`, true); 
  xhr.send();
}

function logout() {
  window.open("/logout", "_self");
}

function onReqDone(xhr) {
  if (xhr.readyState != XMLHttpRequest.DONE) return;
  if (xhr.status != 200) return;

  deviceStatus = xhr.response.split("");
  for (let i=0; i < deviceStatus.length; i++) {
    // console.log(parseInt(deviceStatus[i]));
    var isOnline = parseInt(deviceStatus[i]) == 1;
    var dot = document.getElementById(`status_dev_${i}`);
    dot.style.backgroundColor = isOnline ? "#68bd5a" : "#f36356";
  }
  return;
}

function refresh() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = () => onReqDone(xhr);
  xhr.open("GET", "/refresh", true);
  xhr.send();
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

function addButton({image, title, lore, id} = {}) {
  var element = document.createElement('div');
  element.innerHTML = `
  <button class="card-wrapper" onclick="reqWakeUp(${id})">
    <img src="${image}">
    <span id="name"> ${title} </span>
    <span id="ip"> ${lore} </span>
    <span name="dot" id="status-dev-${id}">
  </button>`;
  document.querySelector("#list-viewport").appendChild(element)
  return element
}

function onReqList(xhr) {
  if (xhr.readyState != XMLHttpRequest.DONE) return;
  if (xhr.status != 200) return;

  const jsonData = JSON.parse(xhr.responseText);
  for (var i = 0; i < jsonData.length; i++) {
    addButton({
      id: jsonData[i].id,
      title: jsonData[i].na,
      lore: jsonData[i].ip,
      image: jsonData[i].im
    })
  }

  // Draw shadow for column
  setShadow(document.querySelector('.list-wrapper'));

}

function main() {
  document.querySelector('.list-wrapper')
  .addEventListener('scroll', (e) => { // Listen to scroll view to add shadow
    const el = e.currentTarget;
    setShadow(el);
  });

  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = () => onReqList(xhr);
  xhr.open("GET", "/list", true);
  xhr.send();
  
  setInterval(refresh, 6000); // Refresh device status every 6s
  refresh();

}

window.onload = main;