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
	drawRoutine = DrawNode::create();
}

Character* Character::create(const char* filename, Vec2 position)
{
	//创建GameElement中类似Sprite的部分
	Character* character = new (std::nothrow)Character;
	if (character && character->initWithFile(filename))
	{
		character->setPosition(position);
		//属性设置
		character->positionCurrent = position;
		//自动释放
		character->autorelease();
		return character;
	}
	CC_SAFE_DELETE(character);
	return nullptr;
}

void Character::move()
{
	/*
	//判断该不该走
	if (positionGoal == NULL ||
		positionCurrent == positionGoal)return;
	//移动
	float _distance = distance(positionCurrent, positionGoal);
	float time = _distance / velocity;
	*/

	//判断是否改变了终点
	if (path.front() == positionGoal)
	{

	}
	//动作集合和动作顺序
	Vector<FiniteTimeAction*> moveToActions;
	Sequence* sequence = Sequence::create(moveToActions);

	while (1)
	{

	}
	//移动到下一个点
	Point nextPosition = path.back();
	nextPosition = convertFromTMToWorld(nextPosition);
	//把将要移动的点从path中删除
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
	//倒叙加入path
	vector<Point>::iterator p;
	for (p = final_path.end(); p != final_path.begin(); p--)
	{
		path.push_back(*p);
	}
	*/
	
	//下一个目标点在path数组的末端
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