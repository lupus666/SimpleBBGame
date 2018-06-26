#ifndef _GameScene_H_
#define _GameScene_H_

#include"cocos2d.h"
#include"ui\CocosGUI.h"

#include"SystemHeader.h"
#include"MainScene.h"

class GameScene :public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	
	CREATE_FUNC(GameScene);


	void menuReturnCallback(cocos2d::Ref* pSender);
};
#endif // !_GameScene_H_

