#include "GameData.h"


GameData* GameData::m_instance = NULL;

GameData::GameData() {
	updateInfo();
}

GameData* GameData::getInstance(){
	if (m_instance == NULL){
		m_instance = new GameData();
	}

	return m_instance;
}

void GameData::updateInfo(){
	Document read_Json_Data;
	ssize_t size3;

	char* buf3 = (char*)FileUtils::getInstance()->getFileData("data/data.json", "r", &size3);
	std::string content3(buf3, size3);

	read_Json_Data.Parse(content3.c_str());
	gunNormal = read_Json_Data["gun"]["normal"].GetInt();
	gunOption1 = read_Json_Data["gun"]["option1"].GetInt();
	gunOption2 = read_Json_Data["gun"]["option2"].GetInt();

	missileNormal = read_Json_Data["missile"]["normal"].GetInt();
	missileOption1 = read_Json_Data["missile"]["option1"].GetInt();
	missileOption2 = read_Json_Data["missile"]["option2"].GetInt();

	atNormal = read_Json_Data["at"]["normal"].GetInt();
	atOption1 = read_Json_Data["at"]["option1"].GetInt();
	atOption2 = read_Json_Data["at"]["option2"].GetInt();

	weapon1 = read_Json_Data["weapon"]["weapon1"].GetInt();
	weapon2 = read_Json_Data["weapon"]["weapon2"].GetInt();
	weapon3 = read_Json_Data["weapon"]["weapon3"].GetInt();
	weapon4 = read_Json_Data["weapon"]["weapon4"].GetInt();

	armorLv = read_Json_Data["armor"]["armorLv"].GetInt();
	volume = read_Json_Data["volume"].GetInt();
	BP = read_Json_Data["BP"].GetInt();
	nameCharacter = read_Json_Data["nameCharacter"].GetString();
}

void writeInfo(){
//	std::string str="MINH";
	
}

void GameData::setNameCharacter(std::string name){
	nameCharacter = name;
}

std::string GameData::getNameCharacter(){
	return nameCharacter;
}

int GameData::getGunNormal(){
	return gunNormal;
};
void GameData::setGunNormal(int rank){
	gunNormal = rank;
}

int GameData::getGunOption1(){
	return gunOption1;
};
void GameData::setGunOption1(int rank){
	gunOption1 = rank;
}

int GameData::getGunOption2(){
	return gunOption2;
};
void GameData::setGunOption2(int rank){
	gunOption2 = rank;
}

int GameData::getMissileNormal(){
	return missileNormal;
};
void GameData::setMissileNormal(int rank){
	missileNormal = rank;
}

int GameData::getMissileOption1(){
	return missileOption1;
};
void GameData::setMissileOption1(int rank){
	missileOption1 = rank;
}

int GameData::getMissileOption2(){
	return missileOption2;
};
void GameData::setMissileOption2(int rank){
	missileOption2 = rank;
}

int GameData::getAtNormal(){
	return atNormal;
};
void GameData::setAtNormal(int rank){
	atNormal = rank;
}

int GameData::getAtOption1(){
	return atOption1;
};
void GameData::setAtOption1(int rank){
	atOption1 = rank;
}

int GameData::getAtOption2(){
	return atOption2;
};
void GameData::setAtOption2(int rank){
	atOption2 = rank;
}


int GameData::getWeapon1(){
	return weapon1;
};
int GameData::getWeapon2(){
	return weapon2;
};
int GameData::getWeapon3(){
	return weapon3;
};
int GameData::getWeapon4(){
	return weapon4;
};

int GameData::getArmorLv(){
	return armorLv;
};

int GameData::getBP(){
	return BP;
}

void GameData::setBP(int bp){
	this->BP = bp;
}

int GameData::getVolume(){
	return volume;
}

void GameData::setVolume(int volume){
	this->volume = volume;
}