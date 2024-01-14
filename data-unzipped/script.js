const entries = ['home', 'settings', 'files', 'update'];
const terminal = document.getElementById('d-terminal');
const commandInput = document.getElementById('i-command');
const sendButton = document.getElementById('b-send');
const clearButton = document.getElementById('b-clear');
const statusLine = document.getElementById('o-status');
const stepSizeInput = document.getElementById('i-stepSize');
const stepSizeValue = document.getElementById('i-stepSize-value');
const feedRateInput = document.getElementById('i-feedRate');
const feedRateValue = document.getElementById('i-feedRate-value');
const spindleSpeedInput = document.getElementById('i-spindleSpeed');
const spindleSpeedValue = document.getElementById('i-spindleSpeed-value');
const resetButton = document.getElementById('b-reset');
const homeButton = document.getElementById('b-homing');
const unlockButton = document.getElementById('b-unlock');
const probeButton = document.getElementById('b-probe');
const resumeButton = document.getElementById('b-resume');
const xLeftButton = document.getElementById('b-x-left');
const xRightButton = document.getElementById('b-x-right');
const yFrontButton = document.getElementById('b-y-front');
const yBackButton = document.getElementById('b-y-back');
const zUpButton = document.getElementById('b-z-up');
const zDownButton = document.getElementById('b-z-down');
const spindleButton = document.getElementById('b-spindle');
const stopButton = document.getElementById('b-stop');

var ws = null;

function connect() {
    ws = new WebSocket("ws://" + /*location.host*/ "vevor-cnc" + "/ws");
    ws.onopen = function () {
        console.log(new Date().toISOString(), ": WebSocket open!");
    };

    ws.onmessage = function (e) {
        //console.log('Message received:', e.data);
        if (e.data[0] != '<')
            addLine(escapeHtml(e.data), false);
        else
            statusLine.innerHTML = escapeHtml(e.data);
    };

    ws.onclose = function (e) {
        console.log(new Date().toISOString(), ": WebSocket closed!");
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

function debounce(func, timeout = 300) {
    let timer;
    return (...args) => {
        clearTimeout(timer);
        timer = setTimeout(() => { func.apply(this, args); }, timeout);
    };
}

const debouncedSpindleUpdate = debounce((value) => setSpindleSpeed(value), 700);

function updateSpindleSpeed() {
    spindleSpeedValue.value = spindleSpeedInput.value;
    debouncedSpindleUpdate(spindleSpeedValue.value);
}
spindleSpeedValue.value = spindleSpeedInput.value;

function setSpindleSpeed(value) {
    spindleSpeedInput.value = value;
    spindleSpeedValue.value = value;
    sendCommand("S" + spindleSpeedValue.value);
}

commandInput.addEventListener('keydown', function (event) {
    if (event.key === 'Enter')
        addLineFromInput();
});

sendButton.addEventListener('click', addLineFromInput);
clearButton.addEventListener('click', function () { terminal.innerHTML = ""; });
resetButton.addEventListener('click', function () { sendCommand(String.fromCharCode(24) + " ;(Reset)"); });
spindleButton.addEventListener('click', function () {
    if (spindleButton.getAttribute('aria-pressed') == "true")
        sendCommand("S" + spindleSpeedValue.value + "M3");
    else
        sendCommand("M5");
});
homeButton.addEventListener('click', function () { sendCommand("$H"); });
unlockButton.addEventListener('click', function () { sendCommand("$X"); });
resumeButton.addEventListener('click', function () { sendCommand("~"); });
stopButton.addEventListener('click', function () { sendCommand("!"); });
xLeftButton.addEventListener('click', function () { sendMovement("X", true); });
xRightButton.addEventListener('click', function () { sendMovement("X"); });
yFrontButton.addEventListener('click', function () { sendMovement("Y"); });
yBackButton.addEventListener('click', function () { sendMovement("Y", true); });
zUpButton.addEventListener('click', function () { sendMovement("Z"); });
zDownButton.addEventListener('click', function () { sendMovement("Z", true); });
probeButton.addEventListener('click', function() {
    if (confirm("Please ensure that probing is prepared!\n\nStart probing?"))
        probe();
});

function probe() {
    sendCommand("M5G21G91");
    sendCommand("G38.2Z-41F100");
    sendCommand("G0Z1");
    sendCommand("G38.2Z-2F10");
    sendCommand("G1Z1.2F50");
}

function sendMovement(axis, negative = false) {
    axis = axis.toUpperCase();
    var feedRate = feedRateValue.value;
    var step = stepSizeValue.value;
    if (axis == "Z")
        step = Math.min(step, 10);
    sendCommand("$J=G91" + axis + (negative ? "-" : "") + step + "F" + feedRate);
}

function addLineFromInput() {
    sendCommand(commandInput.value);
    commandInput.value = "";
}

function sendCommand(command) {
    addLine(command);
    if (ws && ws.readyState == WebSocket.OPEN)
        ws.send(command + "\n");
    else
        addLine("*** Not connected! ***", false);
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

const escapeHtml = (unsafe) => {
    return unsafe.replaceAll('&', '&amp;').replaceAll('<', '&lt;').replaceAll('>', '&gt;').replaceAll('"', '&quot;').replaceAll("'", '&#039;');
}