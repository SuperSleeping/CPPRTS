var io = require('socket.io').listen(3000);
console.log('Server on port 3000.');

io.sockets.on('connection', function (socket) {
    console.log('someone connected');
    //��ͻ��˷�����Ϣ
    socket.send('Hello Cocos2d-x');
    //ע��ͻ�����Ϣ
    socket.on('message', function (data) {
        console.log(data);
    });

    //ע��callServerEvent�¼������ڿͻ��˵���
    socket.on('callServerEvent', function (data) {
        console.log(data);
        //��ͻ��˷�����Ϣ�����ÿͻ��˵�callClientEvent�¼�
        socket.emit('callClientEvent', { message: 'Hello Client.' });
    });

});