#include"Tower.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "GameData.h"
#include "SimpleAudioEngine.h"

using namespace rapidjson;


Tower::Tower()
{
}
Tower::Tower(int type, Vec2 pos, cocos2d::Layer *scene)
{

	this->rank = 0;
	setTowerType(type);
	sprite = Sprite::create();
	bg = Sprite::create();
	rankIcon = Sprite::create();
	sprite->setScale(0.7);
	bg->setScale(0.7);
	rankIcon->setScale(0.3);
	updateInfo();
	if (this->towerType == 3){
		sprite->setAnchorPoint(Vec2(0.5, 0.35));
	};
	setPos(pos);


	scene->addChild(bg);
	scene->addChild(sprite, 2);
	scene->addChild(rankIcon, 2);

	waiting = Sprite::create("tower/tower-waiting.png");
	waiting->setPosition(pos);
	scene->addChild(waiting, 2);
	waitingLabel = Label::create(StringUtils::toString(towerTimeBuild), "fonts/kenvector_future.ttf", 16);
	waitingLabel->setPosition(pos);
	waitingLabel->setTextColor(Color4B(0, 0, 0, 255));
	scene->addChild(waitingLabel, 2);

	bulletNumb = 20 * towerSpeedAttack;
	createBullet(scene);
	timeWait = towerSpeedAttack;

	createOption(scene);

}

void Tower::createBullet(Layer* scene){
	for (int i = 0; i < bulletNumb; i++)
	{
		Bullet *bullet = new Bullet(bulletType, scene);
		bullet->setVisible(false);
		listBullet.push_back(bullet);
	}
}

void Tower::updateInfo(){
	Document read_Json_Tower;
	ssize_t size3;

	char* buf3 = (char*)FileUtils::getInstance()->getFileData("data/dataTower.json", "r", &size3);
	string content3(buf3, size3);

	string towerCode = StringUtils::toString(this->towerType * 10 + this->rank);

	read_Json_Tower.Parse(content3.c_str());
	this->sprite->setTexture(read_Json_Tower[towerCode.c_str()]["sprite"].GetString());
	this->bg->setTexture(read_Json_Tower[towerCode.c_str()]["bg"].GetString());
	this->rankIcon->setTexture(read_Json_Tower[towerCode.c_str()]["rankIcon"].GetString());

	setGoldBuild(read_Json_Tower[towerCode.c_str()]["goldBuild"].GetInt());
	setGoldSell(read_Json_Tower[towerCode.c_str()]["goldSell"].GetInt());

	towerTimeBuild = read_Json_Tower[towerCode.c_str()]["timeBuild"].GetFloat();
	setTowerDmg(read_Json_Tower[towerCode.c_str()]["damage"].GetFloat());
	setTowerRangeAttack(read_Json_Tower[towerCode.c_str()]["range"].GetFloat());
	setTowerSpeedAttack(read_Json_Tower[towerCode.c_str()]["speed"].GetFloat());
	setBulletType(read_Json_Tower[towerCode.c_str()]["bulletType"].GetFloat());
}

void Tower::setPos(Vec2 pos){
	this->pos = pos;
	sprite->setPosition(pos);

	bg->setPosition(pos);
	rankIcon->setPosition(pos.x + 25, pos.y + 25);
}

Vec2 Tower::getPos(){
	return pos;
}

void Tower::setTowerDmg(float dmg)
{
	towerDmg = dmg;
}

void Tower::setTowerRangeAttack(int range)
{
	towerRangeAttack = range;
}

int Tower::getTowerRangeAttack()
{
	return towerRangeAttack;
}

void Tower::setTowerSpeedAttack(float speedAttack)
{
	towerSpeedAttack = speedAttack;
}

float Tower::getTowerSpeedAttack()
{
	return towerSpeedAttack;
}

void Tower::setTowerType(int k)
{
	towerType = k;
}

float Tower::getTowerDmg()
{
	return towerDmg;
}

int Tower::getGoldBuild()
{
	return goldBuild;
}

void Tower::updateTowerTimeBuild(float time)
{
	towerTimeBuild -= time;
	if (towerTimeBuild <= 0) {
		if (waiting->isVisible()) showOption();
		waiting->setVisible(false);
		waitingLabel->setVisible(false);
	}
	else {
		waitingLabel->setString(StringUtils::toString(1.0*((int)(towerTimeBuild * 100)) / 100));
	}
}

float Tower::getTowerTimeBuild()
{
	return towerTimeBuild;
}

void Tower::upRank(int inc = 1)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/tower_up.mp3");
	this->rank += inc;
	updateInfo();
}

int Tower::getTowerType()
{
	return towerType;
}

void Tower::setGoldBuild(int gold)
{
	goldBuild = gold;
}

int Tower::getGoldBuild(int type, int rank = 0){
	Document read_Json_Tower;
	ssize_t size3;
	char* buf3 = (char*)FileUtils::getInstance()->getFileData("data/dataTower.json", "r", &size3);
	string content3(buf3, size3);
	string towerCode = StringUtils::toString(type * 10 + rank);
	read_Json_Tower.Parse(content3.c_str());

	return read_Json_Tower[towerCode.c_str()]["goldBuild"].GetInt();
}

int Tower::getGoldSell(){
	return goldSell;
}

void Tower::setGoldSell(int gold){
	goldSell = gold;
}

int Tower::getRank()
{
	return rank;
}

vector<Bullet*> Tower::getBullet() {
	return listBullet;
}

void Tower::rotate()
{

}

Rect Tower::getBox(){
	return bg->getBoundingBox();
}

void Tower::setBulletType(int type){
	bulletType = type;
}

int Tower::getBulletType(){
	return bulletType;
}

void Tower::destroy(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/sell_tower.mp3");

	sprite->removeFromParent();
	bg->removeFromParent();
	rankIcon->removeFromParent();
	waiting->removeFromParent();
	waitingLabel->removeFromParent();
	label1->removeFromParent();
	label2->removeFromParent();
	label3->removeFromParent();
	range->removeFromParent();
	labelBg1->removeFromParent();
	labelBg2->removeFromParent();
	labelBg3->removeFromParent();
	upgradeBg->removeFromParent();
	upgradeBg1->removeFromParent();
	upgradeIcon->removeFromParent();
	upgradeIcon1->removeFromParent();
	sellIcon->removeFromParent();

	for (int i = 0; i < bulletNumb; i++){
		listBullet[i]->destroy();

	}
}

void Tower::shoot(float x, float y)
{
	if (towerType == 1){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/gun_shot.mp3");
	}
	else if (towerType == 2){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/missile_shot.mp3");
	}
	else CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/tank_fire.mp3");
	resetTimeWait();
	for (int i = 0; i < listBullet.size(); i++)
	{
		if ((listBullet[i]->getVisible() == false))
		{
			auto bullet = listBullet[i];
			bullet->setPosition(sprite->getPositionX(), sprite->getPositionY());
			auto rotate = RotateTo::create(0.001, getCorner2(this->sprite->getPositionX(), this->sprite->getPositionY(), x, y) + 90);
			auto rotate2 = RotateTo::create(0.001, getCorner2(this->sprite->getPositionX(), this->sprite->getPositionY(), x, y));
			this->sprite->runAction(rotate);
			bullet->setVisible(true);
			auto move = MoveTo::create(bullet->getSpeed()*getDistance(x, y, getPos().x, getPos().y), Vec2(x, y));
			auto callfunc1 = CallFunc::create(CC_CALLBACK_0(Tower::completeShoot, this, bullet));
			//auto callfunc2 = CC_CALLBACK_0(Bullet::setPosition, this, m_sprite->getPositionX(), m_sprite->getPositionY());
			auto sequen = Sequence::create(rotate2, move, callfunc1, NULL);
			bullet->getSprite()->runAction(sequen);
			break;
		}
	}
}

void Tower::completeShoot(Bullet *bullet)
{
	bullet->setVisible(false);
	//bullet->getSprite()->stopAllActions();
}

void Tower::updateTimeWait(float dt){
	timeWait += dt;
}

bool Tower::checkTimeWait(){
	if (timeWait > towerSpeedAttack) return true;
	return false;
}

void Tower::resetTimeWait(){
	timeWait = 0;
}

void Tower::createOption(Layer* scene){
	range = Sprite::create("tower/range.png");
	range->setPosition(getPos());
	range->setVisible(false);
	scene->addChild(range, 1);

	upgradeBg = Sprite::create("upgrade_rank/tower-upgrade.png");
	upgradeBg->setVisible(false);
	upgradeBg->setScale(0.5);
	scene->addChild(upgradeBg, 3);

	upgradeBg1 = Sprite::create("upgrade_rank/tower-upgrade.png");
	upgradeBg1->setVisible(false);
	upgradeBg1->setScale(0.5);
	scene->addChild(upgradeBg1, 3);

	upgradeIcon = Sprite::create();
	upgradeIcon->setVisible(false);
	upgradeIcon->setScale(0.25);
	scene->addChild(upgradeIcon, 3);

	upgradeIcon1 = Sprite::create();
	upgradeIcon1->setVisible(false);
	upgradeIcon1->setScale(0.25);
	scene->addChild(upgradeIcon1, 3);

	sellIcon = Sprite::create("upgrade_rank/tower-destroy.png");
	sellIcon->setVisible(false);
	sellIcon->setScale(0.5);
	sellIcon->setPosition(getPos().x, getPos().y - 80);
	scene->addChild(sellIcon, 3);

	labelBg1 = Sprite::create("upgrade_rank/option_label_bg.png");
	labelBg1->setVisible(false);
	scene->addChild(labelBg1, 3);

	labelBg2 = Sprite::create("upgrade_rank/option_label_bg.png");
	labelBg2->setVisible(false);
	scene->addChild(labelBg2, 3);

	labelBg3 = Sprite::create("upgrade_rank/option_label_bg.png");
	labelBg3->setVisible(false);
	labelBg3->setPosition(sellIcon->getPositionX(), sellIcon->getPositionY() - 20);
	scene->addChild(labelBg3, 3);

	label1 = Label::create("", "fonts/kenvector_future.ttf", 12);
	label1->setTextColor(Color4B(0, 0, 0, 255));
	label1->setVisible(false);
	scene->addChild(label1, 3);

	label2 = Label::create("", "fonts/kenvector_future.ttf", 12);
	label2->setTextColor(Color4B(0, 0, 0, 255));
	label2->setVisible(false);
	scene->addChild(label2, 3);


	label3 = Label::create("", "fonts/kenvector_future.ttf", 12);
	label3->setTextColor(Color4B(0, 0, 0, 255));
	label3->setVisible(false);
	label3->setPosition(labelBg3->getPosition());
	scene->addChild(label3, 3);
}

void Tower::updateOption(){
	range->setScale((1.0)*towerRangeAttack * 2 / 490);
	if (rank < 3) {
		upgradeBg->setPosition(getPos().x, getPos().y + 80);
		upgradeIcon->setTexture("upgrade_rank/rank-" + StringUtils::toString((rank + 1)) + ".png");
		upgradeIcon->setPosition(upgradeBg->getPositionX(), upgradeBg->getPositionY() + 10);
		labelBg1->setPosition(upgradeBg->getPositionX(), upgradeBg->getPositionY() - 20);
		label1->setPosition(labelBg1->getPosition());
		label1->setString("LV" + StringUtils::toString(rank + 1) + " - " + StringUtils::toString(getGoldBuild(towerType, rank + 1)) + "G");
	}
	else if (rank == 3){
		string name = "";
		if (towerType == 1) name = "gun-";
		if (towerType == 2) name = "missile-";
		if (towerType == 3) name = "cannon-";
		upgradeBg->setPosition(getPos().x - 70, getPos().y + 80);
		upgradeIcon->setTexture("upgrade_rank/rank-" + name + StringUtils::toString((1)) + ".png");
		upgradeIcon->setPosition(upgradeBg->getPositionX(), upgradeBg->getPositionY() + 10);
		labelBg1->setPosition(upgradeBg->getPositionX(), upgradeBg->getPositionY() - 20);
		label1->setPosition(labelBg1->getPosition());
		label1->setString("OP1 - " + StringUtils::toString(getGoldBuild(towerType, rank + 1)) + "G");

		upgradeBg1->setPosition(getPos().x + 70, getPos().y + 80);
		upgradeIcon1->setTexture("upgrade_rank/rank-" + name + StringUtils::toString((2)) + ".png");
		upgradeIcon1->setPosition(upgradeBg1->getPositionX(), upgradeBg1->getPositionY() + 10);
		labelBg2->setPosition(upgradeBg1->getPositionX(), upgradeBg1->getPositionY() - 20);
		label2->setPosition(labelBg2->getPosition());
		label2->setString("OP2 - " + StringUtils::toString(getGoldBuild(towerType, rank + 1)) + "G");
	}

	label3->setString("DES. - " + StringUtils::toString(getGoldSell()) + "G");

}

void Tower::showOption(){
	updateOption();
	range->setVisible(true);
	sellIcon->setVisible(true);
	label3->setVisible(true);
	labelBg3->setVisible(true);
	if (getTowerTimeBuild() <= 0){
		if (rank < 3){
			int rankAvailable;
			if (getTowerType() == 1){
				rankAvailable = GameData::getInstance()->getGunNormal();
			}
			if (getTowerType() == 2){
				rankAvailable = GameData::getInstance()->getMissileNormal();
			}
			if (getTowerType() == 3){
				rankAvailable = GameData::getInstance()->getAtNormal();
			}
			if (rank + 1 <= rankAvailable){
				upgradeBg->setVisible(true);
				upgradeIcon->setVisible(true);
				label1->setVisible(true);
				labelBg1->setVisible(true);
			}
		}
		else if (rank == 3){
			int op1, op2;
			if (getTowerType() == 1){
				op1 = GameData::getInstance()->getGunOption1();
				op2 = GameData::getInstance()->getGunOption2();
			}
			if (getTowerType() == 2){
				op1 = GameData::getInstance()->getMissileOption1();
				op2 = GameData::getInstance()->getMissileOption2();
			}
			if (getTowerType() == 3){
				op1 = GameData::getInstance()->getAtOption1();
				op2 = GameData::getInstance()->getAtOption2();
			}
			if (op1 == 1){
				upgradeBg->setVisible(true);
				upgradeIcon->setVisible(true);
				label1->setVisible(true);
				labelBg1->setVisible(true);
			} 
			if (op2 == 1){
				upgradeBg1->setVisible(true);
				upgradeIcon1->setVisible(true);
				label2->setVisible(true);
				labelBg2->setVisible(true);
			}
		}

	}
}

void Tower::hideOption(){
	range->setVisible(false);
	sellIcon->setVisible(false);
	upgradeBg->setVisible(false);
	upgradeIcon->setVisible(false);
	upgradeBg1->setVisible(false);
	upgradeIcon1->setVisible(false);
	label1->setVisible(false);
	label2->setVisible(false);
	label3->setVisible(false);
	labelBg1->setVisible(false);
	labelBg2->setVisible(false);
	labelBg3->setVisible(false);
}

bool Tower::isRangeShow(){
	return range->isVisible();
}



int Tower::checkOptionClick(Vec2 pos){
	if (upgradeBg->getBoundingBox().containsPoint(pos)){
		return 1;
	}
	if (upgradeBg1->getBoundingBox().containsPoint(pos)){
		return 2;
	}
	if (sellIcon->getBoundingBox().containsPoint(pos)){
		return 3;
	}

	return 0;
}
