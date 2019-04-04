#ifndef _MAIN_MENU_SCENE_H_
#define _MAIN_MENU_SCENE_H_

#include "cocos2d.h"
using namespace std;

class MainMenuScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void createMainMenuTiles();
	void createUI();
	void Back();
	void Starts();
	void Settings();
	void Abouts();
	void Quits();
	void Upgrade();
	void funcQuits();
	void ChangeName();
	void ChangedSlider();
	void Mute();
	void update(float) override;
	int stringToIntMainMenu(string s);
	CREATE_FUNC(MainMenuScene);
};
#endif