#include"GameScene.h"
#include"SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
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
	this->addChild(menu);
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(Vec2(1100, 0));

	auto MouseEvent = EventListenerMouse::create();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseEvent, this);
	MouseEvent->onMouseMove = [](Event* event) {
		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocationInView();
		log("%d %d", position.x, position.y);
	};

















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
