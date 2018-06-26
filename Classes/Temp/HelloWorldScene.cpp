#include "Temp\HelloWorldScene.h"

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

	Sprite *title = Sprite::create("HelloWorld/title.png");

	// position the label on the center of the screen
	title->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	
    auto fadeIn = FadeIn::create(0.5);
	auto fadeOut = FadeOut::create(1.0);
	auto seq = Sequence::create(fadeIn, fadeOut);
	title->runAction(seq);
	this->addChild(title);
	//开始精灵

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

	Director::getInstance()->getTextureCache()->addImageAsync("texture/bean.png",
		CC_CALLBACK_1(HelloWorld::loadingTextureCallBack, this));

	this->scheduleUpdate();

	/*auto sprite = Sprite::createWithSpriteFrameName("loading/loading4.png");
	this->addChild(sprite);
	sprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	Animation* animation = Animation::create();
	for (int i = 1; i <= 4; i++)
	{
		__String *frameName = __String::createWithFormat("loading%d.png", i);
		log("frameName = %s", frameName->getCString());
		SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
		animation->addSpriteFrame(spriteFrame);
	}

	animation->setDelayPerUnit(0.5f);           //设置两个帧播放时间
	animation->setRestoreOriginalFrame(true);    //动画执行后还原初始状态

	Animate* action = Animate::create(animation);
	sprite->runAction(RepeatForever::create(action));*/

	
	return true;
}

void HelloWorld::loadingTextureCallBack(Texture2D *texture)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/bean.plist", texture);
}

void HelloWorld::update(float dt)
{
	float ct = progresstime->getPercentage();
	ct += 0.5;
	progresstime->setPercentage(ct);
	if (ct >= 100)
	{
		TransitionFade *tScene = CCTransitionFade::create(2, Main::createScene(), Color3B::WHITE);
		Director::sharedDirector()->replaceScene(tScene);
	}
}


void HelloWorld::onExit()
{
	this->unscheduleAllCallbacks();
	Layer::onExit();
}