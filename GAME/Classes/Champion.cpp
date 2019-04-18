#include "Champion.h"
#include "Bullet.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "SimpleAudioEngine.h"

using namespace rapidjson;

Champion::Champion(cocos2d::Layer *layer)
{
	m_sprite = cocos2d::Sprite::create();
	m_sprite->setVisible(false);
	freeFlag = true;
	layer->addChild(m_sprite, 2);
	setStatus(true);
	Document read_Json_MC;
	ssize_t size3;

	char* buf3 = (char*)FileUtils::getInstance()->getFileData("data/dataMC.json", "r", &size3);
	string content3(buf3, size3);

	read_Json_MC.Parse(content3.c_str());
	this->setHp(read_Json_MC["MC"]["hp"].GetInt());
	baseHp = getHp();
	this->setMoveSpeed(read_Json_MC["MC"]["moveSpeed"].GetFloat());
	this->setWeaponType(read_Json_MC["MC"]["weaponType"].GetInt());
	this->setArmorLv(read_Json_MC["MC"]["armorLv"].GetInt());

	switch (weaponType)
	{
	default:
	case 1:
		m_sprite->setTexture("Champion/hero_handgun.png");
		break;
	case 2:
		m_sprite->setTexture("Champion/hero_rifle.png");
		break;
	case 3:
		m_sprite->setTexture("Champion/hero_sniper.png");
		break;
	case 4:
		m_sprite->setTexture("Champion/hero_rpg.png");
		break;
	}

	weapon = new Weapon(weaponType);
	createBullet(layer);
	timeWait = weapon->getAttackSpeed();
}

void Champion::setHp(int hp)
{
	this->hp = hp;
}

int Champion::getHp()
{
	return hp;
}

void Champion::setMoveSpeed(float moveSpeed)
{
	this->moveSpeed = moveSpeed;
}

float Champion::getMoveSpeed()
{
	return moveSpeed;
}

void Champion::setWeaponType(int weaponType)
{
	this->weaponType = weaponType;
}

int Champion::getWeaponType()
{
	return weaponType;
}

void Champion::setArmorLv(int lv)
{
	armorLv = lv;
}

int Champion::getArmorLv()
{
	return armorLv;

}

void Champion::setFreeFlag(bool free)
{
	freeFlag = free;
}

bool Champion::getFreeFlag()
{
	return freeFlag;
}

void Champion::setVisible(bool visible)
{
	m_sprite->setVisible(visible);
}

bool Champion::getVisible()
{
	return m_sprite->isVisible();
}

void Champion::shoot(float x, float y)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/soldier_shot.mp3");
	for (int i = 0; i < listBullet.size(); i++)
	{
		resetTimeWait();
		if ((listBullet[i]->getVisible() == false))
		{
			auto bullet = listBullet[i];
			bullet->setPosition(m_sprite->getPositionX() + 15, m_sprite->getPositionY() + 12);
			auto rotate = RotateTo::create(0.001, getCorner2(this->m_sprite->getPositionX(), this->m_sprite->getPositionY(), x, y));
			auto rotate2 = RotateTo::create(0.001, getCorner2(this->m_sprite->getPositionX(), this->m_sprite->getPositionY(), x, y));
			this->m_sprite->runAction(rotate);
			bullet->setVisible(true);
			auto move = MoveTo::create(bullet->getSpeed()*getDistance(getPositionX(), getPositionY(), x, y), Vec2(x, y));
			auto callfunc1 = CallFunc::create(CC_CALLBACK_0(Champion::completeShoot, this, bullet));
			//auto callfunc2 = CC_CALLBACK_0(Bullet::setPosition, this, m_sprite->getPositionX(), m_sprite->getPositionY());
			auto sequen = Sequence::create(rotate2, move, callfunc1, NULL);
			bullet->getSprite()->runAction(sequen);
			break;
		}
	}
}

void Champion::completeShoot(Bullet *bullet)
{
	bullet->setVisible(false);
	//bullet->getSprite()->stopAllActions();
}

Weapon* Champion::getWeapon(){
	return weapon;
}

float getCurrentAngle(cocos2d::Node* node){
	float rotAng = node->getRotation();
	if (rotAng >= 180.f){
		rotAng -= 360.f;
	}
	else if (rotAng < -180.f){
		rotAng += 360.f;
	}
	return rotAng;
}

float getAngleDifference(float angle1, float angle2){
	float diffAngle = (angle1 - angle2);
	if (diffAngle >= 180.f){
		diffAngle -= 360.f;
	}
	else if (diffAngle < -180.f){
		diffAngle += 360.f;
	}

	return diffAngle;
}

void Champion::rotate(Vec2 vec){
	// xoay nguoi theo chieu di chuyen
	float nodeCurrentAngle = getCurrentAngle(m_sprite);
	float diffAngle = getAngleDifference(CC_RADIANS_TO_DEGREES(vec.getAngle()), nodeCurrentAngle);
	float rotation = nodeCurrentAngle + diffAngle;
	m_sprite->setRotation(-rotation);
}

void Champion::createBullet(Layer* scene){
	for (int i = 0; i < weapon->getBulletNumb(); i++)
	{
		Bullet *bullet = new Bullet(weapon->getBulletType(), scene);
		bullet->setVisible(false);
		listBullet.push_back(bullet);
	}
}

void Champion::updateTimeWait(float dt){
	timeWait += dt;
}

bool Champion::checkTimeWait(){
	if (timeWait > weapon->getAttackSpeed()) return true;
	return false;
}

void Champion::resetTimeWait(){
	timeWait = 0;
}

void Champion::setStatus(bool stt){
	status = stt;
}

bool Champion::getStatus(){
	return status;
}

void Champion::die(){
	setStatus(false);
	m_sprite->setVisible(false);
}

void Champion::revieve(){
	setStatus(true);
	m_sprite->setVisible(true);
	setHp(baseHp);
}

int Champion::getBaseHp(){
	return baseHp;
}