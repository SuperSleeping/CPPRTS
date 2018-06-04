var io = require('socket.io').listen(3000);
console.log('Server on port 3000.');

io.sockets.on('connection', function (socket) {
    console.log('someone connected');
    //向客户端发送消息
    socket.send('Hello Cocos2d-x');
    //注册客户端消息
    socket.on('message', function (data) {
        console.log(data);
    });

    //注册callServerEvent事件，便于客户端调用
    socket.on('callServerEvent', function (data) {
        console.log(data);
        //向客户端发送消息，调用客户端的callClientEvent事件
        socket.emit('callClientEvent', { message: 'Hello Client.' });
    });

});