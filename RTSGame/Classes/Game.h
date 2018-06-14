#ifndef _GAME_H_
#define _GAME_H_


#include "cocos2d.h"
#include <vector>
#include "GameElement/GameElement.h"

USING_NS_CC;
using std::vector;

class Game : public cocos2d::Scene
{
private:
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	//��
	Layer* mouseLayer;
	Layer* game;
	
	//��ͼ
	//@map�㼶��
	//@  10   ���������
	//@	 100  ���
	TMXTiledMap* map;
	TMXLayer* _grass;
	TMXLayer* _grasswet;
	TMXLayer* _ground;
	TMXLayer* _sea;
	TMXLayer* _meta;

	//Ԫ��ͼ����
	//@empty 0 @full 1
	bool readBlock(Point position);
	void addBlock(Point position);
	void removeBlock(Point position);

	//����ת��
	//@visibleSize  ���ڿɼ���С
	//@mapSize      ��ͼ��С
	//@tmSize       ��ͼ��Ƭ��С
	Point visibleSize;
	Point mapSize;
	Point tmSize;

	//@��Ļ����ת���ɲ����꣨��������ϵ��
	Point convertToMapLayer(Point position)
	{
		position = position - map->getPosition();
		return position;
	}
	//@��������ת������Ƭ��ͼ����
	Point convertToTiledMap(Point position)
	{
		position.y = mapSize.y - position.y;

		Point tmPoint;
		int addx = 0, addy = 0;
		if ((int)position.x % (int)tmSize.x > tmSize.x / 2) addx++;
		if ((int)position.y % (int)tmSize.y > tmSize.y / 2) addy++;
		tmPoint.x = (int)(position.x / tmSize.x) + addx;
		tmPoint.y = (int)(position.y / tmSize.y) - addy;
		return tmPoint;
	}
	//@��Ƭת������������
	Point convertFromTMToWorld(Point position)
	{
		position.x *= tmSize.x;
		position.y *= tmSize.y;
		position.y = mapSize.y - position.y;
		return position;
	}


	//�¼�
	//@������Ϣ
	int myTeam;
	int resourceMoney;
	int resourcePower;
	//@��Ϸ״̬ - 0-off 1-on 2-building
	int selectedState;
	int buildState;
	
	//ĳЩ��������
	//@��Χ�ж� - ע�Ᵽ֤����ϵһ��
	bool rectContain(Rect rect,Point position)
	{
		if (position.x >= rect.getMinX() && position.x <= rect.getMaxX() && position.y >= rect.getMinY() &&position.y <= rect.getMaxY())
			return true;
		else
			return false;
	}

	//@onMouseMove:
	//@�ƶ�����
	void onMouseMove(cocos2d::Event* event);
	void onMouseDown(cocos2d::Event* event);
	
	//@����״̬
	Sprite* BuildingPictureWithMouse;

	//@�˵��¼�
	Menu* menu;
	Rect menuRect;

	MenuItemImage* basement_button;
	MenuItemImage* basementx_button;
	MenuItemImage* barrack_button;
	MenuItemImage* barrackx_button;
	MenuItemImage* powerplant_button;
	MenuItemImage* powerplantx_button;
	MenuItemImage* minefield_button;
	MenuItemImage* minefieldx_button;
	MenuItemImage* warfactory_button;
	MenuItemImage* warfactoryx_button;
	MenuItemImage* infantry_button;
	MenuItemImage* infantryx_button;
	MenuItemImage* dog_button;
	MenuItemImage* dogx_button;
	MenuItemImage* tank_button;
	MenuItemImage* tankx_button;

	//@��������/����
	//@BUILDING - ��������״̬�ͽ�����Ϣ
	int BUILDING;

	void buttonBasement(Ref* pSender);
	void buttonBarrack(Ref* pSender);
	void buttonPowerplant(Ref* pSender);
	void buttonMinefield(Ref* pSender);
	void buttonWarfactory(Ref* pSender);


	void buttonx(Ref* pSender);



	//Update���ں���
	virtual void update(float dt);
	void menuUpdate();

	//�����л�
	void menuReturn(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);

};

#endif // __GAME_H__
