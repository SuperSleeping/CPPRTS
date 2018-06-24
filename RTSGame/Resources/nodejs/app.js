var io = require('socket.io').listen(3000);
var number = 0;
var snumber = String(number);
console.log(snumber);
console.log('Server on port 3000.');

io.sockets.on('connection', function (socket) {
    console.log('someone connected');
    socket.send(String(number));
    socket.broadcast.send(String(number));
    socket.emit('numberClientEvent', String(number));
    number = number + 1;
    //console.log(number);
    //注册客户端消息
    socket.on('message', function (data) {
        console.log(data);
        socket.send(data);
        socket.broadcast.send(data);
    });
    socket.on('beginServerEvent', function (data) {
        socket.send(data);
        socket.broadcast.send(data);
    })
});