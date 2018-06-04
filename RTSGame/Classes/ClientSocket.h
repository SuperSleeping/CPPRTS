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
};

//ί��Э�麯��
void onConnect(cocos2d::network::SIOClient *client)
{

}
void onMessage(cocos2d::network::SIOClient *client, const std::string& data)
{

}
void onClose(cocos2d::network::SIOClient *client)
{

}
void onError(cocos2d::network::SIOClient *client, const std::string& data)
{

}

//����һ��client��Ա
Client mySocket;
#endif