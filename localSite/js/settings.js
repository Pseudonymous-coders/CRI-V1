$('#siteNav').affix({
    offset: {
        top: 100
    }
});

ws = new WebSocket('ws://localhost:9098/ws');

document.querySelector('#version #versions').style.visibility = "hidden";

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
        document.querySelector('#version #versions').style.visibility = "visible";
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

    }else if (str.data == "NOROOT") {
        window.location = "index.html";
    }else if (str.data.substring(0, 4) == "PERC") {
        perc = str.data.substring(4);
        notify("Updating... "+perc+"%");

    } else if (str.data.substring(0, 10) == "DONEUPDATE") {
        notify("Finalizing...");
        document.querySelector('#cover').style.visibility = "hidden";
        setTimeout(function() {
            notify("Done installing, refreshing..");
            setTimeout(function() {
                location.reload();
            }, 1000);
        }, 6000);

    } else if (str.data.substring(0, 10) == "FAILUPDATE") {
        notify("Error updating...");
    } else if (str.data.substring(0, 7) == "PPADONE") {
        notify("Added PPA");
        reset();
    } else if (str.data.substring(0, 7) == "DEBDONE") {
        notify("Installed Deb");
        reset();
    }
}

ws.onclose = function() {
    window.location = "index.html";
    document.getElementById('subText').innerHTML = "We're sorry :(";
    document.querySelector('.notify').innerHTML = "CRI is down";
}

function notify(notifier){
    document.querySelector('.notify').innerHTML = notifier;
}

function reset() {
    setTimeout(function() {
        document.querySelector('.notify').innerHTML = "CRI";
    }, 6000);
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
    if (confirm("Are you POSITIVE you want to remove CRI?")) {
        ws.send("CRIMOVE");
        if (confirm("Reboot is needed for the removal of CRI to happen, reboot now?")) {
            notify("Rebooting...");
            setTimeout(function(){
                ws.send("REBOOT");
            }, 2000);
        } else {
            notify("Please reboot to remove CRI");
        }
    } else {
        notify("Not removing CRI");
        setTimeout(function(){
            notify("Phew! Close one huh?");
        }, 2000);
        reset();
    }
}

function reinstall() {
    if (confirm("Are you POSITIVE you want to reinstall CRI?")) {
        ws.send("CRINSTALL");
        if(confirm("Reboot is needed for the reinstall to take effect, reboot now?")) {
            notify("Rebooting...");
            setTimeout(function(){
                ws.send("REBOOT");
            }, 2000);
        } else {
            notify("Please reboot to reinstall CRI");
        }
    }
}

function addPPA() {
    PPA = document.getElementById('PPA').value;
    ws.send("ADDPPA"+PPA);
}

function addDeb() {
    PPA = document.getElementById('Deb').value;
    ws.send("ADDDEB"+PPA);
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

$(".danger #front-flip").click(function() {
    $(".danger").toggleClass("danger-active");
});

$(".package #front-flip").click(function() {
    $(".package").toggleClass("package-active");
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
