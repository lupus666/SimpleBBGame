#ifndef __SKIN_SCENE_H__
#define __SKIN_SCENE_H__
#include "cocos2d.h"
#include "MainScene.h"
#include "SystemHeader.h"

USING_NS_CC;

/*typedef  enum {
	MenuItemSelectedL,
	MenuItemSelectedR
} ActionType;
*/
class Skin : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	// a selector call back
	void menuReturnCallback(cocos2d::Ref* pSender);
	void menuSelectCallback(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(Skin);
private:
	Vector<Sprite*> skinArray;
	int _skinID;
	void setIndex(int index);
	//设置当前显示菜单项的索引号
	float getIndex();
	//返回被选中的item
	MenuItem * getCurrentItem();
	//用该节点作为所有商品的父节点 方便集体调整位置
    Node *view = nullptr;

	//开始触摸的坐标
	Vec2 touchBeginVec2;

	virtual bool onTouchBegan(cocos2d::Touch *touch);
	virtual void onTouchMoved(cocos2d::Touch *touch);
	virtual void onTouchEnded(cocos2d::Touch *touch);
	virtual void onTouchCancelled(cocos2d::Touch *touch);

	//计算角色集合中 距离中心最近的角色与中心的距离
	float calcDistance();

};

#endif