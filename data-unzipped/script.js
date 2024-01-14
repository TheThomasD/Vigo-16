const entries = ['home', 'settings', 'files', 'update'];
const terminal = document.getElementById('d-terminal');
const commandInput = document.getElementById('i-command');
const sendButton = document.getElementById('b-send');
const stepSizeInput = document.getElementById('i-stepSize');
const stepSizeValue = document.getElementById('i-stepSize-value');
const feedRateInput = document.getElementById('i-feedRate');
const feedRateValue = document.getElementById('i-feedRate-value');
const spindleSpeedInput = document.getElementById('i-spindleSpeed');
const spindleSpeedValue = document.getElementById('i-spindleSpeed-value');

var ws = null;

function connect() {
    ws = new WebSocket("ws://" + /*location.host*/ "vevor-cnc" + "/ws");
    ws.onopen = function () {
        console.log("WebSocket ist mit dem " + ws.protocol + " Protokoll geöffnet");
        ws.send("Hello Server!");
    };

    ws.onmessage = function (e) {
        console.log('Message received:', e.data);
        addLine(e.data, false);
    };

    ws.onclose = function (e) {
        console.log("WebSocket wurde geschlossen");
        setTimeout(function () {
            connect();
        }, 1000);
    };

    ws.onerror = function (err) {
        console.error('Socket encountered error: ', err.message, 'Closing socket');
        ws.close();
    };
}

connect();

function updateStepSize() {
    stepSizeValue.value = stepSizeInput.value;
}
updateStepSize();

function setStepSize(value) {
    stepSizeInput.value = value;
    updateStepSize();
}

function updateFeedRate() {
    feedRateValue.value = feedRateInput.value;
}
updateFeedRate();

function setFeedRate(value) {
    feedRateInput.value = value;
    updateFeedRate();
}

function updateSpindleSpeed() {
    spindleSpeedValue.value = spindleSpeedInput.value;
}
updateSpindleSpeed();

function setSpindleSpeed(value) {
    spindleSpeedInput.value = value;
    updateSpindleSpeed();
}

commandInput.addEventListener('keydown', function (event) {
    if (event.key === 'Enter')
        addLineFromInput();
});

sendButton.addEventListener('click', addLineFromInput);

function addLineFromInput() {
    addLine(commandInput.value);
    if (ws && ws.readyState == WebSocket.OPEN)
        ws.send(commandInput.value);
    else
        addLine("*** Not connected! ***", false);
    commandInput.value = "";
}

entries.forEach(entry => {
    document.getElementById('b-' + entry).addEventListener('click', function () { show(entry); });
});

function show(entry) {
    entries.forEach(entry => {
        document.getElementById('d-' + entry).classList.add('d-none');
        const buttonClassList = document.getElementById('b-' + entry).classList;
        buttonClassList.remove('active');
        buttonClassList.add('link-body-emphasis');
    });
    document.getElementById('d-' + entry).classList.remove('d-none');
    const buttonClassList = document.getElementById('b-' + entry).classList;
    buttonClassList.add('active');
    buttonClassList.remove('link-body-emphasis');
}

function addLine(line, isSent = true) {
    if (line) {
        if (isSent) terminal.innerHTML += '&gt;'
        terminal.innerHTML += line + '<br>';
        terminal.scrollTop = terminal.scrollHeight; // scroll to bottom
    }
}