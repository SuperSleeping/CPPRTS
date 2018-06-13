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

	//��ͼ
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
	//@viewPoint    ��ǰ��Ұ�����½�����������ϵ�ϵ�λ��
	Point visibleSize;
	Point mapSize;
	Point tmSize;
	Point viewPoint;

	//@��Ļ����ת���ɲ����꣨��������ϵ��
	Point convertToMapLayer(Point position)
	{
		position.x += viewPoint.x;
		position.y += viewPoint.y;
		return position;
	}
	//@��������ת������Ƭ��ͼ����
	Point convertToTiledMap(Point position)
	{
		Point tmPoint;
		tmPoint.x = position.x / tmSize.x;
		tmPoint.y = (mapSize.y - position.y) / tmSize.y;
		return tmPoint;
	}

	//�¼�
	//@��Ϸ״̬ - 0-off 1-on 2-building 
	int buildState;

	//@onMouseMove:
	//@�ƶ�����
	void onMouseMove(cocos2d::Event* event);

	//@�˵��¼�
	//@��������/����
	//@BUILDING - ��������״̬�ͽ�����Ϣ
	int BUILDING;
	void buttonBasement(Ref* pSender);
	void buttonBasementx(Ref* pSender);



	//Update���ں���
	virtual void update(float dt);

	//�����л�
	void menuReturn(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);

};

#endif // __GAME_H__
