#include "LoginScene.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"

USING_NS_CC;

Scene* LoginScene::createScene()
{
	return LoginScene::create();
}

bool LoginScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	
	createBackground();
	createLoginButton();
	initplayerBox();

	return true;
}

void LoginScene::createBackground()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite* bg = Sprite::create("Scene/001.jpg");
	
	bg->setScale(1.4f);
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	addChild(bg);
}
void LoginScene::createLoginButton()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto loginButton = ui::Button::create("Scene/Button_Normal.png", "Scene/Button_Press.png");
	
	loginButton->setTitleText("Login");
	loginButton->setTitleColor(Color3B::BLACK);
	loginButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	
	
	loginButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		if (type != ui::Widget::TouchEventType::ENDED)return;
		auto playername = playernameInput->getString();
		if (playername.empty())
		{
			MessageBox("Playername can't be empty", "Alert");
		}
		else
		{
			playername.substr(0, 16);
			UserDefault::getInstance()->getStringForKey("playername", playername);
			/*
			
			
			*/
		}
	});
	addChild(loginButton);
}
void LoginScene::initplayerBox()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	playernameBG = Sprite::create("Scene/input_normal.png");
	
	playernameInput = ui::TextField::create("Input Playername Here", "Alert",30);
	playernameInput->setString(UserDefault::getInstance()->getStringForKey("playername", ""));
	playernameInput->setColor(Color3B::BLACK);
	playernameInput->setCursorChar('|');
	playernameInput->setCursorEnabled(true);
	playernameInput->setMaxLength(16);
	playernameInput->setMaxLengthEnabled(true);
	playernameInput->setPosition(playernameBG->getContentSize() / 2);
	playernameInput->addEventListener(CC_CALLBACK_2(LoginScene::textFieldEvent, this));

	playernameBG->addChild(playernameInput);
	playernameBG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	addChild(playernameBG);
}
void LoginScene::textFieldEvent(Ref* sender, ui::TextField::EventType event)
{
	switch (event)
	{
	case ui::TextField::EventType::ATTACH_WITH_IME:
		playernameBG->setTexture("Scene/input_active.png");
		break;
	case ui::TextField::EventType::DETACH_WITH_IME:
		playernameBG->setTexture("Scene/input_normol.png");
		break;
	}
}



