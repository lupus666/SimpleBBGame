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
	//���õ�ǰ��ʾ�˵����������
	float getIndex();
	//���ر�ѡ�е�item
	MenuItem * getCurrentItem();
	//�øýڵ���Ϊ������Ʒ�ĸ��ڵ� ���㼯�����λ��
    Node *view = nullptr;

	//��ʼ����������
	Vec2 touchBeginVec2;

	virtual bool onTouchBegan(cocos2d::Touch *touch);
	virtual void onTouchMoved(cocos2d::Touch *touch);
	virtual void onTouchEnded(cocos2d::Touch *touch);
	virtual void onTouchCancelled(cocos2d::Touch *touch);

	//�����ɫ������ ������������Ľ�ɫ�����ĵľ���
	float calcDistance();

};

#endif