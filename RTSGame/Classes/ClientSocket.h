//SocketIOί��Client�ಢ����һ��Client������Ϊȫ�ֱ��������ڳ����л�ʱ���½�������
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
//��������������Ip��ַ��������
void Client::buildConnection(char *hostIp)
{
	sioClient = cocos2d::network::SocketIO::connect(hostIp, *this);
	return;
}
//ί��Э�麯��
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

//����һ��client��Ա��Ϊȫ�ֵ�socket���Ӷ���
Client mySocket;
#endif