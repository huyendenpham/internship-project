#ifndef __WEAPON_H__

#define __WEAPON_H__

#include "cocos2d.h"
USING_NS_CC;

class Weapon {
private:
	float dmg, attackSpeed;
	int bulletType, bulletNumb, rangeAttack;
public:
	Weapon();
	Weapon(int type);
	void setDmg(float dmg);
	float getDmg();
	void setAttackSpeed(float attackSpeed);
	float getAttackSpeed();
	void setBulletType(int type);
	int getBulletType();
	void setBulletNumb(int numb);
	int getBulletNumb();
	void setRangeAttack(int range);
	int getRangeAttack();
};


#endif