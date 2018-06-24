#ifndef _GAME_H_
#define _GAME_H_

#include "network/SocketIO.h"
#include "cocos2d.h"
#include <vector>
#include "GameElement/GameElement.h"


USING_NS_CC;
using std::vector;

extern int Characters[118][138];

//extern vector<Infantry*> infantryGroup[4];

class Game : public cocos2d::Scene,public cocos2d::network::SocketIO::SIODelegate
{
private:
	cocos2d::network::SIOClient *sioClient;
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	/*ί��Э�麯��*/
	virtual void onConnect(cocos2d::network::SIOClient *client);
	virtual void onMessage(cocos2d::network::SIOClient *client, const std::string& data);
	virtual void onClose(cocos2d::network::SIOClient *client);
	virtual void onError(cocos2d::network::SIOClient *client, const std::string& data);
	
	void attackRespone(const std::string &data);
	void moveRespone(const std::string &data);
	void createRespone(const std::string &data);
	void buildRespone(const std::string &data);

	void numberClientEvent(cocos2d::network::SIOClient *client, const std::string& data);

	//��
	Layer* mouseLayer;
	Layer* game;

	void updateResources(float di);


	//Ԫ��ͼ����
	//@empty 0 @full 1
	bool readBlock(Point tmPoint);
	void addBlock(Point tmPoint);
	void removeBlock(Point tmPoint);
	//@Ԫ��ͼBlock������isBlock���鱣�� �����޸�
	bool isBlock[118][138];
	void isBlockInitialize();




	//�¼�
	//@������Ϣ
	
	int resourceMoney;
	int resourcePower;
	//@��Ϸ״̬ - 0-off 1-on 2-building
	bool selectedState;			//0��ѡ��״̬/1ѡ��״̬
	int selectedType;			//0��ѡ�����/ѡ��Ľ������ͣ��ı�˵���/ѡ������״̬
	Point selectedSpawnPoint;
	int buildState;				//0�ǽ���״̬/Ԥ���轨������
	bool OKtobuilt;

	//ĳЩ��������
	//@��Χ�ж� - ע�Ᵽ֤����ϵһ��
	bool rectContain(Rect rect, Point position)
	{
		if (position.x >= rect.getMinX() && position.x <= rect.getMaxX() && position.y >= rect.getMinY() && position.y <= rect.getMaxY())
			return true;
		else
			return false;
	}

	//@��ײ���
	//@������

	//      *
	//    * * *
	//      *		occupiedCoordinate[0]
	//      *
	//    * * *
	//  * * * * *
	//    * * *
	//      *		occupiedCoordinate[1]
	vector<Vec2> occupiedCoordinate[2];
	vector<Vec2> checkOccupiedCoordinate[2];
	vector<Vec2>::iterator iter;
	void occupiedRelatedCoordinateInitialize()
	{
		occupiedCoordinate[0].push_back(Vec2(-1, 0));
		occupiedCoordinate[0].push_back(Vec2(0, -1));
		occupiedCoordinate[0].push_back(Vec2(0, 0));
		occupiedCoordinate[0].push_back(Vec2(0, 1));
		occupiedCoordinate[0].push_back(Vec2(1, 0));

		occupiedCoordinate[1].push_back(Vec2(-2, 0));
		occupiedCoordinate[1].push_back(Vec2(-1, -1));
		occupiedCoordinate[1].push_back(Vec2(-1, 0));
		occupiedCoordinate[1].push_back(Vec2(-1, 1));
		occupiedCoordinate[1].push_back(Vec2(0, -2));
		occupiedCoordinate[1].push_back(Vec2(0, -1));
		occupiedCoordinate[1].push_back(Vec2(0, 0));
		occupiedCoordinate[1].push_back(Vec2(0, 1));
		occupiedCoordinate[1].push_back(Vec2(0, 2));
		occupiedCoordinate[1].push_back(Vec2(1, -1));
		occupiedCoordinate[1].push_back(Vec2(1, 0));
		occupiedCoordinate[1].push_back(Vec2(1, 1));
		occupiedCoordinate[1].push_back(Vec2(2, 0));

		checkOccupiedCoordinate[0].push_back(Vec2(0, 0));

		checkOccupiedCoordinate[1].push_back(Vec2(-1, 0));
		checkOccupiedCoordinate[1].push_back(Vec2(0, -1));
		checkOccupiedCoordinate[1].push_back(Vec2(0, 0));
		checkOccupiedCoordinate[1].push_back(Vec2(0, 1));
		checkOccupiedCoordinate[1].push_back(Vec2(1, 0));

	}

	//@��ײ��⣺������Χ����Ƭ������
	bool buildingTypeJudge(int x)
	{
		if (x == 2)
		{
			return 0;
		}
		else return 1;
	}
	bool readOccupiedTile(Point tmPoint, int buildingType);		//����ռ�ռ��Ƿ��ѱ�ռ�У��ж��Ƿ����½�	//ռ��-1 δռ��-0
	void changeOccupiedTile(Point tmPoint, int buildingType);	//�½���ı��ͼ����

	//����¼�
	//@����״̬
	//@�Ѱ������ܣ�
	//
	//******Up:
	//		*(����ѡ��ĳBuilding/Character)
	//
	//******Down��
	//		*(��ѡCharacter)
	//		��һ���½���
	//
	//******Move:
	//		�ӽ��л�
	//		����״̬�£�ѡ���Ŀɽ������͸�������ƶ�
	void onMouseMove(cocos2d::Event* event);
	void onMouseDown(cocos2d::Event* event);
	void onMouseUp(cocos2d::Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode,cocos2d::Event* event);

	//@��ѡ��Χ
	Point firstPress;
	Point lastPress;
	Rect selectRect;
	DrawNode* rectangle;
	void drawline();

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
	MenuItemImage* teamchange_button;
	//@��������/����
	//@BUILDING - ��������״̬�ͽ�����Ϣ
	int BUILDING;

	void buttonBasement(Ref* pSender);
	void buttonBarrack(Ref* pSender);
	void buttonPowerplant(Ref* pSender);
	void buttonMinefield(Ref* pSender);
	void buttonWarfactory(Ref* pSender);
	void buttonInfantry(Ref* pSender);
	void buttonDog(Ref* pSender);
	void buttonTank(Ref* pSender);
	void buttonTeamchange(Ref* pSender);

	void buttonx(Ref* pSender);

	//



	//Update���ں���
	virtual void update(float dt);
	void menuUpdate();

	void updateMapCharacter(float di);
	void updateZOrder(float di);
	
	//�����л�
	void menuReturn(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);

};

#endif // __GAME_H__
