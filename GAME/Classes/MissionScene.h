#ifndef __MISSION_SCENE_H__
#define __MISSION_SCENE_H__

#include "cocos2d.h"

class MissionScene : public cocos2d::Scene
{

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void createUIMission();
	CREATE_FUNC(MissionScene);
};

#endif 
