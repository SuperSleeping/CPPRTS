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
std::vector <Sprite*>MiniSoldierList[4];
std::vector<Sprite*> MiniBuildingList[4];
//static int SoldierTag = 0;
static int MyNumber;
static TMXTiledMap *PlayMap;
time_t initTime;
time_t Time;
Label* gametime;
Label* resources_gold;
Label* resources_gold_per_second;
Label* resources_power_avaliable;
Label* resources_power_sum;
Sprite* menu;
Sprite* jidi_g;
Sprite* dianchang_g;
Sprite* kuangchang_g;
Sprite* bingying_g;
Sprite* chechang_g;
Sprite* em;
Vec2 clickinit;

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
	sioClient->on("numberClientEvent", CC_CALLBACK_2(GameScene::numberClientEvent, this));

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto timebg = Sprite::create("timebg.png");
	timebg->setPosition(Vec2(origin.x + visibleSize.width / 6, origin.y + visibleSize.height * 17 / 18));
	this->addChild(timebg, 400);
	gametime = Label::create("", "Arial", 20);
	gametime->setPosition(Vec2(origin.x + visibleSize.width / 6, origin.y + visibleSize.height * 17 / 18));
	this->addChild(gametime, 400);
	time(&initTime);

	resources_gold = Label::create("", "Arial", 20);
	this->addChild(resources_gold, 400);
	resources_gold->setPosition(Vec2(1400, 880));


	resources_gold_per_second = Label::create("", "Arial", 20);
	this->addChild(resources_gold_per_second, 400);
	resources_gold_per_second->setPosition(Vec2(1500, 880));

	resources_power_avaliable = Label::create("", "Arial", 20);
	this->addChild(resources_power_avaliable, 400);
	resources_power_avaliable->setPosition(Vec2(1400, 860));

	resources_power_sum = Label::create("", "Arial", 20);
	this->addChild(resources_power_sum, 400);
	resources_power_sum->setPosition(Vec2(1500, 860));

	//���䴮��  p    a      t   xxxxyyy  
	//��� ����   ����
				//��ȡ��ұ��0/1/2/3  
							//0��2 1��3һ��


														//�˵���
	menu = Sprite::create("menucube.png");
	this->addChild(menu, 100);
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(Vec2(1100, 0));

	auto jidi = Sprite::create("jidi2.png");
	menu->addChild(jidi, 1);
	jidi->setAnchorPoint(Vec2(0, 0));
	jidi->setPosition(0, 100);

	auto dianchang = Sprite::create("dianchang2.png");
	menu->addChild(dianchang, 1);
	dianchang->setAnchorPoint(Vec2(0, 0));
	dianchang->setPosition(100, 100);

	auto kuangchang = Sprite::create("kuangchang2.png");
	menu->addChild(kuangchang, 1);
	kuangchang->setAnchorPoint(Vec2(0, 0));
	kuangchang->setPosition(200, 100);

	auto bingying = Sprite::create("bingying2.png");
	menu->addChild(bingying, 1);
	bingying->setAnchorPoint(Vec2(0, 0));
	bingying->setPosition(300, 100);

	auto chechang = Sprite::create("chechang2.png");
	menu->addChild(chechang, 1);
	chechang->setAnchorPoint(Vec2(0, 0));
	chechang->setPosition(400, 100);


	//��ɫ��ť
	jidi_g = Sprite::create("jidi3.png");
	menu->addChild(jidi_g, -2);
	jidi_g->setAnchorPoint(Vec2(0, 0));
	jidi_g->setPosition(0, 100);

	dianchang_g = Sprite::create("dianchang3.png");
	menu->addChild(dianchang_g, -2);
	dianchang_g->setAnchorPoint(Vec2(0, 0));
	dianchang_g->setPosition(100, 100);

	kuangchang_g = Sprite::create("kuangchang3.png");
	menu->addChild(kuangchang_g, -2);
	kuangchang_g->setAnchorPoint(Vec2(0, 0));
	kuangchang_g->setPosition(200, 100);

	bingying_g = Sprite::create("bingying3.png");
	menu->addChild(bingying_g, -2);
	bingying_g->setAnchorPoint(Vec2(0, 0));
	bingying_g->setPosition(300, 100);

	chechang_g = Sprite::create("chechang3.png");
	menu->addChild(chechang_g, -2);
	chechang_g->setAnchorPoint(Vec2(0, 0));
	chechang_g->setPosition(400, 100);


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

	MyNumber = 0;
	//MyNumber = UserDefault::getInstance()->getIntegerForKey(PLAYER_NUMBER) ;
	

	PlayMap = TMXTiledMap::create("newmap.tmx");
	addChild(PlayMap);
	auto minimap = Sprite::create("tem/minimap.png");
	minimap->setPosition(Vec2(150, 150));
	this->addChild(minimap, 300);
	em = Sprite::create("tem/em.png");
	em->setPosition(Vec2(em->getContentSize().width / 2, em->getContentSize().height / 2));
	em->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(em, 300);
	for (int i = 0; i < 100; ++i)
	{
		auto s = Sprite::create("tem/sb.png");
		s->setPosition(Vec2(xView * 2, yView * 2));
		this->addChild(s, 400);
		MiniSoldierList[0].push_back(s);
		auto b = Sprite::create("tem/bb.png");
		b->setPosition(Vec2(xView * 2, yView * 2));
		this->addChild(b, 400);
		MiniBuildingList[0].push_back(b);
	}
	for (int i = 0; i < 100; ++i)
	{
		auto s = Sprite::create("tem/sp.png");
		s->setPosition(Vec2(xView * 2, yView * 2));
		this->addChild(s, 400);
		MiniSoldierList[1].push_back(s);
		auto b = Sprite::create("tem/bp.png");
		b->setPosition(Vec2(xView * 2, yView * 2));
		this->addChild(b, 400);
		MiniBuildingList[1].push_back(b);
	}
	for (int i = 0; i < 100; ++i)
	{
		auto s = Sprite::create("tem/sr.png");
		s->setPosition(Vec2(xView * 2, yView * 2));
		this->addChild(s, 400);
		MiniSoldierList[2].push_back(s);
		auto b = Sprite::create("tem/br.png");
		b->setPosition(Vec2(xView * 2, yView * 2));
		this->addChild(b, 400);
		MiniBuildingList[2].push_back(b);
	}
	for (int i = 0; i < 100; ++i)
	{
		auto s = Sprite::create("tem/sy.png");
		s->setPosition(Vec2(xView * 2, yView * 2));
		this->addChild(s, 400);
		MiniSoldierList[3].push_back(s);
		auto b = Sprite::create("tem/by.png");
		b->setPosition(Vec2(xView * 2, yView * 2));
		this->addChild(b, 400);
		MiniBuildingList[3].push_back(b);
	}
	switch (MyNumber) {
	case 0:
		break;
	case 2:

		PlayMap->setPosition(Vec2(0, -3100));
		break;
	case 1:
		PlayMap->setPosition(Vec2(-2400, 0));
		break;
	case 3:
		PlayMap->setPosition(Vec2(-2400, -3100));
		break;
	}
	//map->setScale(1 / 0.78125, 1 / 0.78125);

	//log("%f %f", map->getContentSize().width, map->getContentSize().height);


	/*auto testenem = Soldier::create("enemy.png");
	testenem->SetSide(2);
	testenem->setPosition(Vec2(400, 400));
	SoldierList[1].pushBack(testenem);
	PlayMap->addChild(testenem, 100);
	testenem->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);
	testenem->scheduleUpdate();
	testenem->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);*/


	/*auto testmy = Soldier::create("seedz.png");
	testmy->SetSide(1);
	testmy->setPosition(Vec2(300, 400));
	SoldierList[MyNumber].pushBack(testmy);
	PlayMap->addChild(testmy, 100);
	testmy->scheduleUpdate();
	testmy->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
	testmy->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);


	testenem->SetTarget(testmy);

	buildRespone(SpawnDatastring(1, 'b', 300, 300, 1));*/

	static auto MouseReply = EventListenerMouse::create();

	MouseReply->onMouseDown = [=](Event *event) {
		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocationInView();
		Vec2 target = position - PlayMap->getPosition();
		clickinit = position;
	};

	MouseReply->onMouseUp = [=](Event *event) {					//������굯���¼�
		//string str = "11";
		//sioClient->send(str);
		//log("%d", SoldierList[0].size());
		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocationInView();					//����¼�����Ļ����ϵλ��
		Vec2 target = position - PlayMap->getPosition();			//����¼��ĵ�ͼ����ϵλ��
		auto k = (position.x - clickinit.x)*(position.x - clickinit.x) + (position.y - clickinit.y)*(position.y - clickinit.y);
																	//target = target * 0.78125;							
																	//log("%f %f", e->getLocationInView().x, e->getLocationInView().y);
		if (!bool(e->getMouseButton())) {					//left button ���
			if (k >= 5)
			{
				for (auto soldier : SoldierList[MyNumber])
				{
					auto m = soldier->getPosition();
					if (ContainRect(m, clickinit, position))
					{
						soldier->Select(1);
						soldier->SelectedReply();
					}
					else
					{
						soldier->Select(0);
						soldier->SelectedReply();
					}
				}
			}
			else if (ContainRect(position, Vec2(0, 300), Vec2(300, 0))) {           //�����Χ������С��ͼ
				if (!(PlayMap->numberOfRunningActions())) {				
					auto map_move = MoveTo::create(1.0f, Vec2(-position.x * 40 / 3 + 800, -position.y * 40 / 3 + 450));
					PlayMap->runAction(map_move);
				}
			}
			else if (ContainRect(position, Vec2(1100, 300), Vec2(1600, 0))) {		//�����Χ���������¿�
				if (ContainRect(position, Vec2(1100, 300), Vec2(1600, 200))) {
					for (Building* factory : BuildingList[MyNumber]) {

						//if (factory->Type() == 4 && !factory->Destroyed() && Gold[MyNumber] >= 500) {

							//sioClient->send(SpawnDatastring(MyNumber, 'c', (factory->getPosition() - Vec2(0, 75)).x, (factory->getPosition() - Vec2(0, 75)).y));
							createRespone(SpawnDatastring(MyNumber, 'c', (factory->getPosition() - Vec2(0, 75)).x, (factory->getPosition() - Vec2(0, 75)).y));
							for (auto sp_obj : SoldierList[MyNumber]) {										//����soldierlist����Ԫ�أ�ȫ�����δѡ��״̬
								sp_obj->Select(0);
								sp_obj->SelectedReply();
							}
						//}
					}
				}
				else if (ContainRect(position, Vec2(1100, 200), Vec2(1600, 100))) {				//�����Χ���������·������Ǹ��򣬱�־���뽨��״̬
					if (ContainRect(position, Vec2(1100, 200), Vec2(1200, 100))) {
						//if (!ConstructionYard[MyNumber]) {												//����
							building = 1;
					//	}
						//else {
							//���������л���
						//}
					}
					else  if (ContainRect(position, Vec2(1200, 200), Vec2(1300, 100))) {		//�糧
						//if (ConstructionYard[MyNumber] && Gold[MyNumber] >= 1000) {
							building = 2;
						//}
						//else {
							//������ǰ����������-��Ҫ����
						//}
					}
					else if (ContainRect(position, Vec2(1300, 200), Vec2(1400, 100))) {			//��
						//if (PowerPlant[MyNumber] && Power[MyNumber] >= 50 && Gold[MyNumber]>=2000) {
							building = 3;
						//}
						//else {
							//������ǰ����������-ȱ�ٵ糧���㹻����
						//}
					}
					else if (ContainRect(position, Vec2(1400, 200), Vec2(1500, 100))) {			//��Ӫ
						//if (PowerPlant[MyNumber] && Power[MyNumber] >= 50 && Gold[MyNumber]>=1000) {
							building = 4;
						//}
						//else {
							//������ǰ����������-ȱ�ٵ糧���㹻����
						//}
					}
					else if (ContainRect(position, Vec2(1500, 200), Vec2(1600, 100))) {
						//if (PowerPlant[MyNumber] && OreRefinery[MyNumber] && Power[MyNumber] >= 100 &&Gold[MyNumber]>=2000) {
							building = 5;
						//}
						//else {
							//������ǰ����������-ȱ�ٵ糧/�󳧻��㹻����
						//}
					}


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



					buildRespone(SpawnDatastring(MyNumber, 'b', target.x, target.y, building));
					//sioClient->send(SpawnDatastring(MyNumber, 'b', target.x, target.y,building));
					//����ַ����� %MyNumber b  %target.x  %target.y��
					building = 0;
				}
				else {
					for (auto sp_obj : SoldierList[MyNumber]) {									//����soldierlist����Ԫ�أ�����һ���������е�soldier��Ϊ��ѡ��״̬
						if (ContainSprite(sp_obj, target, 20) && !sp_obj->Died()) {
							for (auto sp_objt : SoldierList[MyNumber]) {
								sp_objt->Select(0);
								sp_objt->SelectedReply();
							}
							//log("%d", sp_obj->getTag());
							sp_obj->Select(1);
							sp_obj->SelectedReply();
							swallow = 1;												//���ɱ�־���������¼�����������������if
							break;
						}
					}
					if (!swallow) {
						for (int i = 0; i < 4; i++) {
							if (i % 2 != MyNumber % 2) {
								for (auto enemy : SoldierList[i]) {								//���ез�soldier��������ѡ��Ŀ�����ù���target
									if (ContainSprite(enemy, target, 20) && !enemy->Died()) {
										for (auto myso : SoldierList[MyNumber]) {
											if (myso->Selected()) {
												//myso->SetTarget(enemy);
												//�����%MyNumber a %myso->getTag() %enemy->getTag()�� 
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
					if (!swallow) {														//û�е���soldier���ж��Ƿ����building��Ϊ��������
						for (int i = 0; i < 4; i++) {
							if (i % 2 != MyNumber % 2) {
								for (auto enemy : BuildingList[i]) {								//���ез�soldier��������ѡ��Ŀ�����ù���target
									if (ContainSprite(enemy, target, 50) && !enemy->Destroyed()) {
										for (auto myso : SoldierList[MyNumber]) {
											if (myso->Selected()) {
												//myso->SetTarget(enemy);
												//�����%MyNumber a %myso->getTag() %enemy->getTag()�� 
												destroyRespone(SpawnDatastring(MyNumber, 'd', myso->getTag(), enemy->getTag()));
												//sioClient->send(SpawnDatastring(MyNumber, 'd', myso->getTag(), enemy->getTag()));
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
			swallow = 0;									//�������ɱ�־









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
		Vec2 target = position - PlayMap->getPosition();
		//target = target * 0.78125;
		//log("%f %f",position.x,position.y);
		//log("%f %f", target.x, target.y);
		//map->stopAllActions();							
		if (position.x > xView - 100) {							//��꿿����Ļ�Ҳ�
			if (!(PlayMap->numberOfRunningActions())) {
				auto map_move = MoveTo::create((PlayMap->getPosition().x + (4000 - xView)) / 2000, Vec2(-(4000 - xView), PlayMap->getPosition().y));
				PlayMap->runAction(map_move);
			}
		}
		else if (position.x < 100) {							//��꿿����Ļ��Ե��࣬���ӽ����ƣ�������ͼ����
			if (!(PlayMap->numberOfRunningActions())) {
				auto map_move = MoveTo::create((-(PlayMap->getPosition().x)) / 2000, Vec2(0, PlayMap->getPosition().y));
				PlayMap->runAction(map_move);
			}
		}
		else if (position.y < 100) {							//��꿿����Ե�ϲ�
			if (!(PlayMap->numberOfRunningActions())) {
				auto map_move = MoveTo::create((-(PlayMap->getPosition().y)) / 2000, Vec2(PlayMap->getPosition().x, 0));
				PlayMap->runAction(map_move);
			}
		}
		else if (position.y > yView - 100) {					//�²�
			if (!(PlayMap->numberOfRunningActions())) {
				auto map_move = MoveTo::create((PlayMap->getPosition().y + (4000 - yView)) / 2000, Vec2(PlayMap->getPosition().x, -(4000 - yView)));
				PlayMap->runAction(map_move);
			}
		}
		else {
			PlayMap->stopAllActions();								//�Ƶ���Ļ����ʱֹͣ�ƶ�
		}
		if (building) {											//����״̬��ͬʱ��Ӧ��Ļ�ƶ����齨����������ƶ�
			string build_type;
			switch (building) {
			case 1:
				build_type = "jidi.png";
				break;
			case 2:
				build_type = "dianchang.png";
				break;
			case 3:
				build_type = "kuangchang.png";
				break;
			case 4:
				build_type = "bingying.png";
				break;
			case 5:
				build_type = "chechang.png";
				break;
			}
			auto factory = Sprite::create(build_type);
			if (!virtual_factory.empty()) {						//һ������ƶ����Ƴ��齨��������Ψһ��Ԫ�أ������µ��齨��Ԫ�ص���ǰ���λ��
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
		auto t = target - position;
		em->setPosition(Vec2(t.x * 3 / 40 + em->getContentSize().width / 2, t.y * 3 / 40 + em->getContentSize().height / 2));
	};

	auto dispatcher = Director::getInstance()->getEventDispatcher();					//�������¼�������
	dispatcher->addEventListenerWithSceneGraphPriority(MouseReply, this);

	//for (auto myso : MySoldierList) {
	//myso->scheduleUpdate();
	//myso->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
	//}
	scheduleUpdate();


	//content
	auto content = RichText::create();
	content->setContentSize(Size(400, 200));
	content->ignoreContentAdaptWithSize(false);
	content->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	content->setTag(112);
	this->addChild(content);
	auto text = RichElementText::create(1, Color3B::RED, 255, "Hi!", "Arial", 20);
	content->pushBackElement(text);

	//chat
	auto chatbox = EditBox::create(Size(400, 20), Scale9Sprite::create("editbox.jpg"));
	chatbox->setPlaceHolder("Press Enter to chat: ");
	chatbox->setFontColor(Color3B::BLACK);
	chatbox->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	chatbox->setTag(111);
	chatbox->setVisible(false);
	chatbox->setInputMode(EditBox::InputMode::SINGLE_LINE);
	this->addChild(chatbox);

	auto sendMessage = EventListenerKeyboard::create();
	sendMessage->onKeyPressed = CC_CALLBACK_2(GameScene::sendCallback,this);
	_eventDispatcher->addEventListenerWithFixedPriority(sendMessage, 1);
	/*auto sendItem = MenuItemFont::create("send", CC_CALLBACK_1(GameScene::sendCallback, this));
	sendItem->setPosition(Vec2(origin.x, origin.y + visibleSize.height / 3));
	Menu *mn = Menu::create(sendItem, NULL);
	this->addChild(mn);*/

	//sound
	/*if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Ready The Army.mp3", true);
		//���鱳�������Ƿ����ڲ���
		bool flag = &SimpleAudioEngine::isBackgroundMusicPlaying;
		log("%d", flag);
	}*/

	//return
	this->schedule(schedule_selector(GameScene::updateMini), 0.5f, kRepeatForever, 0);
	this->schedule(schedule_selector(GameScene::updateTime), 1.0f, kRepeatForever, 0);
	this->schedule(schedule_selector(GameScene::updateResources), 1.0f, kRepeatForever, 0);
	this->schedule(schedule_selector(GameScene::updateGrayButton), 0.5f, kRepeatForever, 0);
	return true;
}

void GameScene::sendCallback(EventKeyboard::KeyCode keyCode,Event *event)
{
	auto editbox = reinterpret_cast<EditBox*>(this->getChildByTag(111));
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER)
	{
		if (!editbox->isVisible())
		{
			editbox->setVisible(true);
		}
		else
		{
			int playerNumber = UserDefault::getInstance()->getIntegerForKey(PLAYER_NUMBER);
			std::string message = std::to_string(playerNumber);
			std::string playerName = UserDefault::getInstance()->getStringForKey(USER_NAME, "User");
			std::string content = editbox->getText();
			if (content.length())
			{
				message = message + " p" + playerName + ":" + content;
				editbox->setText("");
				sioClient->send(message);
			}
			editbox->setVisible(false);
		}
	}
	return;
}

void GameScene::numberClientEvent(cocos2d::network::SIOClient *client, const std::string& data)
{
	std::string pNumber = data;
	log("Client Called");
	log("%s", data.c_str());
	pNumber.erase(0, 1);
	pNumber.pop_back();
	int number = atoi(pNumber.c_str());
	MyNumber = number;
	log("MyNumber: %d", MyNumber);
	switch (MyNumber) {
	case 0:
		break;
	case 2:

		PlayMap->setPosition(Vec2(0, -3100));
		break;
	case 1:
		PlayMap->setPosition(Vec2(-2400, 0));
		break;
	case 3:
		PlayMap->setPosition(Vec2(-2400, -3100));
		break;
	}
	UserDefault::getInstance()->setIntegerForKey(PLAYER_NUMBER, number);
	return;
}
void GameScene::onConnect(cocos2d::network::SIOClient *client)
{
	return;
}

void GameScene::onMessage(cocos2d::network::SIOClient *client, const std::string& data)
{
	//const char *cData = data.c_str();
	/*�ӷ���˽���ָ�����ָ��ִ����Ӧ�ĺ���*/
	//log("get");
	if (data[3] == 'p')
	{
		chatResponse(data);
	}
	else if (data[3] == 'c') {
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
	static int i = 0;
	/*����data[0]������Ӹ��ı�����ɫ��ʱ����ʵ��*/
	auto content = reinterpret_cast<RichText*>(this->getChildByTag(112));
	int mSide = data[1];
	int mySide = UserDefault::getInstance()->getIntegerForKey(PLAYER_NUMBER);
	//��ȡ��������
	std::string message = data;
	message.erase(0, 4);
	message.pop_back();
	log("%s", message.c_str());
	if (mSide % 2 == mySide % 2)
	{
		auto re1 = RichElementNewLine::create(-1, Color3B::WHITE, 255);
		auto re2 = RichElementText::create(i, Color3B::WHITE, 255, message, "Arial", 20);
		content->pushBackElement(re1);
		content->pushBackElement(re2);
	}
	else
	{
		auto re1 = RichElementNewLine::create(-1, Color3B::RED, 255);
		auto re2 = RichElementText::create(i, Color3B::RED, 255, message, "Arial", 20);
		content->pushBackElement(re1);
		content->pushBackElement(re2);
	}
	i++;
	if (i > 4)
	{
		content->removeElement(i - 5);
	}
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
		//Building* fac;
	case 1: {
		auto fac = Building::create("jidi.png");
		fac->SetType(1);
		fac->SetSide(Player);
		BuildingList[Player].pushBack(fac);
		fac->setAnchorPoint(Vec2(0.5, 0.5));
		PlayMap->addChild(fac, 3);
		fac->setPosition(Vec2(x, y));
		//fac->scheduleUpdate();
		fac->schedule(schedule_selector(Building::updateBuild), 0.1f, 100, 0);
		fac->scheduleOnce(schedule_selector(Building::updateBegin), 0);
		break; }
	case 2: {
		auto fac = Building::create("dianchang.png");
		fac->SetType(2);
		fac->SetSide(Player);
		BuildingList[Player].pushBack(fac);
		fac->setAnchorPoint(Vec2(0.5, 0.5));
		PlayMap->addChild(fac, 3);
		fac->setPosition(Vec2(x, y));
		//fac->scheduleUpdate();
		fac->schedule(schedule_selector(Building::updateBuild), 0.1f, 100, 0);
		fac->scheduleOnce(schedule_selector(Building::updateBegin), 0);

		break; }
	case 3: {
		auto fac = Building::create("kuangchang.png");
		fac->SetType(3);
		fac->SetSide(Player);
		BuildingList[Player].pushBack(fac);
		fac->setAnchorPoint(Vec2(0.5, 0.5));
		PlayMap->addChild(fac, 3);
		fac->setPosition(Vec2(x, y));
		//fac->scheduleUpdate();
		fac->schedule(schedule_selector(Building::updateBuild), 0.1f, 100, 0);
		fac->scheduleOnce(schedule_selector(Building::updateBegin), 0);
		fac->schedule(schedule_selector(Building::updateOre), 1.0f, kRepeatForever, 10.0f);
		break; }
	case 4: {
		auto fac = Building::create("bingying.png");
		fac->SetType(4);
		fac->SetSide(Player);
		BuildingList[Player].pushBack(fac);
		fac->setAnchorPoint(Vec2(0.5, 0.5));
		PlayMap->addChild(fac, 3);
		fac->setPosition(Vec2(x, y));
		//fac->scheduleUpdate();
		fac->schedule(schedule_selector(Building::updateBuild), 0.1f, 100, 0);
		fac->scheduleOnce(schedule_selector(Building::updateBegin), 0);

		break; }
	case 5: {
		auto fac = Building::create("chechang.png");
		fac->SetType(5);
		fac->SetSide(Player);
		BuildingList[Player].pushBack(fac);
		fac->setAnchorPoint(Vec2(0.5, 0.5));
		PlayMap->addChild(fac, 3);
		fac->setPosition(Vec2(x, y));
		//fac->scheduleUpdate();
		fac->schedule(schedule_selector(Building::updateBuild), 0.1f, 100, 0);
		fac->scheduleOnce(schedule_selector(Building::updateBegin), 0);
		break; }
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
	Gold[Player] -= 500;
	//log("%f %f", soldier->getPosition().x, soldier->getPosition().y);
	//log("%s", data);
	//log("Player: %d", Player);
	//log("MyNumber: %d", Player);
	//log("size:  %d", SoldierList[Player].size());
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
void GameScene::updateTime(float di)
{
	double tem;
	time(&Time);
	tem = difftime(Time, initTime);
	int second = static_cast<int>(tem);
	int min = 0;
	int hour = 0;
	hour = tem / 3600;
	tem = tem - hour * 3600;
	min = tem / 60;
	tem = tem - min * 60;
	second = tem;
	gametime->setString(to_string(hour) + "h" + to_string(min) + "m" + to_string(second) + "s");
}

string GoldString(int player) {
	string str;
	char gd[10];
	char gp[4];
	_itoa_s(Gold[player], gd, 10);
	_itoa_s(OreRefinery[player] * 50, gp, 10);
	str += gd;
	str += "  ";
	str += gp;
	return str;

}

void GameScene::updateResources(float di) {
	resources_gold->setString(to_string(Gold[MyNumber]));
	resources_gold_per_second->setString(to_string(OreRefinery[MyNumber] * 50) + " / second");
	resources_power_avaliable->setString(to_string(Power[MyNumber]));
	resources_power_sum->setString("/ " + to_string(PowerPlant[MyNumber] * 100));
}

void GameScene::updateGrayButton(float di) {
	if (ConstructionYard[MyNumber]) {
		jidi_g->setZOrder(2);
	}
	else {
		jidi_g->setZOrder(-2);
	}
}
void GameScene::updateMini(float di)
{
	//soldier
	for (int i = 0; i < 4; ++i)
	{
		int j = 0;
		for (auto soldier : SoldierList[i])
		{
			if (!soldier->Died())
			{
				auto p = soldier->getPosition();
				MiniSoldierList[i][j]->setPosition(Vec2(p.x * 3 / 40, p.y * 3 / 40));
			}
			if (soldier->Died())
			{
				auto p = Director::getInstance()->getVisibleSize();
				MiniSoldierList[i][j]->setPosition(Vec2(p.width * 2, p.height * 2));
			}
			j += 1;
		}
	}
	//building
	for (int i = 0; i < 4; ++i)
	{
		int j = 0;
		for (auto building : BuildingList[i])
		{
			auto p = building->getPosition();
			MiniBuildingList[i][j]->setPosition(Vec2(p.x * 3 / 40, p.y * 3 / 40));
			j += 1;
		}
	}
}