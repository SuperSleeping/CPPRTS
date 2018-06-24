#ifndef  _WAITING_SCENE_H_
#define _WAITING_SCENE_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "UserDefault.h"
#include "network/SocketIO.h"
USING_NS_CC;

class WaitingScene : public Scene, public cocos2d::network::SocketIO::SIODelegate
{
	cocos2d::network::SIOClient *sioClient;
public:
	static cocos2d::Scene *createScene();

	virtual bool init();
	virtual void onConnect(cocos2d::network::SIOClient *client);
	virtual void onMessage(cocos2d::network::SIOClient *client, const std::string& data);
	virtual void onClose(cocos2d::network::SIOClient *client);
	virtual void onError(cocos2d::network::SIOClient *client, const std::string& data);

	CREATE_FUNC(WaitingScene);
};
#endif // ! _WAITING_SCENE_H_
