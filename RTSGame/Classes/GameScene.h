#ifndef _GAME_SCENE_
#define _GAME_SCENE_

#include "cocos2d.h"
#include "network/SocketIO.h"
USING_NS_CC;

class GameScene :public Scene,public cocos2d::network::SocketIO::SIODelegate
{
	cocos2d::network::SIOClient *sioClient;
public:
	static Scene* createScene();
	virtual bool init();

	virtual void onConnect(cocos2d::network::SIOClient *client);
	virtual void onMessage(cocos2d::network::SIOClient *client, const std::string& data);
	virtual void onClose(cocos2d::network::SIOClient *client);
	virtual void onError(cocos2d::network::SIOClient *client, const std::string& data);

	void sendCallback(Ref *pSender);

	void chatResponse(const std::string& data);

	CREATE_FUNC(GameScene);
};
#endif