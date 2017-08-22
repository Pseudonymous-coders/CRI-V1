$('#siteNav').affix({
    offset: {
        top: 100
    }
})

document.getElementById('down-btn').style.visibility = "hidden";

var coms = '<div class="row promo">' +
'    <a id="com" onclick="cmdRun()">'+
'	    <div class="col-md-4 promo-item item-1">'+
'		    <h3 id="run-btn">'+
'    		    Run'+
'    		</h3>'+
'        </div>'+
'    </a>'+
'    <a id="com" onclick="cmdInstall()">'+
'		<div class="col-md-4 promo-item item-2">'+
'			<h3 id="ins-btn">'+
'				Install'+
'			</h3>'+
'		</div>'+
'    </a>'+
'    <a id="com" onclick="cmdRemove()">'+
'		<div class="col-md-4 promo-item item-3">'+
'			<h3 id="rem-btn">'+
'				Remove'+
'			</h3>'+
'		</div>'+
'    </a>'+
'</div>'

try {
    ws = new WebSocket('ws://localhost:9098/ws');
}catch(err){
    document.getElementById('mainText').innerHTML = "CRI is down, please restart";
    document.getElementById('subText').innerHTML = "We're sorry :(";
    document.getElementById('coms').innerHTML = "";
    document.getElementById('down-btn').style.visibility = "hidden";
}

ws.onopen = function() {
    document.getElementById("mainText").innerHTML = "CRI";
    document.getElementById("subText").innerHTML = "Your new best friend";
    document.getElementById('down-btn').style.visibility = "visible";
    ws.send("connected");
    document.getElementById('coms').innerHTML = coms;

}

ws.onmessage = function(str) {
    if (str.data.substring(0,3) == "URL"){
        var BGImage = console.log(str.data.substring(3));
        document.getElementById('header').style.backgroundImage="url('"+BGImage+"')";
    } else if (str.data.substring(0, 6) == "NOROOT"){
        document.getElementById("mainText").innerHTML = "No Root Access";
        document.getElementById("subText").innerHTML = "Please Reboot";
        document.getElementById('down-btn').style.visibility = "hidden";
        document.getElementById('coms').innerHTML = "";
    }
}

ws.onclose = function() {
    document.getElementById('mainText').innerHTML = "CRI is down, please restart";
    document.getElementById('subText').innerHTML = "We're sorry :(";
    document.getElementById('coms').innerHTML = "";
    document.getElementById('down-btn').style.visibility = "hidden";
}


function cmdRun() {
    window.location = "run.html";
}

function cmdInstall() {
    window.location = "install.html";
}

function cmdRemove() {
    window.location = "remove.html";
}

function googleIt(){
    var toGoogle = document.querySelector("#gQuery").value;
    var url = "http://www.google.com/?safe=active&ssui=on#safe=strict&q="+encodeURIComponent(toGoogle.toString());
    window.location = url;
    return false;
}

function handle(e){
    if(e.keyCode === 13){
        googleIt();
    }

    return false;
}

$('#down-btn').on('click', function(e){
    e.preventDefault();
    var target= $(this).get(0).id == 'ins-btn' ? $('#down-btn') : $('#ins-btn');
    $('html, body').stop().animate({
        scrollTop: target.offset().top
    }, 1000);
});

