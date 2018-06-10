#include "Spore.h"
#include "Macro.h"


Spore* Spore::create(const std::string& filename)
{
	Spore* spore = new Spore();
	if (spore && spore->init(filename))
	{
		spore->autorelease();
		return spore;
	}
	CC_SAFE_DELETE(spore);
	return nullptr;
}
bool Spore::init(const std::string& filename)
{
	if (!Entity::initWithSpriteFrameName(filename))
	{
		return false;
	}

	_score = SPORE_SCORE;
	_radius = SPORE_RADIUS;

	Size size = this->getContentSize();
	float scale = (_radius * 2) / size.width;
	this->setScale(scale);

	return true;
}