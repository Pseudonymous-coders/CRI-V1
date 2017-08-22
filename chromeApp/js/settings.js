$('#siteNav').affix({
    offset: {
        top: 100
    }
});

i = 0;
curVer = "0.2";
curRel = "Zooplankton";
curDat = "5/15/2016"; 
latVer = "0.25";
latRel = "Zooplankton";
latDat = "5/19/2016";
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

function notify(notifier){
    document.querySelector('.notify').innerHTML = notifier;
}

function reset() {
    setTimeout(function() {
        document.querySelector('.notify').innerHTML = "<a href='index.html'>CRI</a>";
    }, 6000);
}

function stopAll() {
    notify("Stopped all programs");
    reset();
}

function update() {
    document.getElementById('cover').style.visibility = "visible";
    upPerc();
    i = 0;
    
    
}

function upPerc () {
    setTimeout(function () {
        notify("Updating... "+i+"%");
        i++;
        if (i <= 100) {
            upPerc();
        } else {
            notify("Finalizing...");
            setTimeout(function() {
                notify("Updated");
                setTimeout(function () {
                    document.getElementById('cover').style.visibility = "hidden";
                    notify("CRI");
                }, 1000);
            }, 2000);
        }
    }, 50);
}

function remove() {
    if (confirm("Are you POSITIVE you want to remove CRI?")) {
        if (confirm("Reboot is needed for the removal of CRI to happen, reboot now?")) {
            notify("Rebooting...");
            reset();
        } else {
            notify("Please reboot to remove CRI");
            reset()
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
        if(confirm("Reboot is needed for the reinstall to take effect, reboot now?")) {
            notify("Rebooting...");
            reset();
        } else {
            notify("Please reboot to reinstall CRI");
            reset();
        }
    }
}

function addPPA() {
    PPA = document.getElementById('PPA').value;
    notify("Added PPA: "+PPA);
    reset();
}

function addDeb() {
    DEB = document.getElementById('Deb').value;
    notify("Installed Deb: "+ DEB);
    reset();
}

function doneUpdate() {
    reset();
}

/*$(".program #front-flip").click(function() {
    $(".program").toggleClass("program-active");
});

$(".danger #front-flip").click(function() {
    $(".danger").toggleClass("danger-active");
});

$(".package #front-flip").click(function() {
    $(".package").toggleClass("package-active");
});
*/
