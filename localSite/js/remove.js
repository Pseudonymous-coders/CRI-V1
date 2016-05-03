var name = "";
var desc = "";
var sntnc = "";
var finals = [];
$('#siteNav').affix({
    offset: {
        top: 100
    }
})

ws = new WebSocket('ws://localhost:9097/ws')
ws.onopen = function() {
    console.log("connected");
}

ws.onmessage = function(str) {
    if (str.data.substring(0,7) == "REMOVED") {
        document.getElementById("items").innerHTML = "<h2>Removed something</h2>";
    } else if (str.data == "NONE") {
        document.getElementById('items').innerHTML = "<h2>No results found</h2>";
    } else {
        var results = str.data;
        var results = results.split("\n");
        finals = [];
        delete results[results.indexOf("")];
        for (var i=0;i<results.length-1;i++) {
            sntnc = results[i];
            result = sntnc.split("\t");
            finals[result[0]] += result[0];
            console.log(result[0])
        }
        var button = document.getElementById('btn-search');
        var txtIn = document.getElementById('text-search');
        button.style.top = "10%";
        button.style.left = "5%";
        txtIn.style.top = "10%";
        txtIn.style.left = "12%";
        document.getElementById('items').innerHTML = "";
        for (item in finals) {
            document.getElementById('items').innerHTML += "<button onclick='remove(this.id)' class='item' id='"+item+"'>"+item+"</button><br><br>";
        }
    }
}

function search() {
    document.getElementById('items').innerHTML = "";
    toSearch = document.getElementById('text-search').value;
    if (toSearch != "") {
        console.log(toSearch);
        ws.send("INSLIST"+toSearch);
        document.getElementById('items').innerHTML = "<h2> Loading...</h2>";
    } else {
        document.getElementById('items').innerHTML = "<h2> Please enter a query </h1>";
    }
}

function remove(id) {
    ws.send("REMOVE"+id);
}
