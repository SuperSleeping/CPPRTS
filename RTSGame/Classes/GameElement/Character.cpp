#include "Character.h"

extern Point convertFromTMToWorld(Point Position);

Character::Character()
{
	//����ѡ�б��
	shadow = Sprite::create("Game/rhombus.png");
	shadow->setPositionNormalized(Vec2(0.5, 0));
	shadow->setVisible(false);
	this->addChild(shadow, -1);
	//����Ѫ��
	bloodFrame = Sprite::create("Game/character/bloodFrame.png");
	bloodPicture = Sprite::create("Game/character/blood.png");
	bloodPicture->setAnchorPoint(Vec2(0, 0));
	bloodPicture->setPosition(Vec2(1, 1));
	bloodFrame->addChild(bloodPicture);
	bloodFrame->setPositionNormalized(Vec2(0.5, 1.2));
	this->addChild(bloodFrame,1);
	//����·������
	drawRoutine = DrawNode::create();
}

Character* Character::create(const char* filename, Vec2 position)
{
	//����GameElement������Sprite�Ĳ���
	Character* character = new (std::nothrow)Character;
	if (character && character->initWithFile(filename))
	{
		character->setPosition(position);
		//��������
		character->positionCurrent = position;
		//�Զ��ͷ�
		character->autorelease();
		return character;
	}
	CC_SAFE_DELETE(character);
	return nullptr;
}

void Character::move()
{
	//�ƶ�����һ����
	Point nextPosition = path.back();
	positionCurrent = nextPosition;
	nextPosition = convertFromTMToWorld(nextPosition);
	//�ѽ�Ҫ�ƶ��ĵ��path��ɾ��
	path.pop_back();

	auto moveToAction = MoveTo::create(0.2, nextPosition);
	this->runAction(moveToAction);
}

void Character::pathInit(vector<Point> &final_path)
{
	/*
	//�������path
	vector<Point>::iterator p;
	for (p = final_path.end(); p != final_path.begin(); p--)
	{
		path.push_back(*p);
	}
	*/
	
	//��һ��Ŀ�����path�����ĩ��
	for (Point p : final_path)
	{
		path.push_back(p);
	}
	/*
	//��·��
	vector<Point>::iterator p;
	for (p = path.begin(); p != path.end(); p++)
	{
		if (p == path.end() - 1)
		{
			Point one = *p;
			one = convertFromTMToWorld(one);
			drawRoutine->drawLine(one, positionCurrent, Color4F(0, 0, 0, 1));
		}
		else
		{
			Point one = *p;
			Point two = *(p + 1);
			one = convertFromTMToWorld(one);
			two = convertFromTMToWorld(two);
			drawRoutine->drawLine(one, two, Color4F(0, 0, 0, 1));
		}
	}
	*/
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

void Character::setPositionGoal(Point position)
{
	positionGoal = position;
	positionGoal_change = true;
}
