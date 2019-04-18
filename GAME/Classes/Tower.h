#pragma once
#include "_Object.h"
#include "cocos2d.h"
#include "Bullet.h"
#include <vector>
#include "ui\CocosGUI.h"

using namespace std;
class Tower : public _Object
{
private:
	Sprite* sprite;
	Sprite* bg;
	Sprite* rankIcon;
	Sprite *waiting;
	Label* waitingLabel;
	int towerType;//1 : machine gun, 2 missile, 3 at cannon
	int goldBuild;
	int goldSell;
	float towerTimeBuild;
	float towerDmg;
	int towerRangeAttack;
	float towerSpeedAttack;
	int rank;
	Vec2 pos;
	int bulletNumb;
	int bulletType;
	vector<Bullet*> listBullet;
	float timeWait;

	Label *label1, *label2, *label3;
	Sprite *range, *labelBg1, *labelBg2, *labelBg3;
	Sprite *upgradeBg, *upgradeIcon, *upgradeBg1, *upgradeIcon1, *sellIcon;

public:
	Tower();
	Tower(int type, Vec2 pos, cocos2d::Layer *scene);
	void setPos(Vec2 );
	Vec2 getPos();
	void setTowerRangeAttack(int);
	int getTowerRangeAttack();
	void setTowerDmg(float);
	float getTowerDmg();
	void setTowerSpeedAttack(float);
	float getTowerSpeedAttack();
	void setTowerType(int);
	int getTowerType();
	int getGoldBuild();
	static int getGoldBuild(int type, int rank);
	void setGoldBuild(int);
	int getGoldSell();
	void setGoldSell(int);
	void updateTowerTimeBuild(float);
	float getTowerTimeBuild();
	void upRank(int );
	int getRank();
	vector<Bullet*> getBullet();
	void rotate();
	Rect getBox();
	void updateInfo();
	void setBulletType(int type);
	int getBulletType();
	void destroy();
	void createBullet(Layer* scene);

	void createOption(Layer*);
	void showOption();
	void hideOption();
	bool isRangeShow();
	void updateOption();
	int checkOptionClick(Vec2 pos);

	void shoot(float, float);
	void completeShoot(Bullet*);
	void updateTimeWait(float dt);
	bool checkTimeWait();
	void resetTimeWait();
};
