#ifndef _CHAMPION_H_
#define _CHAMPION_H_
#include "cocos2d.h"
#include "_Object.h"
#include <vector>
#include "Bullet.h"
#include "Weapon.h"
using namespace std;

class Champion : public _Object
{
private:
	int hp;
	float moveSpeed;
	int weaponType;
	int armorLv;
	bool freeFlag;
	Weapon *weapon;
	vector<Bullet *> listBullet;
	float timeWait;
	bool status;
	int baseHp;
public:
	vector<Bullet *> getBulletList();
	Champion();
	Champion(cocos2d::Layer *layer);
	void setStatus(bool);
	bool getStatus();
	void setHp(int hp);
	int getHp();
	void setMoveSpeed(float moveSpeed);
	float getMoveSpeed();
	void setWeaponType(int weaponType);
	int getWeaponType();
	void setArmorLv(int lv);
	int getArmorLv();
	void setVisible(bool visible);
	bool getVisible();
	void setFreeFlag(bool free);
	bool getFreeFlag();
	void shoot(float, float);
	void completeShoot(Bullet *);
	Weapon* getWeapon();
	void rotate(Vec2 vec);
	void createBullet(Layer* scene);
	bool checkTimeWait();
	void updateTimeWait(float);
	void resetTimeWait();

	int getBaseHp();
	void die();
	void revieve();
};
#endif //_CHAMPION_H_
