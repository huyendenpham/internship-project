#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__

#include "cocos2d.h"
#include "Champion.h"
#include "Tower.h"
#include "UI.h"
#include "Unit.h"

class GamePlayScene : public cocos2d::Layer
{
private :
	Champion *myHero;
	vector<ValueMap> locationPoint;
	vector<ValueMap> locationPointSky;
	vector<ValueMap> towerLocation;
	vector<Tower*> towerList;
	TMXTiledMap *tileMap = NULL;
	Layer *MapLayer, *MissionSuccessLayer, *MissionFailLayer;
	TMXLayer *mPhysicsLayer;
	UI *UILayer;
	int gold, lives;	
	vector <Unit*> tempGround, tempSky;



public:
	static cocos2d::Scene* createScene(int lv);

	virtual bool init();
	///////////////////////////
	// create map and process object group
	void createTiledMap();
	void loadListPoint();
	//////////////////////////
	// create UI
	void updateUI();
	void createMissionSuccess();
	void createMissionFail();
	//////////////////////////
	// a selector callback
	void menuCloseCallback();
	//////////////////////////////////////////
	//tower builder
	void buildTower(int type, Vec2 pos);
	bool checkTower(Vec2 pos);
	/////////////////////////////////////////
	//touch event
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event  *event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event  *event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event  *event);
	/////////////////////////////////////////
	// math event
	float getCorner(float xa, float ya, float xb, float yb);
	float getCorner2(float xa, float ya, float xb, float yb);
	float getDistance(float xa, float ya, float xb, float yb);
	///////////////////////////////////////
	// override update funtion
	void update(float) override;
	//////////////////////////////////////
	// update funtion
	void heroController();
	void heroDetectEnemy(float);
	void towerDetectEnemy(float);
	void updateUnit(float);
	void updateBullet(float);
	void actionUnitFinished(Unit*);
	void stopAllMove();
	
	// implement the "static create()" method manually
	CREATE_FUNC(GamePlayScene);
};

#endif // __GAMEPLAY_SCENE_H__

