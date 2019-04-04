#ifndef __GAME_DATA_H__

#define __GAME_DATA_H__

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"

using namespace rapidjson;
USING_NS_CC;

class GameData{
private:
	static GameData* m_instance;
	GameData();
	int gunNormal, gunOption1, gunOption2, missileNormal, missileOption1, missileOption2, atNormal, atOption1, atOption2;
	int weapon1, weapon2, weapon3, weapon4;
	int armorLv;
	int volume;
	int BP;
	std::string nameCharacter;
public:
	
	static GameData* getInstance();
	int getGunNormal();
	void setGunNormal(int);
	int getGunOption1();
	void setGunOption1(int);
	int getGunOption2();
	void setGunOption2(int);

	int getMissileNormal();
	void setMissileNormal(int);
	int getMissileOption1();
	void setMissileOption1(int);
	int getMissileOption2();
	void setMissileOption2(int);

	int getAtNormal();
	void setAtNormal(int);
	int getAtOption1();
	void setAtOption1(int);
	int getAtOption2();
	void setAtOption2(int);

	int getWeapon1();
	int getWeapon2();
	int getWeapon3();
	int getWeapon4();

	int getArmorLv();

	int getBP();
	void setBP(int);

	std::string getNameCharacter();
	void setNameCharacter(std::string);

	int getVolume();
	void setVolume(int);

	void updateInfo();
	void writeInfo();
};


#endif
