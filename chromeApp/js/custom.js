$('#siteNav').affix({
    offset: {
        top: 100
    }
})

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

document.getElementById("mainText").innerHTML = "CRI";
document.getElementById("subText").innerHTML = "Your new best friend";
document.getElementById('down-btn').style.visibility = "visible";
document.getElementById('coms').innerHTML = coms;


function cmdRun() {
    window.location = "run.html";
}

function cmdInstall() {
    window.location = "install.html";
}

function cmdRemove() {
    window.location = "remove.html";
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

