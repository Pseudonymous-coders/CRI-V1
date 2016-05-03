
// Offset for Site Navigation
$('#siteNav').affix({
    offset: {
        top: 100
    }
})

var connected = 0;

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

ws = new WebSocket('ws://localhost:9098/ws');

ws.onopen = function() {
    document.getElementById("mainText").innerHTML = "CRI";
    document.getElementById("subText").innerHTML = "Your new best friend";
    ws.send("connected");
    connected = 1;
    document.getElementById('coms').innerHTML = coms;

}

ws.onmessage = function(str) {
    if (str.data.substring(0,3) == "URL"){
        var BGImage = console.log(str.data.substring(3));
        document.getElementById('header').style.backgroundImage="url('"+BGImage+"')";

    }
}

ws.onclose = function() {
    document.getElementById('mainText').innerHTML = "CRI is down, please restart";
    document.getElementById('subText').innerHTML = "We're sorry :(";
    document.getElementById('coms').innerHTML = "";
    connected = 0;
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
