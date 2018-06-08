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

	//传输串码  p    a      t   xxxxyyy  
			//玩家 动作   类型
	MyNumber = 0;			//获取玩家编号0/1/2/3  
							//0和2 1和3一队

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
	MouseReply->onMouseUp = [=](Event *event) {					//监听鼠标弹起事件

		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocationInView();					//鼠标事件的屏幕坐标系位置
		Vec2 target = position - map->getPosition();			//鼠标事件的地图坐标系位置
		//target = target * 0.78125;							
		//log("%f %f", e->getLocationInView().x, e->getLocationInView().y);
		if (!bool(e->getMouseButton())) {					//left button 左键
			if (ContainRect(position, Vec2(1100, 300), Vec2(1600, 0))) {		//点击范围包含在右下框
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
							SoldierList[MyNumber].pushBack(soldier);											//在固定位置创建一只妙蛙种子并推入soldierlist容器
							//输出“%MyNumber c  %(factory->getPosition()-Vec2(0,75)).x  %(factory->getPosition()-Vec2(0,75)).y”
							for (auto sp_obj : SoldierList[MyNumber]) {										//遍历soldierlist容器元素，全部设成未选中状态
								sp_obj->Select(0);
								sp_obj->SelectedReply();
							}
						}
					}
				}
				else if (ContainRect(position, Vec2(1100, 200), Vec2(1600, 100))) {
					building = 1;															//点击范围包含在右下方上面那个框，标志进入建造状态

				}
			}

			else {
				if (building) {															//建造状态
					if (!virtual_factory.empty()) {										//虚建筑容器非空，即跟随鼠标移动的透明建筑存在
						for (auto fac : virtual_factory) {
							fac->removeFromParent();									//移除虚的透明建筑
						}
						virtual_factory.popBack();
					}
					
					
					auto fac = Building::create("bd.png");								//在该位置添加实际建筑并改变标志building，退出建筑状态
					BuildingList[MyNumber].pushBack(fac);
					fac->setAnchorPoint(Vec2(0.5, 0.5));
					map->addChild(fac, 3);
					fac->setPosition(target);
					fac->SetType(1);
					building = 0;
					//输出字符串“ %MyNumber b  %target.x  %target.y”
				}
				else {
					for (auto sp_obj : SoldierList[MyNumber]) {									//遍历soldierlist容器元素，将第一个被鼠标点中的soldier变为被选中状态
						if (ContainSprite(sp_obj, target, 20)) {
							for (auto sp_objt : SoldierList[MyNumber]) {
								sp_objt->Select(0);
								sp_objt->SelectedReply();
							}
							log("%d", sp_obj->getTag());
							sp_obj->Select(1);
							sp_obj->SelectedReply();
							swallow = 1;												//吞噬标志，避免点击事件继续触发接下来的if
							break;
						}
					}
					if (!swallow) {
						for (int i = 0; i < 4;i++) {
							if (i % 2 != MyNumber % 2) {
								for (auto enemy : SoldierList[i]) {								//点中敌方soldier，给所有选中目标设置攻击target
									if (ContainSprite(enemy, target, 20) && !enemy->Died()) {
										for (auto myso : SoldierList[MyNumber]) {
											if (myso->Selected()) {
												myso->SetTarget(enemy);
												//输出“%MyNumber a %myso->getTag() %enemy->getTag()” 
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

					if (!swallow) {														//即没有点中任何soldier，也没有下达攻击指令，那么就遍历容器将所有被选中的士兵向点击位置移动
						for (auto sp_obj : SoldierList[MyNumber]) {
							if (sp_obj->Selected()) {
								
								sp_obj->SetTarget(nullptr);
								
								sp_obj->SetDestination(target);
								//输出“%MyNumber m %sp_obj->getTag() %target.x %target.y”
							}
						}
					}
				}
			}
			swallow = 0;									//重置吞噬标志







			//log("%d", MySoldierList.size());
			//log("%d", OpSoldierList.size());

		}
		if (bool(e->getMouseButton())) {												//right button 右键事件
			if (ContainRect(position, Vec2(xView - 300, 200), Vec2(xView, 0))) {		//右下第一个框范围事件

			}
			else {
				for (auto sp_obj : SoldierList[MyNumber]) {										//遍历容器，所有soldier变成未选中状态
					sp_obj->Select(0);
					sp_obj->SelectedReply();
				}
			}
			building = 0;																//退出建筑状态并把虚建筑移除
			if (!virtual_factory.empty()) {
				for (auto fac : virtual_factory) {
					fac->removeFromParent();
				}
				virtual_factory.popBack();
			}
		}
	};
	MouseReply->onMouseMove = [=](Event *event) {								//鼠标移动事件
		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocationInView();
		Vec2 target = position - map->getPosition();
		//target = target * 0.78125;
		//log("%f %f",position.x,position.y);
		//log("%f %f", target.x, target.y);
		//map->stopAllActions();							//地图大小为2000*1500，更改地图请对应更改magic numbers
		if (position.x > xView - 100) {							//鼠标靠近屏幕右侧
			if (!(map->numberOfRunningActions())) {				//这一句是为了地图视角移动更加流畅
				auto map_move = MoveTo::create((map->getPosition().x + (4000 - xView)) / 2000, Vec2(-(4000 - xView), map->getPosition().y));
				map->runAction(map_move);
			}
		}
		else if (position.x < 100) {							//鼠标靠近屏幕边缘左侧，将视角左移，即将地图右移
			if (!(map->numberOfRunningActions())) {
				auto map_move = MoveTo::create((-(map->getPosition().x)) / 2000, Vec2(0, map->getPosition().y));
				map->runAction(map_move);
			}
		}
		else if (position.y < 100) {							//鼠标靠近边缘上侧
			if (!(map->numberOfRunningActions())) {
				auto map_move = MoveTo::create((-(map->getPosition().y)) / 2000, Vec2(map->getPosition().x, 0));
				map->runAction(map_move);
			}
		}
		else if (position.y > yView - 100) {					//下侧
			if (!(map->numberOfRunningActions())) {
				auto map_move = MoveTo::create((map->getPosition().y + (4000 - yView)) / 2000, Vec2(map->getPosition().x, -(4000 - yView)));
				map->runAction(map_move);
			}
		}
		else {
			map->stopAllActions();								//移到屏幕中央时停止移动
		}
		if (building) {											//建筑状态会同时响应屏幕移动和虚建筑跟随鼠标移动

			auto factory = Sprite::create("bd.png");
			if (!virtual_factory.empty()) {						//一旦鼠标移动，移除虚建筑容器中唯一的元素，推入新的虚建筑元素到当前鼠标位置
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
	auto dispatcher = Director::getInstance()->getEventDispatcher();					//添加鼠标事件监听器
	dispatcher->addEventListenerWithSceneGraphPriority(MouseReply, this);

	//for (auto myso : MySoldierList) {
	//myso->scheduleUpdate();
	//myso->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
	//}
	scheduleUpdate();




	//输入接口











	

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
bool ContainRect(Vec2 current, Vec2 pt1, Vec2 pt2) {						//判断current坐标是否包含在左上点pt1和右下点pt2之间的矩形
	if ((current.x > pt1.x) && (current.x < pt2.x) && (current.y < pt1.y) && (current.y > pt2.y)) {
		return true;
	}
	else {
		return false;
	}
}
float Distance(Vec2 pt1, Vec2 pt2) {					//返回两点间的直线距离

	float dist = 0;
	dist += pow(pt1.x - pt2.x, 2);
	dist += pow((pt1.y - pt2.y), 2);
	dist = sqrt(dist);
	return dist;

}
bool ContainSprite(Sprite* sp, Vec2 position, float di) {				//判断position点是否在sp元素的位置的x y直径di范围内
	Vec2 spv = sp->getPosition();
	if (spv.x > position.x - di && spv.x < position.x + di && spv.y<position.y + di && spv.y>position.y - di) {
		return true;
	}
	else {
		return false;
	}
}