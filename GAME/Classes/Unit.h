#ifndef _UNIT_H_
#define _UNIT_H_
#include "_Object.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Bullet.h"
#include <vector>
class Unit : public _Object
{
private:
	char* unitName;
	Sprite* bg;
	int locationPoint;
	bool freeFlag;
	bool visible;
	int hp, baseHp;
	float dmg;
	int gold;
	bool status;
	int bulletType;
	int bulletNumb;
	float attackSpeed;
	int range;
	ui::LoadingBar *hpBar;
	Sprite *hpBarBg;
	vector<Bullet *> listBullet;
	float timeWait;
public:
	Unit();
	Unit(cocos2d::Layer *scene, char *name);
	~Unit();

	void createBullet(Layer* scene);

	void setPos(Vec2 pos);
	Vec2 getPos();

	void setVisible(bool v);
	bool getVisible();

	void setName(char *s);
	char *getName();

	void setAttackSpeed(float as);
	float getAttackSpeed();

	void setRange(int range);
	int getRange();

	vector<Bullet *> getBullet();

	void setBulletType(int type);
	int getBulletType();

	void setBulletNumb(int n);
	int getBulletNumb();

	void setHp(int hp);
	int getHp();

	void setDmg(float dmg);
	float getDmg();

	void setGold(int gold);
	int getGold();

	void setStatus(bool stt);
	bool getStatus();

	void setLocationPoint(int lp);
	int getLocationPoint();

	void setFreeFlag(bool flag);
	bool getFreeFlag();

	void AddHpBar(Layer *layer);
	void updatePosHB();

	void setLDBVisible(bool visible);

	void shoot(float, float);
	void completeShoot(Bullet *bullet);

	void updateInfo();

	void runAct(Sequence *seq);
	Sprite* getBg();

	void die();
	void destroy();

	void stop();

	void updateTimeWait(float dt);
	bool checkTimeWait();
	void resetTimeWait();

};
#endif //_UNIT_H_
