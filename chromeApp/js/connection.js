var host = 'localhost'
var port = 3535
var address = host+":"+port
var connected

console.log(address);

var ws = new WebSocket('ws://'+address);

ws.onopen = function() {
    console.log("Connected at "+address);
    connected = true;
}

execApp = {
    'exec': ['run', 'xterm']
}

if (connected == true){
    ws.send(execApp);
}
ws.onclose = function() {
    console.log('Connection closed with '+address)
}
