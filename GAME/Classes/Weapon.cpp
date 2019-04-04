#include "Weapon.h"
#include "json/rapidjson.h"
#include "json/document.h"

using namespace rapidjson;

Weapon::Weapon(){

}

Weapon::Weapon(int type){
	Document read_Json_MC;
	ssize_t size3;

	char* buf3 = (char*)FileUtils::getInstance()->getFileData("data/dataWeapon.json", "r", &size3);
	std::string content3(buf3, size3);

	std::string weaponType = StringUtils::toString(type);

	read_Json_MC.Parse(content3.c_str());
	this->setAttackSpeed(read_Json_MC[weaponType.c_str()]["attackSpeed"].GetFloat());
	this->setBulletNumb(read_Json_MC[weaponType.c_str()]["bulletNumb"].GetInt());
	this->setRangeAttack(read_Json_MC[weaponType.c_str()]["rangeAttack"].GetInt());
	this->setDmg(read_Json_MC[weaponType.c_str()]["dmg"].GetFloat());
	this->setBulletType(read_Json_MC[weaponType.c_str()]["bulletType"].GetInt());
}

void Weapon::setDmg(float dmg){
	this->dmg = dmg;
};
float Weapon::getDmg(){
	return dmg;
};
void Weapon::setAttackSpeed(float attackSpeed){
	this->attackSpeed = attackSpeed;
}
float Weapon::getAttackSpeed(){
	return attackSpeed;
}
void Weapon::setBulletType(int type){
	this->bulletType = type;
}
int Weapon::getBulletType(){
	return bulletType;
}
void Weapon::setBulletNumb(int numb){
	this->bulletNumb = numb;
}
int Weapon::getBulletNumb(){
	return bulletNumb;
}
void Weapon::setRangeAttack(int range){
	this->rangeAttack = range;
}
int Weapon::getRangeAttack(){
	return rangeAttack;
}