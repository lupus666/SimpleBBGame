#include "RegisterScene.h"

Scene* Register::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Register::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Register::init()
{
	if (!Layer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	auto bg = Sprite::create("scene/registerScene/background.png");
	bg->setScale(1.3f);
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	MenuItemSprite *loginMenuItem = MenuItemImage::create("scene/registerScene/login-up.png",
		"scene/registerScene/login-down.png",
		CC_CALLBACK_1(Register::menuLoginCallback, this));
	loginMenuItem->setScale(0.7f);
	loginMenuItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 40));
	Menu *menu = Menu::create(loginMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	
	Scale9Sprite *user_bg = Scale9Sprite::create("scene/registerScene/login-box.png");
	editBoxUsername = EditBox::create(Size(300, 40), user_bg);
	editBoxUsername->setFontColor(Color3B(0, 0, 0));
	editBoxUsername->setFontSize(38);
	editBoxUsername->setPlaceHolder("Name:"); //输入框缺省文字
	editBoxUsername->setPlaceholderFontColor(Color3B::WHITE); //缺省文字颜色
	editBoxUsername->setMaxLength(20);
	editBoxUsername->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);//可以输入任何，但是不包括换行   
	editBoxUsername->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE); 
	editBoxUsername->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2 + 60));
	this->addChild(editBoxUsername);

	return true;
}

void Register::menuLoginCallback(cocos2d::Ref *pSender)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}
	auto tsc = TransitionFade::create(1.0f, Main::createScene());
	if (tsc)
		Director::getInstance()->replaceScene(tsc);
}
