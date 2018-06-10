#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class Bean;
class Spore;
class Thorn;
class PlayerDivision;

class Player :public Node
{
public:

	enum class State
	{
		NORMAL,
		DIVIDE,
		SPIT,
		COMBINE
	};


	Player() 
	{ 
		_divisionNum = 0;
	}
	~Player()
	{
		_divisionlist.clear();
	}

	static Player* create(const std::string&, Vec2, Node*, int);      //单机版
	bool init(const std::string&, Vec2, Node*, int);

	PlayerDivision* createDivision(Vec2, Vec2, int);
	void dividePlayer();

	bool collideBean(Bean*);
	bool collideSpore(Spore*);
	bool collideThorn(Thorn*);
	bool collidePlayer(Player*);

	void updateDivision();
	void resetPlayer();//复活

	void spitSpore(Node*, Map<int, Spore*>&, int);
	int countSpitSpore();

	void setVector(Vec2);
	Vec2 getVector();

	Rect getPlayerRect();//用以计算中心

	std::string getName();
	int getDivisionNum();
	int getScore();
	Vector<PlayerDivision*>& getDivisionlist();

	bool divideFinish() {
		_state = State::NORMAL;
	}

	void setcombine(float t) { _combineEnable = true; }


private:
	Vector<PlayerDivision*> _divisionlist;
	int _divisionNum;

	Node* _map;
	std::string _name;

	Vec2 _vector;
	State _state;

	int _skinID;

	bool _combineEnable;

};
#endif // !_PLAYER_H_

