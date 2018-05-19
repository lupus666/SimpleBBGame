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

	return true;
}

void LoginScene::createBackground()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite* bg = Sprite::create("Scene/001.jpg");
	
	bg->setScale(1.4f);
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
}
void LoginScene::createLoginButton()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto loginButton = ui::Button::create();
	
	loginButton->setTitleText("Login");
	loginButton->setPosition();
}



