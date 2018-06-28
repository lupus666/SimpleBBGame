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
#include"Network\client.h"

USING_NS_CC;
using namespace ui;

class Bean;
class PlayerL;
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


	/*µ¥»ú°æ*/
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
	void collideBean(PlayerL*);

	void spitSpore();
	void divide();
	/*µ¥»ú°æ*/
	
	/*ÍøÂç°æ*/

	void initDataOnline();
	void initBean(int);
	void initRival(player::OtherPlayerInfo& allplayer);

	void updateRival(player::OtherPlayerInfo& allplayer);


	/*ÍøÂç°æ*/
	bool onTouchBegan(Touch* touch,Event *event);
	void onTouchMove(Touch* touch,Event *event);
	void onTouchEnded(Touch* touch,Event *event);

	void onKeyPressed(EventKeyboard::KeyCode keycode, Event * event);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

private:
	Node * _map;
	PlayerL* _player;
	Vector<Bean*> _beanlist;
	Map<std::string, PlayerL*> _rivalMap;
	Map<std::string, SingePlayerInfo*> _rivalTag;
	Map<int,Thorn*> _thornMap;
	Map<int, Spore*> _sporeMap;
	float _mapscale;
	int sporeID;
	SingePlayerInfo playerinfo;
};


#endif // !_GameLayer_H_

