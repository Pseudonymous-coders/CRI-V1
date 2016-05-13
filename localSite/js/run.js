var nameProgs = [];
var progs = "";

$('#siteNav').affix({
    offset: {
        top: 100
    }
})

ws = new WebSocket('ws://localhost:9098/ws')
ws.onopen = function() {
    ws.send("APPLIST");
}

function progClicked(id) {
    ws.send("RUN"+id);
}

ws.onmessage = function(str) {
    progs = str.data;
    nameProgs = progs.split("\n");
    for (i=0;i<nameProgs.length;i++) {
        nameProgs[i] = nameProgs[i].split(" : ");

        document.getElementById('items').innerHTML += "<img height='50' width='50' src='images/icons/"+nameProgs[i][2]+".png'><button onclick='progClicked(this.id)' class='item' id='"+nameProgs[i][0]+"'>"+nameProgs[i][1]+"</button>";
    }
} 
