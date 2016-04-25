var items = new Array();
$('#siteNav').affix({
    offset: {
        top: 100
    }
})

ws = new WebSocket('ws://localhost:9097/ws')
ws.onopen = function() {
}

ws.onmessage = function(str) {
    results = str.data;
    console.log(results);
    console.log(typeof(results));
} 

function search() {
    toSearch = document.getElementById('toSearch').value;
    if (toSearch != "") {
        console.log(toSearch);
        ws.send("SEARCH"+toSearch);
    }
}
