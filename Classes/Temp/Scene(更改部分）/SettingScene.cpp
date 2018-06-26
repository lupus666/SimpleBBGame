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
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, -1);
	
    // 返回图片菜单
	MenuItemImage *okMenuItem = MenuItemImage::create("public/button-back.png", "public/button-back.png", CC_CALLBACK_1(Setting::menuOkCallback, this));
	okMenuItem->setPosition(Vec2(visibleSize.width  - okMenuItem->getContentSize().width / 2, visibleSize.height - okMenuItem->getContentSize().height / 2));
	auto mn = Menu::create(okMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn, 1);
	
	//音效
	auto soundOnMenuItem = MenuItemImage::create(
		"scene/settingScene/setting-on.png",
		"scene/settingScene/setting-on.png");
	auto soundOffMenuItem = MenuItemImage::create(
		"scene/settingScene/setting-off.png",
		"scene/settingScene/setting-off.png");
	auto soundToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::menuSoundToggleCallback, this),
		soundOnMenuItem,
		soundOffMenuItem,
		NULL);

	//音乐
	auto musicOnMenuItem = MenuItemImage::create(
		"scene/settingScene/setting-on.png",
		"scene/settingScene/setting-on.png");
	auto musicOffMenuItem = MenuItemImage::create(
		"scene/settingScene/setting-off.png",
		"scene/settingScene/setting-off.png");
	auto musicToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::menuMusicToggleCallback, this),
		musicOnMenuItem,
		musicOffMenuItem,
		NULL);
	
	auto  menu = Menu::create(soundToggleMenuItem, musicToggleMenuItem, NULL);
	menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(70, 0));
	menu->alignItemsVerticallyWithPadding(30.0f);
	addChild(menu, 1);
	
	//effectsound设置文字
	auto soundLabel = Label::createWithTTF("Sound Effect", "fonts/arial.ttf", 30);
	soundLabel->setPosition(menu->getPosition() - Vec2(150, -38));
	soundLabel->setColor(Color3B(28, 155, 28));
	this->addChild(soundLabel, 1);
	
	//bgm设置文字
	auto bgmLabel = Label::createWithTTF("BackGround Music","fonts/arial.ttf",30);
	bgmLabel->setPosition(soundLabel->getPosition() - Vec2(0, 85));
	bgmLabel->setColor(Color3B(28,155,28));
	this->addChild(bgmLabel, 1);

	

	//设置音效和音乐选中状态
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
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(sound_1);
	auto sc = Main::createScene();
	auto reScene = TransitionFade::create(0.5f, sc);
	Director::getInstance()->replaceScene(reScene);
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
		SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_1, true);
	}
}
