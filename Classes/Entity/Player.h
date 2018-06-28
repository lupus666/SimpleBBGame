#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class Bean;
class Spore;
class Thorn;
class PlayerDivision;

class PlayerL :public Node
{
public:

	enum class State
	{
		NORMAL,
		DIVIDE,
		SPIT,
		STATIC
	};


	PlayerL() 
	{ 
		_divisionNum = 0;
	}
	~PlayerL()
	{
		_divisionlist.clear();
	}

	static PlayerL* create(const std::string&, Vec2, Node*, int);      //单机版
	bool init(const std::string&, Vec2, Node*, int);

	static PlayerL* create(const std::string&, Vec2, Vec2, Node*, int, int, int);
	bool init(const std::string&, Vec2, Vec2, Node*, int, int, int);

	PlayerDivision* createDivision(Vec2, Vec2, int);
	void dividePlayer();

	bool collideBean(Bean*);
	bool collideSpore(Spore*);
	bool collideThorn(Thorn*);
	bool collidePlayer(PlayerL*);

	void updateDivision();
	void resetPlayer();//复活

	void spitSpore(Map<int, Spore*>&, int&);
	int countSpitSpore();

	void setVector(Vec2);
	Vec2 getVector() { return _vector; }

	Rect getPlayerRect();//用以计算中心

	std::string getName() { return _name; }
	int getDivisionNum() { return _divisionNum; }
	int getAllScore();
	Vector<PlayerDivision*>& getDivisionlist() { return _divisionlist; }

	void divideFinish() {
		_state = State::NORMAL;
	}
	void thornFinish() {
		_state = State::STATIC;
	}


	void setcombine(float t) { _combineEnable = true; }

	int getSkin() { return _skinID; }
	void setSkin(int id) { _skinID = id; }

	void setdivideTimes(int d) { _divideTimes = d; }
	void setspitTimes(int s) { _spitTimes = s; }

	int getdivideTimes() { return _divideTimes; }
	int getspitTimes() { return _spitTimes; }

private:
	Vector<PlayerDivision*> _divisionlist;
	int _divisionNum;

	Node* _map;
	std::string _name;

	Vec2 _vector;
	State _state;

	int _skinID;

	bool _combineEnable;

	int _divideTimes;
	int _spitTimes;

};
#endif // !_PLAYER_H_

