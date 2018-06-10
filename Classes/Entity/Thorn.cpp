#include "Thorn.h"
#include "Macro.h"

Thorn* Thorn::create(const std::string& filename)
{
	Thorn* thorn = new Thorn();
	if (thorn&&thorn->init(filename))
	{
		thorn->autorelease();
		return thorn;
	}
	CC_SAFE_DELETE(thorn);
	return nullptr;
}

bool Thorn::init(const std::string& filename)
{
	if (!Entity::initWithSpriteFrameName(filename))
	{
		return false;
	}

	_score = THORN_SCORE;
	_radius = THORN_RADIUS;

	Size size = this->getContentSize();
	float scale = (_radius * 2) / size.width;
	this->setScale(scale);

	return true;
}