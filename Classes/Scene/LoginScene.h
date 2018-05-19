#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"

class LoginScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(LoginScene);
private:

	cocos2d::ui::TextField* playernameInput;

	cocos2d::Sprite* playernameBG;

	void createBackground();
	void createLoginButton();

	void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType event);

};

#endif //__LOGINSCENE_SCENE_H__