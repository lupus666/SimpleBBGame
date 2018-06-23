#ifndef _PlayerDivision_H_
#define _PlayerDivision_H_

#include "Entity.h"

class Bean;
class Thorn;
class Spore;


class PlayerDivision : public Entity
{
public:
	PlayerDivision() {};
	~PlayerDivision() {};

	static PlayerDivision* create(const std::string&, int, int);
	bool init(const std::string&, int, int);



	bool collideBean(Bean* bean);
	bool collideSpore(Spore* spore);
	bool collideThorn(Thorn* thorn);

	void setvector(Vec2);
	Vec2 getvector();

	void setspeed(float);
	float getspeed();

	void setPlayerName(const std::string);
	
	void setPreposition(const Vec2&);
	Vec2 getPreposition();

	void scorehalve();
	void addScore(int);
	void spitspore();

	void change();

	
private:
	Label * _nameLabel;
	std::string _name;

	float _speed;
	Vec2 _vector;
	Vec2 _preposition;

	int _skinID;

};
#endif // !_PlayerDivision_H_

