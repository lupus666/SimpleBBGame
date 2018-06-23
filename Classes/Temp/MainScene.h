#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "SettingScene.h"
#include "GameScene.h"
#include "SystemHeader.h"

USING_NS_CC;

typedef  enum {
	MenuItemStart,
	MenuItemSetting,
	MenuItemHelp
} ActionType;

class Main :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	void menuItemCallback(cocos2d::Ref *sender);
	CREATE_FUNC(Main);
private:
	MenuItemImage * gameItem;
	MenuItemImage * chatItem;
	Layer * mainLayer;
};





#endif
