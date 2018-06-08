#include"GameScene.h"
#include"cocos2d.h"
#include"Soldier.h"
#include"Building.h"
#include<memory>
USING_NS_CC;
bool ContainRect(Vec2 current, Vec2 pt1, Vec2 pt2);
bool ContainSprite(Sprite* sp, Vec2 position, float di);
float Distance(Vec2 pt1, Vec2 pt2);
static Vector<Soldier*> OpSoldierList;
static Vector<Soldier*> MySoldierList;
using namespace CocosDenshion;
static Vector<Soldier*> SoldierList[4];
static Vector<Building*> BuildingList[4];
//static int SoldierTag = 0;
static int MyNumber = 0;
Scene* GameScene::createScene()
{
	return GameScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	//���䴮��  p    a      t   xxxxyyy  
			//��� ����   ����
	MyNumber = 0;			//��ȡ��ұ��0/1/2/3  
							//0��2 1��3һ��

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menu = Sprite::create("menucube.png");
	this->addChild(menu, 100);
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(Vec2(1100, 0));

	static float xView = Director::getInstance()->getVisibleSize().width;
	static float yView = Director::getInstance()->getVisibleSize().height;


	//static Vector<Soldier*> OpSoldierList;
	static Vector<Sprite*> virtual_factory;
	static Vector<Building*> real_factory;
	static Vector<ProgressTimer*> bloodbar;
	static Vector<ProgressTimer*> OpBloodBar;
	static Vector<Sprite*> bloodbg;

	static int building = 0;
	static Sprite* factory = Sprite::create("bd.png");
	log("%f %f", xView, yView);

	static bool swallow = 0;

	TMXTiledMap *map = TMXTiledMap::create("newmap.tmx");
	addChild(map);
	//map->setScale(1 / 0.78125, 1 / 0.78125);

	log("%f %f", map->getContentSize().width, map->getContentSize().height);


	auto testenem = Soldier::create("enemy.png");
	testenem->SetSide(2);
	testenem->setPosition(Vec2(400, 400));
	SoldierList[1].pushBack(testenem);
	map->addChild(testenem, 100);
	testenem->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);
	testenem->scheduleUpdate();
	testenem->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);


	auto testmy = Soldier::create("seedz.png");
	testmy->SetSide(1);
	testmy->setPosition(Vec2(300, 400));
	SoldierList[MyNumber].pushBack(testmy);
	map->addChild(testmy, 100);
	testmy->scheduleUpdate();
	testmy->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
	testmy->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);

	testenem->SetTarget(testmy);


	static auto MouseReply = EventListenerMouse::create();
	MouseReply->onMouseUp = [=](Event *event) {					//������굯���¼�

		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocationInView();					//����¼�����Ļ����ϵλ��
		Vec2 target = position - map->getPosition();			//����¼��ĵ�ͼ����ϵλ��
		//target = target * 0.78125;							
		//log("%f %f", e->getLocationInView().x, e->getLocationInView().y);
		if (!bool(e->getMouseButton())) {					//left button ���
			if (ContainRect(position, Vec2(1100, 300), Vec2(1600, 0))) {		//�����Χ���������¿�
				if (ContainRect(position, Vec2(1100, 300), Vec2(1600, 200))) {
					for (Building* factory : BuildingList[MyNumber]) {
						
						if (factory->Type() == 1) {
							
							Soldier* soldier = Soldier::create("seedz.png");
							soldier->setPosition(factory->getPosition()-Vec2(0,75));
							soldier->SetSide(MyNumber);
							//soldier->setTag(1);
							soldier->scheduleUpdate();
							soldier->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
							soldier->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);
							map->addChild(soldier, 100);
							SoldierList[MyNumber].pushBack(soldier);											//�ڹ̶�λ�ô���һֻ�������Ӳ�����soldierlist����
							//�����%MyNumber c  %(factory->getPosition()-Vec2(0,75)).x  %(factory->getPosition()-Vec2(0,75)).y��
							for (auto sp_obj : SoldierList[MyNumber]) {										//����soldierlist����Ԫ�أ�ȫ�����δѡ��״̬
								sp_obj->Select(0);
								sp_obj->SelectedReply();
							}
						}
					}
				}
				else if (ContainRect(position, Vec2(1100, 200), Vec2(1600, 100))) {
					building = 1;															//�����Χ���������·������Ǹ��򣬱�־���뽨��״̬

				}
			}

			else {
				if (building) {															//����״̬
					if (!virtual_factory.empty()) {										//�齨�������ǿգ�����������ƶ���͸����������
						for (auto fac : virtual_factory) {
							fac->removeFromParent();									//�Ƴ����͸������
						}
						virtual_factory.popBack();
					}
					
					
					auto fac = Building::create("bd.png");								//�ڸ�λ�����ʵ�ʽ������ı��־building���˳�����״̬
					BuildingList[MyNumber].pushBack(fac);
					fac->setAnchorPoint(Vec2(0.5, 0.5));
					map->addChild(fac, 3);
					fac->setPosition(target);
					fac->SetType(1);
					building = 0;
					//����ַ����� %MyNumber b  %target.x  %target.y��
				}
				else {
					for (auto sp_obj : SoldierList[MyNumber]) {									//����soldierlist����Ԫ�أ�����һ���������е�soldier��Ϊ��ѡ��״̬
						if (ContainSprite(sp_obj, target, 20)) {
							for (auto sp_objt : SoldierList[MyNumber]) {
								sp_objt->Select(0);
								sp_objt->SelectedReply();
							}
							log("%d", sp_obj->getTag());
							sp_obj->Select(1);
							sp_obj->SelectedReply();
							swallow = 1;												//���ɱ�־���������¼�����������������if
							break;
						}
					}
					if (!swallow) {
						for (int i = 0; i < 4;i++) {
							if (i % 2 != MyNumber % 2) {
								for (auto enemy : SoldierList[i]) {								//���ез�soldier��������ѡ��Ŀ�����ù���target
									if (ContainSprite(enemy, target, 20) && !enemy->Died()) {
										for (auto myso : SoldierList[MyNumber]) {
											if (myso->Selected()) {
												myso->SetTarget(enemy);
												//�����%MyNumber a %myso->getTag() %enemy->getTag()�� 
											}
										}

										//log("1");
										swallow = 1;
										break;
									}
									if (swallow) {
										break;
									}
								}
							}
						}
					}

					if (!swallow) {														//��û�е����κ�soldier��Ҳû���´﹥��ָ���ô�ͱ������������б�ѡ�е�ʿ������λ���ƶ�
						for (auto sp_obj : SoldierList[MyNumber]) {
							if (sp_obj->Selected()) {
								
								sp_obj->SetTarget(nullptr);
								
								sp_obj->SetDestination(target);
								//�����%MyNumber m %sp_obj->getTag() %target.x %target.y��
							}
						}
					}
				}
			}
			swallow = 0;									//�������ɱ�־







			//log("%d", MySoldierList.size());
			//log("%d", OpSoldierList.size());

		}
		if (bool(e->getMouseButton())) {												//right button �Ҽ��¼�
			if (ContainRect(position, Vec2(xView - 300, 200), Vec2(xView, 0))) {		//���µ�һ����Χ�¼�

			}
			else {
				for (auto sp_obj : SoldierList[MyNumber]) {										//��������������soldier���δѡ��״̬
					sp_obj->Select(0);
					sp_obj->SelectedReply();
				}
			}
			building = 0;																//�˳�����״̬�����齨���Ƴ�
			if (!virtual_factory.empty()) {
				for (auto fac : virtual_factory) {
					fac->removeFromParent();
				}
				virtual_factory.popBack();
			}
		}
	};
	MouseReply->onMouseMove = [=](Event *event) {								//����ƶ��¼�
		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocationInView();
		Vec2 target = position - map->getPosition();
		//target = target * 0.78125;
		//log("%f %f",position.x,position.y);
		//log("%f %f", target.x, target.y);
		//map->stopAllActions();							//��ͼ��СΪ2000*1500�����ĵ�ͼ���Ӧ����magic numbers
		if (position.x > xView - 100) {							//��꿿����Ļ�Ҳ�
			if (!(map->numberOfRunningActions())) {				//��һ����Ϊ�˵�ͼ�ӽ��ƶ���������
				auto map_move = MoveTo::create((map->getPosition().x + (4000 - xView)) / 2000, Vec2(-(4000 - xView), map->getPosition().y));
				map->runAction(map_move);
			}
		}
		else if (position.x < 100) {							//��꿿����Ļ��Ե��࣬���ӽ����ƣ�������ͼ����
			if (!(map->numberOfRunningActions())) {
				auto map_move = MoveTo::create((-(map->getPosition().x)) / 2000, Vec2(0, map->getPosition().y));
				map->runAction(map_move);
			}
		}
		else if (position.y < 100) {							//��꿿����Ե�ϲ�
			if (!(map->numberOfRunningActions())) {
				auto map_move = MoveTo::create((-(map->getPosition().y)) / 2000, Vec2(map->getPosition().x, 0));
				map->runAction(map_move);
			}
		}
		else if (position.y > yView - 100) {					//�²�
			if (!(map->numberOfRunningActions())) {
				auto map_move = MoveTo::create((map->getPosition().y + (4000 - yView)) / 2000, Vec2(map->getPosition().x, -(4000 - yView)));
				map->runAction(map_move);
			}
		}
		else {
			map->stopAllActions();								//�Ƶ���Ļ����ʱֹͣ�ƶ�
		}
		if (building) {											//����״̬��ͬʱ��Ӧ��Ļ�ƶ����齨����������ƶ�

			auto factory = Sprite::create("bd.png");
			if (!virtual_factory.empty()) {						//һ������ƶ����Ƴ��齨��������Ψһ��Ԫ�أ������µ��齨��Ԫ�ص���ǰ���λ��
				for (auto fac : virtual_factory) {
					fac->removeFromParent();
				}
				virtual_factory.popBack();
			}
			virtual_factory.pushBack(factory);
			for (auto fac : virtual_factory) {
				map->addChild(fac, 3);
				fac->setOpacity(100);
				fac->setPosition(target);
			}

		}
	};
	auto dispatcher = Director::getInstance()->getEventDispatcher();					//�������¼�������
	dispatcher->addEventListenerWithSceneGraphPriority(MouseReply, this);

	//for (auto myso : MySoldierList) {
	//myso->scheduleUpdate();
	//myso->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
	//}
	scheduleUpdate();




	//����ӿ�











	

	return true;
}
void GameScene::updateErase(float di) {

	for (auto en : OpSoldierList) {

		//log("%f %f", en->getPosition().x, en->getPosition().y);
		if (en->Died()) {
			//delete en;
			//en =nullptr;
			OpSoldierList.eraseObject(en);

			log("delete");
			break;
		}
	}
	for (auto my : MySoldierList) {

		//log("%f %f", en->getPosition().x, en->getPosition().y);
		if (my->Died()) {
			MySoldierList.eraseObject(my);

			log("delete");
			break;
		}
	}
}




//current point,top left point,lower right point
bool ContainRect(Vec2 current, Vec2 pt1, Vec2 pt2) {						//�ж�current�����Ƿ���������ϵ�pt1�����µ�pt2֮��ľ���
	if ((current.x > pt1.x) && (current.x < pt2.x) && (current.y < pt1.y) && (current.y > pt2.y)) {
		return true;
	}
	else {
		return false;
	}
}
float Distance(Vec2 pt1, Vec2 pt2) {					//����������ֱ�߾���

	float dist = 0;
	dist += pow(pt1.x - pt2.x, 2);
	dist += pow((pt1.y - pt2.y), 2);
	dist = sqrt(dist);
	return dist;

}
bool ContainSprite(Sprite* sp, Vec2 position, float di) {				//�ж�position���Ƿ���spԪ�ص�λ�õ�x yֱ��di��Χ��
	Vec2 spv = sp->getPosition();
	if (spv.x > position.x - di && spv.x < position.x + di && spv.y<position.y + di && spv.y>position.y - di) {
		return true;
	}
	else {
		return false;
	}
}