#include"GameScene.h"
#include"SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "UserDefault.h"
#include "extensions/cocos-ext.h"			//editbox

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
using namespace cocos2d::ui;

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

	////top label
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
	this->addChild(chatbox);

	auto sendItem = MenuItemFont::create("send", CC_CALLBACK_1(GameScene::sendCallback, this));
	sendItem->setPosition(Vec2(origin.x, origin.y + visibleSize.height / 3));
	Menu *mn = Menu::create(sendItem, NULL);
	this->addChild(mn);

	//sound
	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/Ready The Army.mp3", true);
		//检验背景音乐是否正在播放
		bool flag = &SimpleAudioEngine::isBackgroundMusicPlaying;
		log("%d", flag);
	}

	//return

	return true;
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