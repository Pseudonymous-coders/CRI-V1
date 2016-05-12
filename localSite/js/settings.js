$('#siteNav').affix({
    offset: {
        top: 100
    }
})

ws = new WebSocket('ws://localhost:9098/ws')
ws.onopen = function() {
    ws.send("VER")
}

ws.onmessage = function(str) {
    if (str.data.substring(0,3) == "VER") {
        var versions = str.data.substring(3)
        console.log(versions);
    }
}

function stopAll() {
    ws.send("STOPALL");
}

function update() {
    ws.send("UPDATE");
}

function addPPA() {
    PPA = document.getElementById('addPPA').value;
    ws.send("ADDPPA"+PPA);
}
