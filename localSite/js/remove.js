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
    if (confirm("Are you sure you want to uninstall "+id+"?")) {
        ws.send("REMOVE"+id);
    }
}

ws.onmessage = function(str) {
    progs = str.data;
    nameProgs = progs.split("%%SP%%");
    for (i=0;i<nameProgs.length;i++) {
        document.getElementById('apps').innerHTML += "<button onclick='progClicked(this.id)' id='"+nameProgs[i]+"'>"+nameProgs[i]+"</button>";
    }
} 
