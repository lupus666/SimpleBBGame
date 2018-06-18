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

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("BEAN/bean.plist","BEAN/bean.png");




	auto gamelayer = GameLayer::create();
	this->addChild(gamelayer, 1);
	return true;
}