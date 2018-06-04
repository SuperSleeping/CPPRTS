#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "network/SocketIO.h"

class Start :public cocos2d::Scene,public cocos2d::network::SocketIO::SIODelegate
{
	cocos2d::network::SIOClient *sioClient;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menureturnCallback(cocos2d::Ref *pSender);
	void menunewCallback(cocos2d::Ref *pSender);
	void menujoinCallback(cocos2d::Ref *pSender);
	void menurefreshCallback(cocos2d::Ref *pSender);
	void menubeginCallback(cocos2d::Ref *pSender);

	CREATE_FUNC(Start);

	virtual void onConnect(cocos2d::network::SIOClient *client);
	virtual void onMessage(cocos2d::network::SIOClient *client, const std::string& data);
	virtual void onClose(cocos2d::network::SIOClient *client);
	virtual void onError(cocos2d::network::SIOClient *client, const std::string& data);
};

#endif //__START_SCENE_H__