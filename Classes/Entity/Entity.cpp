#include"Classes\Entity\Entity.h"

float Entity::getRadius()
{
	return _radius;
}

void Entity::setRadius(float radius)
{
	_radius = radius;
}

int Entity::setScore()
{
	return _score;
}

void Entity::setScore(int score)
{
	_score = score;
}