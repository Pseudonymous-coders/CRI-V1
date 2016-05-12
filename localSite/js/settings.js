$('#siteNav').affix({
    offset: {
        top: 100
    }
})

try {
    ws = new WebSocket('ws://localhost:9098/ws');
}catch(err){
    document.getElementById('mainText').innerHTML = "CRI is down";
    document.getElementById('subText').innerHTML = "We're sorry :(";
}

ws.onopen = function() {
    document.getElementById('mainText').innerHTML = "";
    document.getElementById('subText').innerHTML = "";
    ws.send("VER")
}

ws.onmessage = function(str) {
    if (str.data.substring(0,3) == "VER") {
        var versions = str.data.substring(3);
        versions = versions.split("\n");
        current = versions[0];
        latest = versions[2];
        console.log(current);
        curVer = current.match("VERSION(.*?)ENDVERSION").pop();
        curRel = current.match("NAME(.*?)ENDNAME").pop();
        curDat = current.match("DATE(.*?)ENDDATE").pop();
        latVer = latest.match("VERSION(.*?)ENDVERSION").pop();
        latRel = latest.match("NAME(.*?)ENDNAME").pop();
        latDat = latest.match("DATE(.*?)ENDDATE").pop();
        document.getElementById('ver').innerHTML = curVer;
        document.getElementById('rel').innerHTML = curRel;
        document.getElementById('dat').innerHTML = curDat;
    }
}

ws.onclose = function() {
    document.getElementById('mainText').innerHTML = "CRI is down";
    document.getElementById('subText').innerHTML = "We're sorry :(";
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
