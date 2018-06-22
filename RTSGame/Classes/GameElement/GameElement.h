#ifndef _GAME_ELEMENT_H_
#define _GAME_ELEMENT_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;

//@屏幕坐标转换成层坐标（世界坐标系）
extern Point convertToMapLayer(Point position);
//@世界坐标转换成瓦片地图坐标
extern Point convertToTiledMap(Point position);
//@瓦片转换成世界坐标
extern Point convertFromTMToWorld(Point position);
//@世界坐标换成相邻的瓦片坐标
extern Point convertToNeightborTiledMap(Point position);
//判断目标是否超出瓦片地图的范围
extern bool isTileOutOfRange(Point tmPoint);

class GameElement : public Sprite
{
public:
	//创建GameElement的函数
	static GameElement* create(const char* filename, Vec2 position);

	//绑定touch监听器
	EventListenerTouchOneByOne * touchListener;

	//属性
	bool selected = 0;
	int team;
	int prize;
	int blood;

	//血条
	Sprite* bloodFrame;
	Sprite* bloodPicture;

	//位置
	Vec2 positionCurrent;
	Vec2 positionCurrentTM;
	void setPositionCurrent(Point positionWorld)
	{
		positionCurrent = positionWorld;
		positionCurrentTM = convertToTiledMap(positionWorld);
	}

	//选择设置
	void setSelected(bool isSelected)
	{
		selected = isSelected;
	}

	//重载setPosition
	virtual void setPosition(const Vec2& pos);

	Point convertToTM(Point position)
	{
		Point tmSize = Vec2(48,24);
		Point mapSize = Vec2(5664, 3312);
		int x, y;
		x = position.x / tmSize.x;
		y = (mapSize.y - position.y) / tmSize.y;
		return Vec2(x, y);
	}

};

#endif