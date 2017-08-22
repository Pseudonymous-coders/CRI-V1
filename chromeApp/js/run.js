$('#siteNav').affix({
    offset: {
        top: 100
    }
})

setTimeout(function(){
    if (connected != true) {
        document.getElementById('items').innerHTML = 'NOT CONNECTED';
    }
}, 200);


ws.onmessage = function(message){
    obj = message.data;
    if (obj.startapp){
        var id = obj.startapp;
        console.log(id);
        document.querySelector('.notify').innerHTML = "Running "+id;
        setTimeout(function() {
            document.querySelector('.notify').innerHTML = "<a href='index.html'>CRI</a>";
        }, 3000);
    }

}


function progClicked(id) {
    
    var obj = {
        'exec': ['run', id]
    };

    ws.send(JSON.stringify(obj));
    
}

progName = "Test App"
iconName = "http://static.appstore.vn/a/uploads/thumbnails/122015/soul-icon-pack_icon.png"

document.getElementById('items').innerHTML += "<button onclick='progClicked(this.id)' class='item' id='"+progName+"'><img height='50' width='50' src='"+iconName+"'><br>"+progName+"</button>";

