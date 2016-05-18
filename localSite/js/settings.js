$('#siteNav').affix({
    offset: {
        top: 100
    }
});

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
        curVer = current.match("VERSION(.*?)ENDVERSION").pop();
        curRel = current.match("NAME(.*?)ENDNAME").pop();
        curDat = current.match("DATE(.*?)ENDDATE").pop();
        latVer = latest.match("VERSION(.*?)ENDVERSION").pop();
        latRel = latest.match("NAME(.*?)ENDNAME").pop();
        latDat = latest.match("DATE(.*?)ENDDATE").pop();
        document.querySelector('#cur #ver').innerHTML = curVer;
        document.querySelector('#cur #rel').innerHTML = curRel;
        document.querySelector('#cur #dat').innerHTML = curDat;
        document.querySelector('#lat #ver').innerHTML = latVer;
        document.querySelector('#lat #rel').innerHTML = latRel;
        document.querySelector('#lat #dat').innerHTML = latDat;

        if (curVer != latVer) {
            document.querySelector('#lat').style.visibility = "visible";
            document.querySelector('#update').style.visibility = "visible";
        }

    }else if (str.data.substring(0, 4) == "PERC") {
        perc = str.data.substring(4);
        document.querySelector('.notify').innerHTML = "Updating... "+perc+"%";

    } else if (str.data.substring(0, 10) == "DONEUPDATE") {
        document.querySelector('.notify').innerHTML = "Finalizing...";
        document.querySelector('#cover').style.visibility = "hidden";
        setTimeout(function() {
            document.querySelector('.notify').innerHTML = "Done installing, refreshing..";
            setTimeout(function() {
                location.reload();
            }, 2000);
        }, 6000);

    } else if (str.data.substring(0, 10) == "FAILUPDATE") {
        document.querySelector('.notify').innerHTML = "Error updating...";
    }
}

ws.onclose = function() {
    document.getElementById('subText').innerHTML = "We're sorry :(";
    document.querySelector('.notify').innerHTML = "CRI is down";
}

function stopAll() {
    ws.send("STOPALL");
}

function update() {
    document.getElementById('cover').style.visibility = "visible";
    display = document.querySelector('.notify');
    //startTimer(0.3*60, display);
    ws.send("UPDATE");
}

function remove() {
    console.log("removing");
    if (confirm("Are you POSITIVE you want to remove CRI?")) {
        ws.send("CRIMOVE");
    }
}

function addPPA() {
    PPA = document.getElementById('addPPA').value;
    ws.send("ADDPPA"+PPA);
}

function doneUpdate() {
    document.querySelector('.notify').innerHTML = "Done Updating, please refresh";
    document.getElementById('cover').style.visibility = "hidden";
    setTimeout( function(){
        document.querySelector('.notify').innerHTML = "<a href='index.html'>CRI</a>"
    }, 3000);
}

$(".program #front-flip").click(function() {
    $(".program").toggleClass("program-active");
});
$(".program #back-flip").click(function(event) {
    event.stopPropagation();
    if (event.target.id == "back"){
        $(".program").toggleClass("program-active");
    }
});


$(".danger").click(function() {
    $(".danger").toggleClass("danger-active");
});
$(".danger #back").click(function(event) {
    $(".danger-active").toggleClass("danger");
});


$(".package #front").click(function() {
    $(".package").toggleClass("package-active");
});
$(".package #back").click(function(event) {
    event.stopPropagation();
    if (event.target.id == "back"){
        $(".package").toggleClass("package-active");
    }
});

/*
function startTimer(duration, display) {
    var timer = duration, minutes, seconds;
    time = setInterval(function () {
        minutes = parseInt(timer / 60, 10);
        seconds = parseInt(timer % 60, 10);

        minutes = minutes < 10 ? "0" + minutes : minutes;
        seconds = seconds < 10 ? "0" + seconds : seconds;

        display.textContent = minutes + ":" + seconds;

        if (--timer < 0) {
            clearInterval(time);
            doneUpdate();
        }
    }, 1000);
}*/
