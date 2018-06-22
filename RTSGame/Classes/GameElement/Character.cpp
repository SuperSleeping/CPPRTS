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
	/*
	//�жϸò�����
	if (positionGoal == NULL ||
		positionCurrent == positionGoal)return;
	//�ƶ�
	float _distance = distance(positionCurrent, positionGoal);
	float time = _distance / velocity;
	*/

	//�ж��Ƿ�ı����յ�
	if (path.front() == positionGoal)
	{

	}
	//�������ϺͶ���˳��
	Vector<FiniteTimeAction*> moveToActions;
	Sequence* sequence = Sequence::create(moveToActions);

	while (1)
	{

	}
	//�ƶ�����һ����
	Point nextPosition = path.back();
	nextPosition = convertFromTMToWorld(nextPosition);
	//�ѽ�Ҫ�ƶ��ĵ��path��ɾ��
	path.pop_back();

	float _distance = distance(positionCurrent, nextPosition);
	float time = _distance / velocity;

	auto moveToAction = MoveTo::create(time, nextPosition);
	this->runAction(sequence);

	positionCurrent = nextPosition;
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