#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("BEAN/bean.plist", "BEAN/bean.png");

	auto bg = Sprite::create("HelloWorld/background.png");
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg);

	auto loadingBg = Sprite::create("HelloWorld/loading0.png");
	loadingBg->setPosition(visibleSize.width / 2, 40);
	this->addChild(loadingBg);

/*	loadingLabel = Label::createWithTTF("loading:0%%", "fonts/Quicksand-Light.ttf", 20);
	loadingLabel->setColor(Color3B(0, 0, 0));
	loadingLabel->setPosition(visibleSize.width / 2, 55);
	this->addChild(loadingLabel);
*/
	auto loadingSprite = Sprite::create("HelloWorld/loading1.png");
	progresstime = ProgressTimer::create(loadingSprite);
	progresstime->setType(ProgressTimerType::BAR);
	progresstime->setBarChangeRate(Vec2(1, 0));
	progresstime->setPercentage(0);
	progresstime->setPosition(visibleSize.width / 2, 40);
	this->addChild(progresstime);

	/*Director::getInstance()->getTextureCache()->addImageAsync("texture/bean.png",
		CC_CALLBACK_1(HelloWorld::loadingTextureCallBack, this));
		*/
	this->scheduleUpdate();

	return true;
}

void HelloWorld::update(float dt)
{
	float ct = progresstime->getPercentage();
	ct += 0.8;
	progresstime->setPercentage(ct);
	if (ct >= 100)
	{
		TransitionFade *tScene = CCTransitionFade::create(2, Register::createScene(), Color3B::WHITE);
		Director::sharedDirector()->replaceScene(tScene);
	}
}


void HelloWorld::onExit()
{
	this->unscheduleAllCallbacks();
	Layer::onExit();
}