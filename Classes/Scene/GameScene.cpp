#include"GameScene.h"
#include"GameLayer.h"

USING_NS_CC;


Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("BEAN/bean.plist", "BEAN/bean.png");

	auto backbutton = MenuItemImage::create("public/button-back.png",
		"public/button-back.png",
		CC_CALLBACK_1(GameScene::menuReturnCallback, this));
	backbutton->setPosition(Vec2(visibleSize.width - backbutton->getContentSize().width / 2, visibleSize.height - backbutton->getContentSize().height / 2));
	auto mn = Menu::create(backbutton, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn, 10);


	auto gamelayer = GameLayer::create();
	this->addChild(gamelayer, -1);

	return true;
}

void GameScene::menuReturnCallback(cocos2d::Ref* pSender)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, Main::createScene()));
}