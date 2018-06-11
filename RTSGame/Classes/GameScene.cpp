#include"GameScene.h"
#include"SimpleAudioEngine.h"
#include"Soldier.h"
#include"Building.h"
#include "ui/CocosGUI.h"
#include "UserDefault.h"
#include "extensions/cocos-ext.h"			//editbox

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
using namespace cocos2d::ui;

bool ContainRect(Vec2 current, Vec2 pt1, Vec2 pt2);
bool ContainSprite(Sprite* sp, Vec2 position, float di);
float Distance(Vec2 pt1, Vec2 pt2);
string SpawnDatastring(int Player, char type, int n, int m);
string SpawnDatastring(int Player, char type, int n, int m, int t);
static Vector<Soldier*> SoldierList[4];
static Vector<Building*> BuildingList[4];
//static int SoldierTag = 0;
static int MyNumber = 0;
static TMXTiledMap *PlayMap;


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

	std::string hostIp = UserDefault::getInstance()->getStringForKey(HOST_IP);
	sioClient = cocos2d::network::SocketIO::connect(hostIp, *this);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();



	MyNumber = 0;			//获取玩家编号0/1/2/3  
							//0和2 1和3一队



	auto menu = Sprite::create("menucube.png");
	this->addChild(menu, 100);
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(Vec2(1100, 0));

	auto dianchang = Sprite::create("dianchang2.png");
	menu->addChild(dianchang);
	dianchang->setAnchorPoint(Vec2(0, 0));
	dianchang->setPosition(100, 100);

	auto kuangchang = Sprite::create("kuangchang2.png");
	menu->addChild(kuangchang);
	kuangchang->setAnchorPoint(Vec2(0, 0));
	kuangchang->setPosition(200, 100);

	auto bingying = Sprite::create("bingying2.png");
	menu->addChild(bingying);
	bingying->setAnchorPoint(Vec2(0, 0));
	bingying->setPosition(300, 100);

	auto chechang = Sprite::create("chechang2.png");
	menu->addChild(chechang);
	chechang->setAnchorPoint(Vec2(0, 0));
	chechang->setPosition(400, 100);








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

	PlayMap = TMXTiledMap::create("newmap.tmx");
	addChild(PlayMap);
	//map->setScale(1 / 0.78125, 1 / 0.78125);

	//log("%f %f", map->getContentSize().width, map->getContentSize().height);


	auto testenem = Soldier::create("enemy.png");
	testenem->SetSide(2);
	testenem->setPosition(Vec2(400, 400));
	SoldierList[1].pushBack(testenem);
	PlayMap->addChild(testenem, 100);
	testenem->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);
	testenem->scheduleUpdate();
	testenem->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);


	auto testmy = Soldier::create("seedz.png");
	testmy->SetSide(1);
	testmy->setPosition(Vec2(300, 400));
	SoldierList[MyNumber].pushBack(testmy);
	PlayMap->addChild(testmy, 100);
	testmy->scheduleUpdate();
	testmy->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
	testmy->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);

	testenem->SetTarget(testmy);

	buildRespone(SpawnDatastring(1, 'b', 300, 300, 1));

	static auto MouseReply = EventListenerMouse::create();
	MouseReply->onMouseUp = [=](Event *event) {					//监听鼠标弹起事件
		string str = "11";
		sioClient->send(str);
		log("%d", SoldierList[0].size());
		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocationInView();					//鼠标事件的屏幕坐标系位置
		Vec2 target = position - PlayMap->getPosition();			//鼠标事件的地图坐标系位置
																	//target = target * 0.78125;							
																	//log("%f %f", e->getLocationInView().x, e->getLocationInView().y);
		if (!bool(e->getMouseButton())) {					//left button 左键
			if (ContainRect(position, Vec2(1100, 300), Vec2(1600, 0))) {		//点击范围包含在右下框
				if (ContainRect(position, Vec2(1100, 300), Vec2(1600, 200))) {
					for (Building* factory : BuildingList[MyNumber]) {

						if (factory->Type() == 1 && !factory->Destroyed()) {

							//sioClient->send(SpawnDatastring(MyNumber, 'c', (factory->getPosition() - Vec2(0, 75)).x, (factory->getPosition() - Vec2(0, 75)).y));
							createRespone(SpawnDatastring(MyNumber, 'c', (factory->getPosition() - Vec2(0, 75)).x, (factory->getPosition() - Vec2(0, 75)).y));
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


					/*auto fac = Building::create("bd.png");								//在该位置添加实际建筑并改变标志building，退出建筑状态
					BuildingList[MyNumber].pushBack(fac);
					fac->setAnchorPoint(Vec2(0.5, 0.5));
					PlayMap->addChild(fac, 3);
					fac->setPosition(target);
					fac->SetType(1);*/

					buildRespone(SpawnDatastring(MyNumber, 'b', target.x, target.y, building));
					//sioClient->send(SpawnDatastring(MyNumber, 'b', target.x, target.y,building));
					//输出字符串“ %MyNumber b  %target.x  %target.y”
					building = 0;
				}
				else {
					for (auto sp_obj : SoldierList[MyNumber]) {									//遍历soldierlist容器元素，将第一个被鼠标点中的soldier变为被选中状态
						if (ContainSprite(sp_obj, target, 20) && !sp_obj->Died()) {
							for (auto sp_objt : SoldierList[MyNumber]) {
								sp_objt->Select(0);
								sp_objt->SelectedReply();
							}
							//log("%d", sp_obj->getTag());
							sp_obj->Select(1);
							sp_obj->SelectedReply();
							swallow = 1;												//吞噬标志，避免点击事件继续触发接下来的if
							break;
						}
					}
					if (!swallow) {
						for (int i = 0; i < 4; i++) {
							if (i % 2 != MyNumber % 2) {
								for (auto enemy : SoldierList[i]) {								//点中敌方soldier，给所有选中目标设置攻击target
									if (ContainSprite(enemy, target, 20) && !enemy->Died()) {
										for (auto myso : SoldierList[MyNumber]) {
											if (myso->Selected()) {
												//myso->SetTarget(enemy);
												//输出“%MyNumber a %myso->getTag() %enemy->getTag()” 
												attackRespone(SpawnDatastring(MyNumber, 'a', myso->getTag(), enemy->getTag()));
												//sioClient->send(SpawnDatastring(MyNumber, 'a', myso->getTag(), enemy->getTag()));
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
					if (!swallow) {														//没有点中soldier，判断是否点中building作为攻击对象
						for (int i = 0; i < 4; i++) {
							if (i % 2 != MyNumber % 2) {
								for (auto enemy : BuildingList[i]) {								//点中敌方soldier，给所有选中目标设置攻击target
									if (ContainSprite(enemy, target, 50) && !enemy->Destroyed()) {
										for (auto myso : SoldierList[MyNumber]) {
											if (myso->Selected()) {
												//myso->SetTarget(enemy);
												//输出“%MyNumber a %myso->getTag() %enemy->getTag()” 
												destroyRespone(SpawnDatastring(MyNumber, 'a', myso->getTag(), enemy->getTag()));
												//sioClient->send(SpawnDatastring(MyNumber, 'a', myso->getTag(), enemy->getTag()));
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

								//sioClient->send(SpawnDatastring(MyNumber, 'a', sp_obj->getTag(), 0));
								//sioClient->send(SpawnDatastring(MyNumber, 'd', sp_obj->getTag(), 0));
								//sioClient->send(SpawnDatastring(MyNumber, 'm', sp_obj->getTag(), target.x, target.y));

								attackRespone(SpawnDatastring(MyNumber, 'a', sp_obj->getTag(), 0));
								destroyRespone(SpawnDatastring(MyNumber, 'd', sp_obj->getTag(), 0));

								moveRespone(SpawnDatastring(MyNumber, 'm', sp_obj->getTag(), target.x, target.y));

							}
						}
					}
				}
			}
			swallow = 0;									//重置吞噬标志









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
		Vec2 target = position - PlayMap->getPosition();
		//target = target * 0.78125;
		//log("%f %f",position.x,position.y);
		//log("%f %f", target.x, target.y);
		//map->stopAllActions();							
		if (position.x > xView - 100) {							//鼠标靠近屏幕右侧
			if (!(PlayMap->numberOfRunningActions())) {
				auto map_move = MoveTo::create((PlayMap->getPosition().x + (4000 - xView)) / 2000, Vec2(-(4000 - xView), PlayMap->getPosition().y));
				PlayMap->runAction(map_move);
			}
		}
		else if (position.x < 100) {							//鼠标靠近屏幕边缘左侧，将视角左移，即将地图右移
			if (!(PlayMap->numberOfRunningActions())) {
				auto map_move = MoveTo::create((-(PlayMap->getPosition().x)) / 2000, Vec2(0, PlayMap->getPosition().y));
				PlayMap->runAction(map_move);
			}
		}
		else if (position.y < 100) {							//鼠标靠近边缘上侧
			if (!(PlayMap->numberOfRunningActions())) {
				auto map_move = MoveTo::create((-(PlayMap->getPosition().y)) / 2000, Vec2(PlayMap->getPosition().x, 0));
				PlayMap->runAction(map_move);
			}
		}
		else if (position.y > yView - 100) {					//下侧
			if (!(PlayMap->numberOfRunningActions())) {
				auto map_move = MoveTo::create((PlayMap->getPosition().y + (4000 - yView)) / 2000, Vec2(PlayMap->getPosition().x, -(4000 - yView)));
				PlayMap->runAction(map_move);
			}
		}
		else {
			PlayMap->stopAllActions();								//移到屏幕中央时停止移动
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
				PlayMap->addChild(fac, 3);
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


	//content
	/*auto content = RichText::create();
	content->setContentSize(Size(400, 200));
	content->ignoreContentAdaptWithSize(false);
	content->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	content->setTag(112);
	this->addChild(content);
	auto text = RichElementText::create(1, Color3B::RED, 255, "Hi!", "Arial", 20);
	content->pushBackElement(text);*/

	//chat
	/*auto chatbox = EditBox::create(Size(400, 20), Scale9Sprite::create("editbox.jpg"));
	chatbox->setPlaceHolder("Press Enter to chat: ");
	chatbox->setFontColor(Color3B::BLACK);
	chatbox->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	chatbox->setTag(111);
	this->addChild(chatbox);*/

	/*auto sendItem = MenuItemFont::create("send", CC_CALLBACK_1(GameScene::sendCallback, this));
	sendItem->setPosition(Vec2(origin.x, origin.y + visibleSize.height / 3));
	Menu *mn = Menu::create(sendItem, NULL);
	this->addChild(mn);*/

	//sound
	/*if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Ready The Army.mp3", true);
		//检验背景音乐是否正在播放
		bool flag = &SimpleAudioEngine::isBackgroundMusicPlaying;
		log("%d", flag);
	}*/

	//return

	return true;
}

void GameScene::sendCallback(Ref *pSender)
{
	int playerNumber = UserDefault::getInstance()->getIntegerForKey(PLAYER_NUMBER);
	std::string message = std::to_string(playerNumber);
	std::string playerName = UserDefault::getInstance()->getStringForKey(USER_NAME, "User");
	auto editbox = reinterpret_cast<EditBox*>(this->getChildByTag(111));
	std::string content = editbox->getText();
	message = message + "c" + playerName + ":" + content;
	sioClient->send(message);
	return;
}

void GameScene::onConnect(cocos2d::network::SIOClient *client)
{
	return;
}

void GameScene::onMessage(cocos2d::network::SIOClient *client, const std::string& data)
{
	//const char *cData = data.c_str();
	/*从服务端接受指令并根据指令执行相应的函数*/
	//log("get");
	if (data[3] == 'c') {
		//log("1");
		//chatResponse(data);
		createRespone(data);
	}
	else if (data[3] == 'b') {
		//log("1");
		//chatResponse(data);
		buildRespone(data);
	}
	else if (data[3] == 'a') {
		attackRespone(data);
	}
	else if (data[3] == 'm') {
		moveRespone(data);
	}
	else if (data[3] == 'd') {
		destroyRespone(data);
	}
	return;
}

void GameScene::onClose(cocos2d::network::SIOClient *client)
{
	return;
}

void GameScene::onError(cocos2d::network::SIOClient *client, const std::string& data)
{
	return;
}

void GameScene::chatResponse(const std::string& data)
{
	/*根据data[0]决定添加富文本的颜色暂时懒得实现*/
	auto content = reinterpret_cast<RichText*>(this->getChildByTag(112));
	//提取聊天内容
	std::string message = data;
	message.erase(0, 3);
	log("%s", message.c_str());
	auto re1 = RichElementNewLine::create(0, Color3B::WHITE, 255);
	auto re2 = RichElementText::create(1, Color3B::WHITE, 255, message, "Arial", 20);
	content->pushBackElement(re1);
	content->pushBackElement(re2);
	return;
}

/*void GameScene::updateErase(float di) {

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
}*/

void GameScene::attackRespone(const std::string &data) {
	int index = 0;
	int Player = data[index] - 48;
	char atk[5];
	char tgt[5];
	int atk_tag;
	int tgt_tag;
	index += 4;
	int a = 0;
	for (index; data[index] != 32; index++) {

		atk[a] = data[index];
		a++;
	}
	atk_tag = atoi(atk);
	index++;
	a = 0;
	for (index; data[index] && data[index] != 32; index++) {

		tgt[a] = data[index];
		a++;
	}
	tgt_tag = atoi(tgt);

	for (auto sd : SoldierList[Player]) {
		if (sd->getTag() == atk_tag) {
			bool find = 0;
			if (tgt_tag == 0) {
				sd->SetTarget(nullptr);
				break;
			}
			for (int i = (Player + 1) % 2; i < 4; i = i + 2) {
				for (auto en : SoldierList[i]) {
					if (en->getTag() == tgt_tag) {
						sd->SetTarget(en);
						find = 1;
						break;
					}
				}
				if (find) {
					break;
				}
			}
			break;
		}
	}
	//sd=PlayMap->getChildByTag(atk_tag)
}

void GameScene::destroyRespone(const std::string &data) {
	int index = 0;
	int Player = data[index] - 48;
	char atk[5];
	char tgt[5];
	int atk_tag;
	int tgt_tag;
	index += 4;
	int a = 0;
	for (index; data[index] != 32; index++) {

		atk[a] = data[index];
		a++;
	}
	atk_tag = atoi(atk);
	index++;
	a = 0;
	for (index; data[index] && data[index] != 32; index++) {

		tgt[a] = data[index];
		a++;
	}
	tgt_tag = atoi(tgt);

	for (auto sd : SoldierList[Player]) {
		if (sd->getTag() == atk_tag) {
			bool find = 0;
			if (tgt_tag == 0) {
				sd->SetDestroy(nullptr);
				break;
			}
			for (int i = (Player + 1) % 2; i < 4; i = i + 2) {
				for (auto en : BuildingList[i]) {
					if (en->getTag() == tgt_tag) {
						sd->SetDestroy(en);
						find = 1;
						break;
					}
				}
				if (find) {
					break;
				}
			}
			break;
		}
	}
	//sd=PlayMap->getChildByTag(atk_tag)
}

void GameScene::buildRespone(const std::string &data) {
	//log("create");
	int index = 0;
	int Player = data[index] - 48;
	char x_c[4];
	char y_c[4];
	char type_c[1];
	int x;
	int y;
	int type;
	index += 4;
	int a = 0;
	for (index; data[index] != 32; index++) {

		x_c[a] = data[index];
		a++;
	}
	x = atoi(x_c);
	index++;
	a = 0;
	for (index; data[index] && data[index] != 32; index++) {

		y_c[a] = data[index];
		a++;
	}
	y = atoi(y_c);
	index++;
	type_c[0] = data[index];
	type = atoi(type_c);

	switch (type) {
	case 1:
		auto fac = Building::create("bd.png");
		fac->SetType(1);
		fac->SetSide(Player);
		BuildingList[Player].pushBack(fac);
		fac->setAnchorPoint(Vec2(0.5, 0.5));
		PlayMap->addChild(fac, 3);
		fac->setPosition(Vec2(x, y));
		//fac->scheduleUpdate();
		fac->schedule(schedule_selector(Building::updateBuild), 0.1f, 100, 0);
		fac->scheduleOnce(schedule_selector(Building::updateBegin), 0);
		break;
	}




	//log("%f %f", fac->getPosition().x, fac->getPosition().y);
	/*log("Player: %d", Player);
	log("MyNumber: %d", Player);
	log("size:  %d", SoldierList[Player].size());*/
}

void GameScene::createRespone(const std::string &data) {
	int index = 0;
	int Player = data[index] - 48;
	char x_c[4];
	char y_c[4];
	int x;
	int y;
	index += 4;
	int a = 0;
	for (index; data[index] != 32; index++) {

		x_c[a] = data[index];
		a++;
	}
	x = atoi(x_c);
	index++;
	a = 0;
	for (index; data[index] && data[index] != 32; index++) {

		y_c[a] = data[index];
		a++;
	}
	y = atoi(y_c);



	Soldier* soldier = Soldier::create("seedz.png");
	soldier->setPosition(x, y);
	soldier->SetSide(Player);
	//soldier->setTag(1);
	soldier->scheduleUpdate();
	soldier->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
	soldier->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);
	PlayMap->addChild(soldier, 100);
	SoldierList[Player].pushBack(soldier);
	//log("%f %f", soldier->getPosition().x, soldier->getPosition().y);
	//log("%s", data);
	log("Player: %d", Player);
	log("MyNumber: %d", Player);
	log("size:  %d", SoldierList[Player].size());
}

void GameScene::moveRespone(const std::string &data) {
	int index = 0;
	int Player = data[index] - 48;
	char tag_c[5];
	char x_c[4];
	char y_c[4];
	int tag;
	int x;
	int y;
	index += 4;
	int a = 0;
	for (index; data[index] != 32; index++) {
		tag_c[a] = data[index];
		a++;
	}
	tag = atoi(tag_c);
	index++;
	a = 0;
	for (index; data[index] != 32; index++) {

		x_c[a] = data[index];
		a++;
	}
	x = atoi(x_c);
	index++;
	a = 0;
	for (index; data[index] && data[index] != 32; index++) {

		y_c[a] = data[index];
		a++;
	}
	y = atoi(y_c);

	for (auto sd : SoldierList[Player]) {
		if (sd->getTag() == tag) {
			sd->SetDestination(Vec2(x, y));
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

string SpawnDatastring(int Player, char type, int n, int m) {
	string data;
	char tp[5];
	char tpx[5];
	char tpy[5];
	_itoa_s(Player, tp, 10);
	data += tp;
	data += " ";
	data += type;
	data += " ";
	_itoa_s(n, tpx, 10);
	_itoa_s(m, tpy, 10);
	data += tpx;
	data += " ";
	data += tpy;
	data += " ";
	return data;
}
string SpawnDatastring(int Player, char type, int n, int m, int t) {
	string data;
	char tp[5];
	char tpx[5];
	char tpy[5];
	char tpz[5];
	_itoa_s(Player, tp, 10);
	data += tp;
	data += " ";
	data += type;
	data += " ";
	_itoa_s(n, tpx, 10);
	_itoa_s(m, tpy, 10);
	_itoa_s(t, tpz, 10);
	data += tpx;
	data += " ";
	data += tpy;
	data += " ";
	data += tpz;
	data += " ";
	return data;
}