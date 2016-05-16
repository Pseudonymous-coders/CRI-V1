var nameProgs = [];
var progs = "";

$('#siteNav').affix({
    offset: {
        top: 100
    }
})

ws = new WebSocket('ws://localhost:9098/ws')
setTimeout(function (){
    if (ws.readyState == (0 || 3)) {
        window.location = "index.html";
    }
}, 200)

ws.onopen = function() {
    ws.send("APPLIST");
}

function progClicked(id) {
    if(confirm("Are you sure you want to remove "+id+"?")){
        ws.send("REMOVE"+id);
    }
}

ws.onmessage = function(str) {
    document.getElementById('items').innerHTML = "";
    progs = str.data;
    nameProgs = progs.split("\n");
    for (i=0;i<nameProgs.length;i++) {
        nameProgs[i] = nameProgs[i].split(" : ");
        if (nameProgs[i][2] == (null || "")) {
            nameProgs[i][2] = "defIcon";
        }
        document.getElementById('items').innerHTML += "<button onclick='progClicked(this.id)' class='item' id='"+nameProgs[i][0]+"'><img height='50' width='50' src='images/icons/"+nameProgs[i][2]+".png'><br>"+nameProgs[i][1]+"</button>";
    }
} 
