#ifndef _GameLayer_H_
#define _GameLayer_H_

#include"cocos2d.h"
#include"Macro.h"
#include"ui\CocosGUI.h"
#include"Classes\Entity\Bean.h"
#include"Classes\Entity\Player.h"
#include"Classes\Entity\PlayerDivision.h"
#include"Classes\Entity\Spore.h"
#include"Classes\Entity\Thorn.h"

USING_NS_CC;
using namespace ui;

class Bean;
class Player;
class Spore;
class Thorn;

class GameLayer:public Layer
{
public:
	GameLayer() {};
	~GameLayer();

	virtual bool init();

	CREATE_FUNC(GameLayer);

	virtual void update(float t);

	void initData();
	void initBean();
	void initThorn();
	void addThorn(float);
	void initPlayer();

	void updateBean();
	void resetBean(Node* node);
	void updateThorn();
	void updateSpore();
	void updatePlayer();
	void updateRival();
	void updateView();

	void collidePlayer();
	void collideBean(Player*);

	void spitSpore();
	void divide();

private:
	Node * _map;
	Player* _player;
	Vector<Bean*> _beanlist;
	Map<std::string, Player*> _rivalMap;
	Map<int,Thorn*> _thornMap;
	Map<int, Spore*> _sporeMap;
	float _mapscale;


};


#endif // !_GameLayer_H_

