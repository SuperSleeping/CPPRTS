#include "Character.h"

extern Point convertFromTMToWorld(Point Position);

Character::Character()
{
	//载入选中标记
	shadow = Sprite::create("Game/rhombus.png");
	shadow->setPositionNormalized(Vec2(0.5, 0));
	shadow->setVisible(false);
	this->addChild(shadow, -1);
	//载入血条
	bloodFrame = Sprite::create("Game/character/bloodFrame.png");
	bloodPicture = Sprite::create("Game/character/blood.png");
	bloodPicture->setAnchorPoint(Vec2(0, 0));
	bloodPicture->setPosition(Vec2(1, 1));
	bloodFrame->addChild(bloodPicture);
	bloodFrame->setPositionNormalized(Vec2(0.5, 1.2));
	this->addChild(bloodFrame,1);
	//载入路径划线
	drawRoutineNote = DrawNode::create();
}

Character::~Character()
{
	delete routine;
}


Character* Character::create(const char* filename, Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Character* character = new (std::nothrow)Character;
	if (character && character->initWithFile(filename))
	{
		character->setPosition(position);
		//属性设置
		character->setPositionCurrent(position);
		character->setPositionGoal(position);
		//自动释放
		character->autorelease();
		return character;
	}
	CC_SAFE_DELETE(character);
	return nullptr;
}

void Character::move()
{
	//移动到下一个点
	//(final_path里是瓦片坐标)
	//(final_path从begin到end是从终点指向起点的过程)
	Point nextPosition = routine->final_path.back();
	nextPosition = convertFromTMToWorld(nextPosition);
	this->setPositionCurrent(nextPosition);

	//把将要移动到的点从path中删除
	routine->final_path.pop_back();

	auto moveToAction = MoveTo::create(0.1, nextPosition);
	this->runAction(moveToAction);
}

void Character::drawRoutine()
{
	//画路径
	vector<Point>::iterator p;
	for (p = routine->final_path.begin(); p != routine->final_path.end(); p++)
	{
		if (p == routine->final_path.end() - 1)
		{
			Point one = *p;
			one = convertFromTMToWorld(one);
			drawRoutineNote->drawLine(one, positionCurrent, Color4F(0, 0, 0, 1));
		}
		else
		{
			Point one = *p;
			Point two = *(p + 1);
			one = convertFromTMToWorld(one);
			two = convertFromTMToWorld(two);
			drawRoutineNote->drawLine(one, two, Color4F(0, 0, 0, 1));
		}
	}
}

void Character::setSelected(bool isSelected)
{
	if (isSelected)
	{
		this->selected = true;
		this->shadow->setVisible(true);
	}
	else
	{
		this->selected = false;
		this->shadow->setVisible(false);
	}
}

