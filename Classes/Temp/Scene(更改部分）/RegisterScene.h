#ifndef __REGISTER_SCENE_H_
#define __REGISTER_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "SystemHeader.h"
#include "MainScene.h"

USING_NS_CC_EXT;

class Register :public cocos2d::Layer
{
private:
	EditBox * editBoxUsername;//用户名

public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	
	void menuLoginCallback(cocos2d::Ref *pSender);//跳转到游戏场景
	CREATE_FUNC(Register);

};
#endif 