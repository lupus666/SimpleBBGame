#ifndef _Entity_H_
#define _Entity_H_

#include"cocos2d.h"

USING_NS_CC;

class Entity : public Sprite
{
public:
	float getRadius();
	void setRadius(float radius);
	int get Score();
	virtual void setScore(int score);
protected:
	float _radius;
	int _score;
};



#endif // !_Entity

