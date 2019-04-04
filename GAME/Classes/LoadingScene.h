#ifndef _LOADING_SCENE_
#define _LOADING_SCENE_
#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include <vector>
#include <iostream>
USING_NS_CC;
using namespace std;
class LoadingScene : public cocos2d::Scene
{
public:
	Scene * nextScene;
	bool nextFlag = false;
	float nextTip;
	ui::LoadingBar *loadingbar;
	Sprite * tank;
	Sprite *plane;
	Sprite * TipSprite;
	Sprite *loadingBarGB;
	Label *labelPercent;
	Label *labelNext;
	vector<string> nameOfTips;
	static cocos2d::Scene* createScene();
	Sprite *createSprite();
	int indexTip;
	virtual bool init();
	virtual void update(float dt);
	void updateLoadingBar();
	void leftTips();
	void rightTips();
	void changeTip(int n);
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event  *event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event  *event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event  *event);

	// implement the "static create()" method manually
	CREATE_FUNC(LoadingScene);
};
#endif //_LOADING_SCENE_
