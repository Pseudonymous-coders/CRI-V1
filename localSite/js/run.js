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
        if (nameProgs[i].substring(0,7) == ("gnome-" || "unity-")) {
            continue;
        } else {
            document.getElementById('items').innerHTML += "<button onclick='progClicked(this.id)' class='item' id='"+nameProgs[i]+"'>"+nameProgs[i]+"</button>";
        }
    }
} 
