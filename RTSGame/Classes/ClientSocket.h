//SocketIO委托Client类并声明一个Client对象作为全局变量无需在场景切换时重新建立连接
#ifndef _ClientSocket_H_
#define _ClientSocket_H_

#include "cocos2d.h"
#include "network/SocketIO.h"
#include "SimpleAudioEngine.h"

class Client :public cocos2d::network::SocketIO::SIODelegate
{
	cocos2d::network::SIOClient *sioClient;
public:
	virtual void onConnect(cocos2d::network::SIOClient *client);
	virtual void onMessage(cocos2d::network::SIOClient *client, const std::string& data);
	virtual void onClose(cocos2d::network::SIOClient *client);
	virtual void onError(cocos2d::network::SIOClient *client, const std::string& data);
	virtual void buildConnection(char *hostIp);
};
//根据所给的主机Ip地址建立连接
void Client::buildConnection(char *hostIp)
{
	sioClient = cocos2d::network::SocketIO::connect(hostIp, *this);
	return;
}
//委托协议函数
void Client::onConnect(cocos2d::network::SIOClient *client)
{
	return;
}
void Client::onMessage(cocos2d::network::SIOClient *client, const std::string& data)
{
	return;
}
void Client::onClose(cocos2d::network::SIOClient *client)
{
	return;
}
void Client::onError(cocos2d::network::SIOClient *client, const std::string& data)
{
	return;
}

//声明一个client成员作为全局的socket连接对象
Client mySocket;
#endif