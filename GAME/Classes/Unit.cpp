#include "Unit.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "SimpleAudioEngine.h"

using namespace rapidjson;

//-------------------------------
//Define funtion of class Unit
//-------------------------------


Unit::Unit()
{
}
Unit::Unit(cocos2d::Layer *scene, char *s)
{
	setName(s);
	m_sprite = Sprite::create();
	bg = Sprite::create();
	setFreeFlag(false);
	setVisible(false);
	this->locationPoint = 0;
	this->AddHpBar(scene);

	updateInfo();
	createBullet(scene);

	timeWait = attackSpeed;

	scene->addChild(bg, 3);
	scene->addChild(m_sprite, 3);


}
Unit::~Unit()
{
	//delete(unitName);
	//delete(loadingbar);
	//delete(loadingBarGB);
}

void Unit::updateInfo(){
	Document read_Json_Unit;
	ssize_t size3;

	char* buf3 = (char*)FileUtils::getInstance()->getFileData("data/dataUnit.json", "r", &size3);
	string content3(buf3, size3);

	read_Json_Unit.Parse(content3.c_str());
	m_sprite->setTexture(read_Json_Unit[unitName]["sprite"].GetString());
	this->bg->setTexture(read_Json_Unit[unitName]["bg"].GetString());

	setHp(read_Json_Unit[unitName]["hp"].GetInt());
	baseHp = getHp();
	setGold(read_Json_Unit[unitName]["gold"].GetInt());
	setAttackSpeed(read_Json_Unit[unitName]["attackSpeed"].GetFloat());
	setBulletNumb(read_Json_Unit[unitName]["bulletNumb"].GetInt());
	setRange(read_Json_Unit[unitName]["rangeAttack"].GetInt());
	setDmg(read_Json_Unit[unitName]["dmg"].GetFloat());
	setBulletType(read_Json_Unit[unitName]["bulletType"].GetInt());
}

void Unit::setPos(Vec2 pos){
	bg->setPosition(pos);
	if (!strcmp(unitName, "sl1") || !strcmp(unitName, "sl2") || !strcmp(unitName, "sl3")){
		m_sprite->setPosition(pos);
	}
	else if (!strcmp(unitName, "tank1") || !strcmp(unitName, "tank2")){
		m_sprite->setPositionX(pos.x + 10);
		m_sprite->setPositionY(pos.y);
	}
	else if (!strcmp(unitName, "plane1") || !strcmp(unitName, "plane2")){
		m_sprite->setPositionX(pos.x + 30);
		m_sprite->setPositionY(pos.y + 30);
	}
}

Vec2 Unit::getPos(){
	return bg->getPosition();
}

void Unit::createBullet(Layer* scene){
	for (int i = 0; i < bulletNumb; i++)
	{
		Bullet *bullet = new Bullet(bulletType, scene);
		bullet->setVisible(false);
		listBullet.push_back(bullet);
	}
}

void Unit::setVisible(bool v){
	visible = v;
	m_sprite->setVisible(v);
	bg->setVisible(v);
}

bool Unit::getVisible(){
	return visible;
}

void Unit::setName(char *name)
{
	unitName = name;
}
char* Unit::getName()
{
	return unitName;
}

void Unit::setAttackSpeed(float as){
	attackSpeed = as;
}
float Unit::getAttackSpeed(){
	return attackSpeed;
}

void Unit::setRange(int range){
	this->range = range;
}
int Unit::getRange(){
	return range;
}

vector<Bullet *> Unit::getBullet(){
	return listBullet;
}

void Unit::setBulletType(int type){
	bulletType = type;
}
int Unit::getBulletType(){
	return bulletType;
}

void Unit::setBulletNumb(int n){
	this->bulletNumb = n;
}

int Unit::getBulletNumb(){
	return bulletNumb;
}

void Unit::setHp(int hp)
{
	this->hp = hp;
}
int Unit::getHp()
{
	return hp;
}

void Unit::setDmg(float dmg)
{
	this->dmg = dmg;
}
float Unit::getDmg()
{
	return dmg;
}
void Unit::setGold(int gold)
{
	this->gold = gold;
}
int Unit::getGold()
{
	return gold;
}
void Unit::setStatus(bool stt)
{
	status = stt;
}
bool Unit::getStatus()
{
	return status;
}
void Unit::setLocationPoint(int lp)
{
	Unit::locationPoint = lp;
}
int Unit::getLocationPoint()
{
	return locationPoint;
}
void Unit::setFreeFlag(bool flag)
{
	freeFlag = flag;
}
bool Unit::getFreeFlag()
{
	return freeFlag;
}
void Unit::setLDBVisible(bool visible)
{
	hpBarBg->setVisible(visible);
	hpBar->setVisible(visible);
}
void Unit::AddHpBar(Layer *layer)
{
	hpBarBg = Sprite::create("unit/bgLdbarHeath.png");
	hpBarBg->setPosition(Vec2(this->m_sprite->getPositionX(), this->m_sprite->getPositionY() + this->m_sprite->getContentSize().height / 2 + 3));
	layer->addChild(hpBarBg, 3);
	hpBar = ui::LoadingBar::create("unit/LdbarHeath.png");
	hpBar->setPercent(100);
	hpBar->setDirection(ui::LoadingBar::Direction::LEFT);
	hpBar->setPosition(hpBarBg->getPosition());
	hpBarBg->setVisible(false);
	hpBar->setVisible(false);
	layer->addChild(hpBar, 3);


}
void Unit::updatePosHB()
{
	if (!strcmp(unitName, "plane1") || !strcmp(unitName, "plane2")) {
		hpBarBg->setPosition(Vec2(getPositionX(), getPositionY() + m_sprite->getContentSize().height / 2 + 1));
	}
	else
		hpBarBg->setPosition(Vec2(getPos().x, getPos().y + m_sprite->getContentSize().height / 2 + 1));
	hpBar->setPosition(hpBarBg->getPosition());
	float percent = (1.0)*hp / baseHp;
	hpBar->setPercent(percent * 100);
	////update Loading Bar
	//auto updateLoadingBar = CallFunc::create([]() {
	//	if (loadingbar->getPercent() < 100)
	//	{
	//		loadingbar->setPercent(loadingbar->getPercent() + 1);
	//	}
	//});

	//auto sequenceRunUpdateLoadingBar = Sequence::createWithTwoActions(updateLoadingBar, DelayTime::create(0.1f));
	//auto repeat = Repeat::create(sequenceRunUpdateLoadingBar, 100);
	//loadingbar->runAction(repeat);
}
void Unit::shoot(float x, float y)
{
	resetTimeWait();
	RotateTo* rotate;
	for (int i = 0; i < listBullet.size(); i++)
	{
		if (listBullet[i]->getVisible() == false)
		{
			listBullet[i]->setPosition(m_sprite->getPositionX(), m_sprite->getPositionY());
			listBullet[i]->setVisible(true);
			rotate = RotateTo::create(0.001, getCorner2(this->m_sprite->getPositionX(), this->m_sprite->getPositionY(), x, y));
			auto move = MoveTo::create(listBullet[i]->getSpeed()*getDistance(getPositionX(), getPositionY(), x, y), Vec2(x, y));
			auto cf = CallFunc::create(CC_CALLBACK_0(Unit::completeShoot, this, listBullet[i]));
			listBullet[i]->getSprite()->runAction(Sequence::create(rotate, move, cf, NULL));
		}
	}
	m_sprite->runAction(rotate->clone());
}
void Unit::completeShoot(Bullet *bullet)
{
	bullet->setVisible(false);
}

void Unit::runAct(Sequence* seq){
	m_sprite->runAction(seq->clone());
	bg->runAction(seq->clone());
}

Sprite* Unit::getBg(){
	return bg;
}

void Unit::die(){
	if (!strcmp(unitName, "sl1") || !strcmp(unitName, "sl2") || !strcmp(unitName, "sl3")){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/unit_dead.mp3");
	}
	else {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/explosion.mp3");
	}
	m_sprite->stopAllActions();
	bg->stopAllActions();
	auto callfunc = CallFunc::create(CC_CALLBACK_0(Unit::destroy, this));
	auto fo = FadeOut::create(0.5f);
	this->runAct(Sequence::create(DelayTime::create(0.3f), fo, callfunc, nullptr));
	hpBar->runAction(fo->clone());
	hpBarBg->runAction(fo->clone());
	setStatus(false);
}

void Unit::destroy()
{
	m_sprite->removeFromParent();
	bg->removeFromParent();
	hpBar->removeFromParent();
	hpBarBg->removeFromParent();
	for (int i = 0; i < bulletNumb; i++){
		listBullet[i]->destroy();
	}
}

void Unit::updateTimeWait(float dt){
	timeWait += dt;
}

bool Unit::checkTimeWait(){
	if (timeWait > attackSpeed) return true;
	return false;
}

void Unit::resetTimeWait(){
	timeWait = 0;
}

void Unit::stop(){
	m_sprite->stopAllActions();
	bg->stopAllActions();
}