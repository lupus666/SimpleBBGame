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
	this->addChild(bg);

	//开始图片菜单
	MenuItemSprite *startMenuItem = MenuItemImage::create("scene/loading/start-up.png",
		"scene/loading/start-down.png",
		CC_CALLBACK_1(Main::menuItemCallback, this));
	startMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(origin.x + visibleSize.width / 4 * 1, origin.y + visibleSize.height / 4 * 3)));

	// 设置图片菜单
	MenuItemImage *settingMenuItem = MenuItemImage::create(
		"scene/loading/setting-up.png",
		"scene/loading/setting-down.png",
		CC_CALLBACK_1(Main::menuItemCallback, this));
	settingMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(origin.x + visibleSize.width / 4 * 2, origin.y + visibleSize.height / 4 * 3)));

	
	Menu* mu = Menu::create(startMenuItem, settingMenuItem, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu);

	
	MenuItemImage *



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
		tsc = TransitionFade::create(1.0f, Game::createScene());
		break;
	case ActionType::MenuItemSetting:
		tsc = TransitionFade::create(1.0f, Setting::createScene());
		break;
	}
	if (tsc)
		Director::getInstance()->pushScene(tsc);

}


void Main::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(MUSIC_KEY)) {
		SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_1, true);
	}
}
