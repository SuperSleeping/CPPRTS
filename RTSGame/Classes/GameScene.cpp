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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menu = Sprite::create("menu.png");
	this->addChild(menu,100);
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




	auto testenem = Soldier::create("enemy.png");
	testenem->SetSide(2);
	testenem->setPosition(Vec2(400, 400));
	OpSoldierList.pushBack(testenem);
	map->addChild(testenem, 100);
	testenem->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);
	testenem->scheduleUpdate();
	testenem->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);


	auto testmy = Soldier::create("seedz.png");
	testmy->SetSide(1);
	testmy->setPosition(Vec2(300, 400));
	MySoldierList.pushBack(testmy);
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
																//log("%f %f", e->getLocationInView().x, e->getLocationInView().y);
		if (!bool(e->getMouseButton())) {					//left button 左键
			if (ContainRect(position, Vec2(xView - 300, 200), Vec2(xView, 0))) {		//点击范围包含在右下框
				Soldier* soldier = Soldier::create("seedz.png");
				soldier->setPosition(Vec2(600, 600));
				soldier->SetSide(1);
				soldier->scheduleUpdate();
				soldier->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
				soldier->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);
				map->addChild(soldier, 100);
				/*ProgressTimer* prog = ProgressTimer::create(Sprite::create("blood.png"));
				prog->setPercentage(100);
				prog->setAnchorPoint(Vec2(0, 0));
				bloodbar.pushBack(prog);
				soldier->addChild(prog);*/
				MySoldierList.pushBack(soldier);											//在固定位置创建一只妙蛙种子并推入soldierlist容器
				for (auto sp_obj : MySoldierList) {										//遍历soldierlist容器元素，全部设成未选中状态
					sp_obj->Select(0);
					sp_obj->SelectedReply();
				}
			}
			else if (ContainRect(position, Vec2(xView - 300, 400), Vec2(xView, 200))) {
				building = 1;															//点击范围包含在右下方上面那个框，标志进入建造状态

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
					real_factory.pushBack(fac);
					fac->setAnchorPoint(Vec2(0.5, 0.5));
					map->addChild(fac, 3);
					fac->setPosition(target);
					building = 0;
				}
				else {
					for (auto sp_obj : MySoldierList) {									//遍历soldierlist容器元素，将第一个被鼠标点中的soldier变为被选中状态
						if (ContainSprite(sp_obj, target, 20)) {
							for (auto sp_objt : MySoldierList) {
								sp_objt->Select(0);
								sp_objt->SelectedReply();
							}
							/*sp_obj->removeAllChildren();						//点击移除精灵兵释放掉容器内占用内存的测试代码
							sp_obj->removeFromParent();
							for (auto pg : bloodbar) {
							if (!pg->getParent()) {
							bloodbar.eraseObject(pg);
							break;
							}
							}
							SoldierList.eraseObject(sp_obj);*/
							//bloodbar.eraseObject(sp_obj->)
							sp_obj->Select(1);
							sp_obj->SelectedReply();
							swallow = 1;												//吞噬标志，避免点击事件继续触发接下来的if
							break;
						}
					}
					if (!swallow) {
						for (auto enemy : OpSoldierList) {								//点中敌方soldier，给所有选中目标设置攻击target
							if (ContainSprite(enemy, target, 20) && !enemy->Died()) {
								for (auto myso : MySoldierList) {
									if (myso->Selected()) {
										myso->SetTarget(enemy);
									}
								}

								log("1");
								swallow = 1;
								break;
							}

						}
					}

					if (!swallow) {														//即没有点中任何soldier，也没有下达攻击指令，那么就遍历容器将所有被选中的士兵向点击位置移动
						for (auto sp_obj : MySoldierList) {
							if (sp_obj->Selected()) {
								sp_obj->stopAllActions();
								sp_obj->SetTarget(nullptr);
								auto move_target = MoveTo::create(Distance(target, sp_obj->getPosition()) / 50, target);
								sp_obj->runAction(move_target);
							}
						}
					}
				}
			}
			swallow = 0;									//重置吞噬标志







			log("%d", MySoldierList.size());
			log("%d", OpSoldierList.size());

		}
		if (bool(e->getMouseButton())) {												//right button 右键事件
			if (ContainRect(position, Vec2(xView - 300, 200), Vec2(xView, 0))) {		//右下第一个框范围事件

			}
			else {
				for (auto sp_obj : MySoldierList) {										//遍历容器，所有soldier变成未选中状态
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
		//	log("%f %f", position.x, position.y);

		//map->stopAllActions();							//地图大小为2000*1500，更改地图请对应更改magic numbers
		if (position.x > xView - 100) {							//鼠标靠近屏幕右侧
			if (!(map->numberOfRunningActions())) {				//这一句是为了地图视角移动更加流畅
				auto map_move = MoveTo::create((map->getPosition().x + (2000 - xView)) / 1000, Vec2(-(2000 - xView), map->getPosition().y));
				map->runAction(map_move);
			}
		}
		else if (position.x < 100) {							//鼠标靠近屏幕边缘左侧，将视角左移，即将地图右移
			if (!(map->numberOfRunningActions())) {
				auto map_move = MoveTo::create((-(map->getPosition().x)) / 1000, Vec2(0, map->getPosition().y));
				map->runAction(map_move);
			}
		}
		else if (position.y < 100) {							//鼠标靠近边缘上侧
			if (!(map->numberOfRunningActions())) {
				auto map_move = MoveTo::create((-(map->getPosition().y)) / 1000, Vec2(map->getPosition().x, 0));
				map->runAction(map_move);
			}
		}
		else if (position.y > yView - 100) {					//下侧
			if (!(map->numberOfRunningActions())) {
				auto map_move = MoveTo::create((map->getPosition().y + (1600 - yView)) / 1000, Vec2(map->getPosition().x, -(1600 - yView)));
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
















/*	////top label
	//runtime
	auto runtime = Label::createWithSystemFont("Runtime:6min", "Arial", 18);
	runtime->setPosition(Vec2(origin.x + visibleSize.width / 11, origin.y + visibleSize.height *29/30));
	this->addChild(runtime);

	//remain
	auto remain = Label::createWithSystemFont("Remain:nothing", "Arial", 18);
	remain->setPosition(Vec2(origin.x + visibleSize.width *10/ 11, origin.y + visibleSize.height *29/30));
	this->addChild(remain);
	
	auto draw = DrawNode::create();
	this->addChild(draw);

	//ability
	Vec2 points1[] = { Vec2(origin.x+visibleSize.width/3,origin.y), Vec2(origin.x + visibleSize.width *2/ 3,origin.y ), Vec2(origin.x + visibleSize.width * 2 / 3,origin.y+visibleSize.height / 15), Vec2(origin.x + visibleSize.width / 3,origin.y + visibleSize.height / 15) };
	draw->drawPolygon(points1, sizeof(points1) / sizeof(points1[0]), Color4F(1, 0, 0, 0), 1, Color4F(1, 0, 0, 1));
	auto ability = Label::createWithSystemFont("ability:bualbula", "Arial", 18);
	//3/9-6/9  0-1/15
	ability->setPosition(Vec2(origin.x + visibleSize.width*4/9, origin.y + visibleSize.height/30));
	this->addChild(ability);

	//map
	Vec2 points2[] = { Vec2(origin.x,origin.y), Vec2(origin.x ,origin.y+visibleSize.height/3), Vec2(origin.x + visibleSize.width/ 3,origin.y + visibleSize.height / 3), Vec2(origin.x + visibleSize.width / 3,origin.y) };
	draw->drawPolygon(points2, sizeof(points2) / sizeof(points2[0]), Color4F(1, 0, 0, 0), 1, Color4F(1, 0, 0, 1));
	//text
	Vec2 points3[] = { Vec2(origin.x + visibleSize.width / 3,origin.y + visibleSize.height / 15), Vec2(origin.x + visibleSize.width/ 3,origin.y + visibleSize.height / 3), Vec2(origin.x + visibleSize.width * 2 / 3,origin.y + visibleSize.height /3), Vec2(origin.x + visibleSize.width *2/ 3,origin.y + visibleSize.height / 15) };
	draw->drawPolygon(points3, sizeof(points3) / sizeof(points3[0]), Color4F(1, 0, 0, 0), 1, Color4F(1, 0, 0, 1));

	//various stuff
	Vec2 points4[] = { Vec2(origin.x + visibleSize.width *2/ 3,origin.y), Vec2(origin.x + visibleSize.width *2/ 3,origin.y + visibleSize.height / 3), Vec2(origin.x + visibleSize.width ,origin.y + visibleSize.height / 3), Vec2(origin.x + visibleSize.width ,origin.y ) };
	draw->drawPolygon(points4, sizeof(points4) / sizeof(points4[0]), Color4F(1, 0, 0, 0), 1, Color4F(1, 0, 0, 1));
	draw->drawSegment(Vec2(origin.x + visibleSize.width *2/ 3, origin.y + visibleSize.height / 9), Vec2(origin.x + visibleSize.width*8/9, origin.y + visibleSize.height / 9), 1, Color4F(1, 0, 0, 1));
	draw->drawSegment(Vec2(origin.x + visibleSize.width * 2 / 3, origin.y + visibleSize.height *2/9), Vec2(origin.x+ visibleSize.width, origin.y + visibleSize.height *2/9), 1, Color4F(1, 0, 0, 1));

	draw->drawSegment(Vec2(origin.x + visibleSize.width * 13/18, origin.y ), Vec2(origin.x + visibleSize.width * 13/18, origin.y + visibleSize.height / 3), 1, Color4F(1, 0, 0, 1));
	draw->drawSegment(Vec2(origin.x + visibleSize.width * 14/18, origin.y ), Vec2(origin.x + visibleSize.width * 14/18, origin.y + visibleSize.height / 3), 1, Color4F(1, 0, 0, 1));
	draw->drawSegment(Vec2(origin.x + visibleSize.width *15/18, origin.y + visibleSize.height / 9), Vec2(origin.x + visibleSize.width * 15/18, origin.y + visibleSize.height / 3), 1, Color4F(1, 0, 0, 1));
	draw->drawSegment(Vec2(origin.x + visibleSize.width * 16/18, origin.y + visibleSize.height / 9), Vec2(origin.x + visibleSize.width * 16/18, origin.y + visibleSize.height / 3), 1, Color4F(1, 0, 0, 1));
	draw->drawSegment(Vec2(origin.x + visibleSize.width * 17/18, origin.y + visibleSize.height*2/9), Vec2(origin.x + visibleSize.width *17/18, origin.y + visibleSize.height / 3), 1, Color4F(1, 0, 0, 1));
	
	auto building = Label::createWithSystemFont("Building:", "Arial", 12);
	building->setPosition(Vec2(origin.x + visibleSize.width * 25/36, origin.y + visibleSize.height/18));
	this->addChild(building);

	auto soldier = Label::createWithSystemFont("Soldier:", "Arial", 12);
	soldier->setPosition(Vec2(origin.x + visibleSize.width * 25/36, origin.y + visibleSize.height*3/18));
	this->addChild(soldier);

	auto tank = Label::createWithSystemFont("Tank:", "Arial", 12);
	tank->setPosition(Vec2(origin.x + visibleSize.width * 25/36, origin.y + visibleSize.height*5/18));
	this->addChild(tank);



	//sound
	

	//return*/

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

/*void GameScene::update(float di) {
log("e");
for (auto en : OpSoldierList) {

if (en->Died()) {
OpSoldierList.eraseObject(en);
log("delete");
break;
}
}
}*/


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