var WebSocket = require('websocket').server;
var http = require('http');

var server = http.createServer(function(req,res){

});

server.listen(3535, function() {

});

wsServer = new WebSocket({
    httpServer: server
});


wsServer.on('connect', function(){
    console.log("Connected with client");
})


wsServer.on('request', function(req) {
    var connection = req.accept(null, req.origin);

    connection.on('message', function(message) {
        if (message.type === 'utf8') {
            var data = message.utf8Data;
            var obj = JSON.parse(data);
            console.log(obj);

            if (obj.exec) {
                startapp = {
                    'startapp':obj.exec[1]
                };
                startapp = JSON.stringify(startapp);
                console.log(startapp);
                connection.sendUTF(startapp);
            }

        }
    });

});
