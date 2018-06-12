#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "MainScene.h"

class HelloWorld : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	virtual void onExit();

	void update(float dt);
	
	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
private:
	cocos2d::ProgressTimer * progresstime; //进度条
	cocos2d::Label * loadingLabel;  //加载文字
	void loadingTextureCallBack(cocos2d::Texture2D* texture);
};

#endif // __HELLOWORLD_SCENE_H__
