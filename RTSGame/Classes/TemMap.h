#ifndef	 _TEM_MAP_SCENE_
#define _TEM_MAP_SCENE_

#include "cocos2d.h"

USING_NS_CC;

class temmap:public Scene
{
public:
	static Scene* createScene();
	virtual bool init();

	//menu
	void pauseCallback(Ref* pSender);
	void tipCallback(Ref* pSender);
	
	//unsure
	void buildingCallback(Ref* pSender);
	void soldierCallback(Ref* pSender);
	void tankCallback(Ref* pSender);

	CREATE_FUNC(temmap);
};
#endif
