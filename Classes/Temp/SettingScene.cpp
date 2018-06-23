#include "SettingScene.h"

USING_NS_CC;

Scene* Setting::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Setting::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Setting::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *bg = Sprite::create("scene/settingScene/setting-background.png");

	// position the label on the center of the screen
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(bg);
	

	
	//ÒôÐ§
	auto soundOnMenuItem = MenuItemImage::create(
		"settingScene/setting-on.png",
		"settingScene/setting-on.png");
	auto soundOffMenuItem = MenuItemImage::create(
		"settingScene/setting-off.png",
		"settingScene/setting-off.png");
	auto soundToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::menuSoundToggleCallback, this),
		soundOnMenuItem,
		soundOffMenuItem,
		NULL);

	//ÒôÀÖ
	auto musicOnMenuItem = MenuItemImage::create(
		"settingScene/setting-on.png",
		"settingScene/setting-on.png");
	auto musicOffMenuItem = MenuItemImage::create(
		"settingScene/setting-off.png",
		"settingScene/setting-off.png");
	auto musicToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::menuMusicToggleCallback, this),
		musicOnMenuItem,
		musicOffMenuItem,
		NULL);
	
	auto  menu = Menu::create(soundToggleMenuItem, musicToggleMenuItem, NULL);
	menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(70, 50));
	menu->alignItemsVerticallyWithPadding(20.0f);
	addChild(menu, 1);
	
	//effectsoundÉèÖÃÎÄ×Ö
	auto soundLabel = Label::createWithTTF("ÒôÐ§", "fonts/arial.ttf", 50);
	soundLabel->setPosition(menu->getPosition() - Vec2(100, -30));
	soundLabel->setColor(Color3B(28, 155, 28));
	this->addChild(soundLabel, 1);
	return true;
	//bgmÉèÖÃÎÄ×Ö
	auto bgmLabel = Label::createWithTTF("±³¾°Òô","fonts/arial.ttf",50);
	bgmLabel->setPosition(soundLabel->getPosition() - Vec2(0, 60));
	bgmLabel->setColor(Color3B(28,155,28));

	// ·µ»ØÍ¼Æ¬²Ëµ¥
	MenuItemImage *okMenuItem = MenuItemImage::create("public/return-up.png", "public/return-down.png", CC_CALLBACK_1(Setting::menuOkCallback, this));
	okMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2)));


	//ÉèÖÃÒôÐ§ºÍÒôÀÖÑ¡ÖÐ×´Ì¬
	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(MUSIC_KEY)) 
	{
		musicToggleMenuItem->setSelectedIndex(0);
	}
	else 
	{
		musicToggleMenuItem->setSelectedIndex(1);
	}
	if (defaults->getBoolForKey(SOUND_KEY)) 
	{
		soundToggleMenuItem->setSelectedIndex(0);
	}
	else 
	{
		soundToggleMenuItem->setSelectedIndex(1);
	}

	return true;
}


void Setting::menuOkCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void Setting::menuSoundToggleCallback(Ref* pSender)
{
	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(SOUND_KEY)) {
		defaults->setBoolForKey(SOUND_KEY, false);
	}
	else {
		defaults->setBoolForKey(SOUND_KEY, true);
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}
}


void Setting::menuMusicToggleCallback(Ref* pSender)
{
	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(MUSIC_KEY)) {
		defaults->setBoolForKey(MUSIC_KEY, false);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
	else {
		defaults->setBoolForKey(MUSIC_KEY, true);
		SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_2, true);
	}
}
}