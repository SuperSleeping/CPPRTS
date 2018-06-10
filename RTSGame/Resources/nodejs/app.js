var io = require('socket.io').listen(3000);
console.log('Server on port 3000.');

io.sockets.on('connection', function (socket) {
    console.log('someone connected');
    //注册客户端消息
    socket.on('message', function (data) {
        console.log(data);
        socket.send(data);
        socket.broadcast.send(data);
    });
});