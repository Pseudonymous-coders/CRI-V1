items = new Array(); 
var name = "";
var desc = "";
var sntnc = "";
$('#siteNav').affix({
    offset: {
        top: 100
    }
})

document.getElementById('items').style.visibility = "hidden";

ws = new WebSocket('ws://localhost:9098/ws')

setTimeout(function (){
    if (ws.readyState == (0 || 3)) {
        window.location = "index.html";
    }
}, 200)

ws.onopen = function() {
}

ws.onmessage = function(str) {
    if (str.data == "DONEINSTALL") {
        document.querySelector('.notify').innerHTML = "Installed";
        setTimeout(function() {
            document.querySelector('.notify').innerHTML = "<a href='index.html'>CRI</a>";
        }, 3000);
    } else {
        if (str.data.split("\n").length > 2000){
            document.getElementById('items').innerHTML = "";
            document.getElementById('items').innerHTML += "<h2>Please be more specific, too many results</h2>";
        } else {
        
            var results = str.data;
            var results = results.split("\n");
            items = [];
            delete results[results.indexOf("")];
            for (var i=0;i<results.length-1;i++) {
                sntnc = results[i];
                result = sntnc.split(" - ");
                name = result[0];
                desc = result[1];
                items[name] = desc;
            }
            var button = document.getElementById('btn-search');
            var txtIn = document.getElementById('text-search');
            button.style.top = "10%";
            button.style.left = "5%";
            txtIn.style.top = "10%";
            txtIn.style.left = "12%";
            for (item in items) {
                document.getElementById('items').innerHTML += "<button onclick='install(this.id)' class='item' id='"+item+"'>"+item+" -- "+items[item]+"</button><br><br>";
            }
        }
    } 
}

function search() {
    document.getElementById('items').innerHTML = "";
    toSearch = document.getElementById('text-search').value;
    if (toSearch != "") {
        ws.send("SEARCH"+toSearch);
        console.log(toSearch);
    }
    document.getElementById('items').style.visibility = "visible";
}

function install(name) {
    if (confirm("Are you sure if you want to install "+name+"?")) {
        ws.send("INSTALL"+name);
        document.querySelector('.notify').innerHTML = "Installing "+name;
    }
}

$('#text-search').keypress(function (e) {
    var code = e.keyCode || e.which;
    if (code === 13) {
        search();
    };
});
