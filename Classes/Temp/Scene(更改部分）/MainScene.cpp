#include "MainScene.h"

Scene * Main::createScene()
{
	auto scene = Scene::create();

	auto layer = Main::create();

	scene->addChild(layer);

	return scene;
}

bool Main::init()
{
	if (!Layer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("scene/menuScene/menu_background.png");
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	bg->setScale(1.3f);
	this->addChild(bg);

	auto title = Sprite::create("scene/menuScene/title.png");
	title->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	title->setScale(1.5f);
	this->addChild(title);

	//¿ªÊ¼Í¼Æ¬²Ëµ¥
	MenuItemSprite *startMenuItem = MenuItemImage::create("scene/loading/start-up.png",
		"scene/loading/start-down.png",
		CC_CALLBACK_1(Main::menuItemCallback, this));
	startMenuItem->setTag(ActionType::MenuItemStart);
	startMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 * 3)));

	// ÉèÖÃÍ¼Æ¬²Ëµ¥
	MenuItemImage *settingMenuItem = MenuItemImage::create(
		"scene/loading/setting-up.png",
		"scene/loading/setting-down.png",
		CC_CALLBACK_1(Main::menuItemCallback, this));
	settingMenuItem->setTag(ActionType::MenuItemSetting);
	settingMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(origin.x + visibleSize.width / 2 - 120, origin.y + visibleSize.height / 4 * 3)));

	//Æ¤·ôÍ¼Æ¬²Ëµ¥
	MenuItemImage *skinMenuItem = MenuItemImage::create(
		"scene/loading/skin-up.png",
		"scene/loading/skin-down.png",
		CC_CALLBACK_1(Main::menuItemCallback, this));
	skinMenuItem->setTag(ActionType::MenuItemSkin);
	skinMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(origin.x + visibleSize.width / 2 + 120, origin.y + visibleSize.height / 4 * 3)));

	
	Menu* mu = Menu::create(startMenuItem, settingMenuItem, skinMenuItem, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu);

	return true;
}

void Main::menuItemCallback(cocos2d::Ref *sender)
{

	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}
	Scene* tsc = nullptr;

	MenuItem* menuItem = (MenuItem*)sender;
	switch (menuItem->getTag())
	{
	case ActionType::MenuItemStart:
		tsc = TransitionFade::create(1.0f, GameScene::createScene());
		break;
	case ActionType::MenuItemSetting:
		tsc = TransitionFade::create(1.0f, Setting::createScene());
		break;
	case ActionType::MenuItemSkin:
		tsc = TransitionFade::create(1.0f, Skin::createScene());
	}
	if (tsc)
		Director::getInstance()->replaceScene(tsc);

}


void Main::onEnterTransitionDidFinish()      
{
	Layer::onEnterTransitionDidFinish();
	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(MUSIC_KEY)) {
		if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
			log("background music is playing");
		}
		else {
			log("background music is not playing");
			SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_1, true);
		}
	}
}

void Main::onExit()
{
	this->unscheduleAllCallbacks();
	Layer::onExit();
}