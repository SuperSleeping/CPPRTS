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


	static auto MouseReply = EventListenerMouse::create();
	MouseReply->onMouseUp = [=](Event *event) {					//������굯���¼�

		EventMouse* e = (EventMouse*)event;
		Vec2 position = e->getLocationInView();					//����¼�����Ļ����ϵλ��
		Vec2 target = position - PlayMap->getPosition();			//����¼��ĵ�ͼ����ϵλ��
		//target = target * 0.78125;							
		//log("%f %f", e->getLocationInView().x, e->getLocationInView().y);
		if (!bool(e->getMouseButton())) {					//left button ���
			if (ContainRect(position, Vec2(1100, 300), Vec2(1600, 0))) {		//�����Χ���������¿�
				if (ContainRect(position, Vec2(1100, 300), Vec2(1600, 200))) {
					for (Building* factory : BuildingList[MyNumber]) {

						if (factory->Type() == 1) {

							Soldier* soldier = Soldier::create("seedz.png");
							soldier->setPosition(factory->getPosition() - Vec2(0, 75));
							soldier->SetSide(MyNumber);
							//soldier->setTag(1);
							soldier->scheduleUpdate();
							soldier->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
							soldier->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);
							PlayMap->addChild(soldier, 100);
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
					PlayMap->addChild(fac, 3);
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
						for (int i = 0; i < 4; i++) {
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
		Vec2 target = position - PlayMap->getPosition();
		//target = target * 0.78125;
		//log("%f %f",position.x,position.y);
		//log("%f %f", target.x, target.y);
		//map->stopAllActions();							//��ͼ��СΪ2000*1500�����ĵ�ͼ���Ӧ����magic numbers
		if (position.x > xView - 100) {							//��꿿����Ļ�Ҳ�
			if (!(PlayMap->numberOfRunningActions())) {				//��һ����Ϊ�˵�ͼ�ӽ��ƶ���������
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

			auto factory = Sprite::create("bd.png");
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

void GameScene::attackRespone(const std::string &data) {

	int Player = data[0];
	char atk[5];
	char tgt[5];
	int atk_tag;
	int tgt_tag;
	int index = 4;
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

void GameScene::buildRespone(const std::string &data) {
	int Player = data[0];
	char x_c[4];
	char y_c[4];
	int x;
	int y;
	int index = 4;
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
	auto fac = Building::create("bd.png");								//�ڸ�λ�����ʵ�ʽ������ı��־building���˳�����״̬
	BuildingList[Player].pushBack(fac);
	fac->setAnchorPoint(Vec2(0.5, 0.5));
	PlayMap->addChild(fac, 3);
	fac->setPosition(Vec2(x,y));
	fac->SetType(1);
}

void GameScene::createRespone(const std::string &data) {
	int Player = data[0];
	char x_c[4];
	char y_c[4];
	int x;
	int y;
	int index = 4;
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
	soldier->setPosition(x,y);
	soldier->SetSide(Player);
	//soldier->setTag(1);
	soldier->scheduleUpdate();
	soldier->schedule(schedule_selector(Soldier::updateAttack), 1.0f, kRepeatForever, 0);
	soldier->scheduleOnce(schedule_selector(Soldier::updateBegin), 0);
	PlayMap->addChild(soldier, 100);
	SoldierList[Player].pushBack(soldier);
}

void GameScene::moveRespone(const std::string &data) {
	int Player = data[0];
	char tag_c[5];
	char x_c[4];
	char y_c[4];
	int tag;
	int x;
	int y;
	int index = 4;
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

void GameScene::sendCallback(Ref *pSender)
{
	int playerNumber = UserDefault::getInstance()->getIntegerForKey(PLAYER_NUMBER);
	std::string message = std::to_string(playerNumber);
	std::string playerName = UserDefault::getInstance()->getStringForKey(USER_NAME,"User");
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
	/*�ӷ���˽���ָ�����ָ��ִ����Ӧ�ĺ���*/
	const char *cData = data.c_str();
	/*从服务端接受指令并根据指令执行相应的函数*/
	const char *cData = data.c_str();
	/*从服务端接受指令并根据指令执行相应的函数*/
	if (data[2] == 'c')
	{
		log("1");
		chatResponse(data);
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