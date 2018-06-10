#include "PlayerDivision.h"
#include "Bean.h"
#include "Thorn.h"
#include "Spore.h"
#include "Macro.h"

PlayerDivision* PlayerDivision::create(const std::string& name,int skinID,int score)
{
	PlayerDivision* playerdivision = new PlayerDivision();
	if (playerdivision&&playerdivision->init(name,skinID,score))
	{
		playerdivision->autorelease();
		return playerdivision;
	}
	CC_SAFE_DELETE(playerdivision);
	return nullptr;
}
bool PlayerDivision::init(const std::string& name,int skinID,int score)
{
	
	if (!Entity::initWithFile(StringUtils::format("SKIN/skin_%d.png", skinID)))
	{
		return false;
	}

	_name = name;
	_score = score;
	_skinID = skinID;
	change();
}


bool PlayerDivision::collideBean(Bean* bean)
{
	Vec2 position = this->getPosition();
	Vec2 beanposition = bean->getPosition();
	if (beanposition.distance(position) <= _radius)
	{
		_score += bean->getScore();
		change();
		return true;
	}
	return false;
}

bool PlayerDivision::collideSpore(Spore* spore)
{
	Vec2 position = this->getPosition();
	Vec2 sporeposition = spore->getPosition();
	if (sporeposition.distance(position) <= _radius)
	{
		_score += spore->getScore();
		change();
		return true;
	}
	return false;
}

bool PlayerDivision::collideThorn(Thorn* thorn)
{
	Vec2 position = this->getPosition();
	Vec2 thornposition = thorn->getPosition();
	int thornscore = thorn->getScore();
	if (_score >= thornscore * 1.5)
	{
		if (thornposition.distance(position) <= (_radius - thorn->getRadius()))  //碰到刺会有特殊处理
			return true;
	}
	return false;
}

void PlayerDivision::setvector(Vec2 v)
{
	_vector = v;
}

Vec2 PlayerDivision::getvector()
{
	return _vector;
}

void PlayerDivision::setspeed(float s)
{
	_speed = s;
}

float PlayerDivision::getspeed()
{
	return _speed;
}

void PlayerDivision::setPlayerName(const std::string& name)
{
	_nameLabel = Label::createWithTTF(name.c_str(), "Marker Felt.ttf", 22);
	Size size = this->getContentSize();
	_nameLabel->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(_nameLabel);
}

void PlayerDivision::setPreposition(const Vec2& preposition)
{
	_preposition = preposition;
}

Vec2 PlayerDivision::getPreposition()
{
	return _preposition;
}

void PlayerDivision::scorehalve()
{
	_score = _score / 2;
	change();
}

void PlayerDivision::spitspore()
{
	_score -= SPORE_SCORE;
	change();
}

void PlayerDivision::change()
{
	_radius = PLAYER_INITIAL_RADIUS * PLAYER_INITIAL_RADIUS*_score / PLAYER_INITIAL_SCORE;
	_speed = (PLAYER_INITIAL_RADIUS / _radius)*PLAYER_INITIAL_SPEED + 1;

	this->setLocalZOrder(_score);

	Size size = this->getContentSize();
	float scale = _radius * 2 / size.width;
	this->setScale(scale);
}