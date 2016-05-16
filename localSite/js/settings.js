$('#siteNav').affix({
    offset: {
        top: 100
    }
})

ws = new WebSocket('ws://localhost:9098/ws');

setTimeout(function (){
    if (ws.readyState == (0 || 3)) {
        window.location = "index.html";
    }
}, 200)

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
    }else if (str.data.substring(0, 8) == "UPDATING") {
        console.log("updating");
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
    document.getElementById('cover').style.visibility = "visible";
    document.getElementById('update-stat').style.visibility = "visible";
    display = document.querySelector('#timer');
    startTimer(4*60, display);
    ws.send("UPDATE");
}

function addPPA() {
    PPA = document.getElementById('addPPA').value;
    ws.send("ADDPPA"+PPA);
}

$(".cool").click(function() {
    $(".cool").toggleClass("cool-active");
});
$(".danger").click(function() {
    $(".danger").toggleClass("danger-active");
});

function startTimer(duration, display) {
    var timer = duration, minutes, seconds;
    setInterval(function () {
        minutes = parseInt(timer / 60, 10);
        seconds = parseInt(timer % 60, 10);

        minutes = minutes < 10 ? "0" + minutes : minutes;
        seconds = seconds < 10 ? "0" + seconds : seconds;

        display.textContent = minutes + ":" + seconds;

        if (--timer < 0) {
            timer = duration;
        }
    }, 1000);
}

