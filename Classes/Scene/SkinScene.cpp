#include "SkinScene.h"

USING_NS_CC;

Scene* Skin::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Skin::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Skin::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *bg = Sprite::create("scene/skinScene/setting-background.png");

	// position the label on the center of the screen
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, -1);

	view = Node::create();
	view->setPosition(visibleSize / 2);
	this->addChild(view);

	//创建所有皮肤
	
	for (int _skinID = 1; _skinID != 8; _skinID++)
	{
		std::string str = StringUtils::format("SKIN/skin_%d.png", _skinID);
		Sprite *sp = Sprite::create(str);
		sp->setPosition(Vec2((_skinID - 1) * sp->getContentSize().width * 1.2, 0));
		sp->setScale(1.2f);
		view->addChild(sp);
		skinArray.pushBack(sp);
	}
		
	MenuItemSprite *OKMenuItem = MenuItemImage::create("scene/skinScene/ok-up.png",
		"scene/skinScene/ok-down.png",
		CC_CALLBACK_1(Skin::menuSelectCallback, this));
	OKMenuItem->setScale(1.0f);
	OKMenuItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
	Menu *menu = Menu::create(OKMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	MenuItemImage *returnMenuItem = MenuItemImage::create("public/button-back.png", 
		"public/button-back.png", 
		CC_CALLBACK_1(Skin::menuReturnCallback, this));
	returnMenuItem->setPosition(Vec2(visibleSize.width - returnMenuItem->getContentSize().width / 2, 
		visibleSize.height - returnMenuItem->getContentSize().height / 2));
	auto mn = Menu::create(returnMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn, 1);

	auto listener = EventListenerTouchOneByOne::create();//创建一个触摸监听(单点触摸）  
	listener->onTouchBegan = CC_CALLBACK_1(Skin::onTouchBegan, this);//指定触摸的回调函数  
	listener->onTouchMoved = CC_CALLBACK_1(Skin::onTouchMoved, this);//指定触摸的回调函数  
	listener->onTouchEnded = CC_CALLBACK_1(Skin::onTouchEnded, this);//指定触摸的回调函数  
	listener->onTouchCancelled = CC_CALLBACK_1(Skin::onTouchCancelled, this);//指定触摸的回调函数  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool Skin::onTouchBegan(Touch *touch)
{
	touchBeginVec2 = touch->getLocation();
	return true;
}

void Skin::onTouchMoved(Touch *touch)
{
	int moveX = touch->getLocation().x - touchBeginVec2.x;
	auto moveBy = MoveBy::create(0.0f, Vec2(moveX, 0));
	view->runAction(moveBy);

	touchBeginVec2 = touch->getLocation();
}

void Skin::onTouchEnded(Touch *touch)
{
	view->stopAllActions();

	auto moveBy = MoveBy::create(0.2f, Vec2(calcDistance(), 0));
	view->runAction(moveBy);
}

void Skin::onTouchCancelled(Touch *touch)
{
	onTouchEnded(touch);
}

float Skin::calcDistance()
{
	float result = 100000000000.0f;
	Size vSize = Director::getInstance()->getVisibleSize();
    int index = 1;
	for (auto skin : skinArray)
	{
		Point p2world = skin->convertToWorldSpace(Point(0, 0)) + Vec2(skin->getContentSize().width / 2, 0);
		
		float dis = vSize.width / 2 - p2world.x;
		float temp = fabs(dis);
		
		if (temp < result)
		{
			result = dis;
			_skinID = index; //更新皮肤索引
		}
		index++;
	}

	return result;
}

void Skin::menuReturnCallback(cocos2d::Ref* pSender)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}
	auto sc = Main::createScene();
	auto reScene = TransitionFade::create(0.5f, sc);
	Director::getInstance()->replaceScene(reScene);
}

void Skin::menuSelectCallback(cocos2d::Ref* pSender)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}
	UserDefault::getInstance()->setIntegerForKey(SKIN_KEY, _skinID);
}