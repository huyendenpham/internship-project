#ifndef __UPGRADE_SCENE_H__
#define __UPGRADE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class UpgradeScene : public cocos2d::Scene
{
public:
	Layer *GunLayer, *MissileLayer, *CannonLayer, *TowerLayer, *HeroLayer;
	

	ui::Button *rankMi1;
	ui::Button *rankMi2;
	ui::Button *rankMi3;
	ui::Button *rankMi4;
	ui::Button *rankMi5;

	ui::Button *rankGun1;
	ui::Button *rankGun2;
	ui::Button *rankGun3;
	ui::Button *rankGun4;
	ui::Button *rankGun5;

	ui::Button *rankCannon1;
	ui::Button *rankCannon2;
	ui::Button *rankCannon3;
	ui::Button *rankCannon4;
	ui::Button *rankCannon5;


	static cocos2d::Scene* createScene();
	virtual bool init();

	void createTowerLayer();
	void createHeroLayer();
	void createGun();
	void createMissile();
	void createCannon();

	void updateInfo(int type, int rank);
	void update(int type);

	void buyTech(int type, int rank, int price);

	void updateBp();

	CREATE_FUNC(UpgradeScene);
};
#endif //