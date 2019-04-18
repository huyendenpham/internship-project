#include "GamePlay.h"
#include "ui/CocosGUI.h"
#include "Level.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "Wave.h"
#include "MissionScene.h"
#include "MainMenuScene.h"
#include "GameData.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;
using namespace rapidjson;

Level *level;
int flag;
Sprite* tempSprite, *bg;
ui::Button *yesBtn, *noBtn;
Label* label;
int wave;
float unitDelayGround, unitDelaySky, skillGameDelay, skillHeroDelay;


Scene* GamePlayScene::createScene(int lv){
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	level = new Level(lv);
	auto layer = GamePlayScene::create();
	scene->addChild(layer);

	return scene;
}

bool GamePlayScene::init(){
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	flag = 0;
	wave = 0;
	unitDelayGround = 0;
	unitDelaySky = 0;
	skillGameDelay = 0;
	skillHeroDelay = 90;

	tempSprite = Sprite::create();
	tempSprite->setVisible(false);
	tempSprite->setScale(0.7);
	addChild(tempSprite);

	MapLayer = cocos2d::Layer::create();

	MissionSuccessLayer = Layer::create();
	MissionFailLayer = Layer::create();
	addChild(MissionSuccessLayer, 5);
	addChild(MissionFailLayer, 5);

	myHero = new Champion(MapLayer);
	addChild(MapLayer, -1);
	// create map
	createTiledMap();
	// create UI layer
	UILayer = new UI(level->numoflevel);
	this->addChild(UILayer, 4);
	// load list point position from map
	loadListPoint();

	// Hero init
	myHero->setVisible(true);
	myHero->setPosition(locationPoint[3].at("x").asInt(), locationPoint[3].at("y").asInt());
	auto physicsBody = PhysicsBody::createBox(Size(myHero->getSprite()->getContentSize()), PhysicsMaterial(0.0f, 0.0f, 10.0f));
	physicsBody->setContactTestBitmask(true);
	physicsBody->setCollisionBitmask(1);
	physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(true);
	physicsBody->setMass(100);
	myHero->getSprite()->setPhysicsBody(physicsBody);

	// update info
	gold = level->getGold();
	lives = level->getLives();
	tempGround = level->listWave[wave]->createListUnitGround(this, Vec2(locationPoint[0].at("x").asInt(), locationPoint[0].at("y").asInt()));
	tempSky = level->listWave[wave]->createListUnitSky(this, Vec2(locationPointSky[0].at("x").asInt(), locationPointSky[0].at("y").asInt()));

	// add touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GamePlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GamePlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GamePlayScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm1.mp3", true);
	// run update 
	scheduleUpdate();

	return true;
}

void GamePlayScene::createTiledMap(){
	tileMap = TMXTiledMap::create(level->mapPath.c_str());
	tileMap->setAnchorPoint(Vec2(0, 0));
	tileMap->setPosition(Vec2(0, 0));

	MapLayer->addChild(tileMap, -1);

	mPhysicsLayer = tileMap->getLayer("Physic");
	mPhysicsLayer->setVisible(false);

	Size layerSize = mPhysicsLayer->getLayerSize();
	for (int i = 0; i < layerSize.width; i++)
	{
		for (int j = 0; j < layerSize.height; j++)
		{
			auto tileSet = mPhysicsLayer->getTileAt(Vec2(i, j));
			if ((tileSet != NULL))
			{
				auto physics = PhysicsBody::createBox(tileSet->getContentSize(), PhysicsMaterial(0.0f, 1.0f, 0.2f));
				physics->setCollisionBitmask(2);
				physics->setContactTestBitmask(true);
				physics->setDynamic(false);
				tileSet->setPhysicsBody(physics);
			}
		}
	}
}

void GamePlayScene::loadListPoint(){
	auto playerTile = tileMap->getObjectGroup("LocationPoint");
	ValueMap Point;
	auto object = playerTile->getObjects();
	int size = 1;
	do {
		char s[5] = "";
		sprintf(s, "LP%d", size);
		Point = playerTile->objectNamed(s);
		locationPoint.push_back(Point);
		size++;
	} while (size <= object.size());

	auto playerTile3 = tileMap->getObjectGroup("LocationPointSky");
	object = playerTile3->getObjects();
	int size3 = 1;
	do {
		char s[6] = "";
		sprintf(s, "LPS%d", size3);
		Point = playerTile3->objectNamed(s);
		locationPointSky.push_back(Point);
		size3++;
	} while (size3 <= object.size());

	auto playerTile2 = tileMap->getObjectGroup("TowerLocation");

	object = playerTile2->getObjects();
	int size2 = 1;
	do {
		char s[5] = "";
		sprintf(s, "T%d", size2);
		Point = playerTile2->objectNamed(s);
		towerLocation.push_back(Point);
		size2++;
	} while (size2 <= tileMap->getObjectGroup("TowerLocation")->getObjects().size());
}

bool GamePlayScene::onTouchBegan(Touch* touch, Event  *event){
	if (UILayer->isMenuOpen() == false){
		if (UILayer->getAT()->getBoundingBox().containsPoint(touch->getLocation())){
			tempSprite->setTexture("tower/at-cannon.png");
			tempSprite->setPosition(touch->getLocation());
			tempSprite->setVisible(true);
			flag = 3;
			return true;
		}
		if (UILayer->getMi()->getBoundingBox().containsPoint(touch->getLocation())){
			tempSprite->setTexture("tower/missile.png");
			tempSprite->setPosition(touch->getLocation());
			tempSprite->setVisible(true);
			flag = 2;
			return true;
		}
		if (UILayer->getGun()->getBoundingBox().containsPoint(touch->getLocation())){
			tempSprite->setTexture("tower/machine-gun.png");
			tempSprite->setPosition(touch->getLocation());
			tempSprite->setVisible(true);
			flag = 1;
			return true;
		}
		if (skillHeroDelay <= 0){
			if (UILayer->checkSkillUsed(touch->getLocation()) == 1){
				flag = 4;
				tempSprite->setTexture("skill_hero1_used.png");
				tempSprite->setPosition(touch->getLocation());
				tempSprite->setVisible(true);
				return true;
			}
		}
		if (skillGameDelay <= 0 && myHero->getStatus() == false){
			if (UILayer->checkSkillUsed(touch->getLocation()) == 2){
				if (GameData::getInstance()->getBP() >= 500){
					 bg = Sprite::create("mission-bg.png");
					bg->setScale(0.5);
					bg->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
					this->addChild(bg,4);
					 label = Label::create("Use 500 BP \nto revieve Hero?\n(" + StringUtils::toString(GameData::getInstance()->getBP()) + " BP lefts)", "fonts/kenvector_future.ttf", 25);
					label->setHorizontalAlignment(CCTextAlignment::CENTER);
					label->setPosition(bg->getPositionX(), bg->getPositionY() + 40);
					this->addChild(label,4);
					yesBtn = ui::Button::create("UI/btnYes.png", "UI/btnYesPress.png");
					 noBtn = ui::Button::create("UI/btnNo.png", "UI/btnNoPress.png");
					 yesBtn->setPosition(Vec2(bg->getPositionX() - 100, bg->getPositionY() - 50));
					 yesBtn->setScale(1.2);
					 yesBtn->addClickEventListener([&](Ref* event){
						myHero->revieve();
						skillGameDelay = 300;
						GameData::getInstance()->setBP(GameData::getInstance()->getBP() - 500);
						label->removeFromParent();
						yesBtn->removeFromParent();
						noBtn->removeFromParent();
						bg->removeFromParent();
					});

					
					 noBtn->setPosition(Vec2(bg->getPositionX() + 100, bg->getPositionY() - 50));
					 noBtn->setScale(1.2);
					 noBtn->addClickEventListener([&](Ref* event){
						
						label->removeFromParent();
						yesBtn->removeFromParent();
						noBtn->removeFromParent();
						bg->removeFromParent();
					});
					 this->addChild(yesBtn,4);
					this->addChild(noBtn,4);
					return false;
				}
			}
		}


		for (int i = 0; i < towerList.size(); i++){
			if (towerList[i]->getBox().containsPoint(touch->getLocation())){
				if (towerList[i]->isRangeShow()){
					towerList[i]->hideOption();
				}
				else 	towerList[i]->showOption();
				return false;
			}
			if (towerList[i]->isRangeShow()){
				int normal,op1, op2;
				if (towerList[i]->getTowerType() == 1){
					normal = GameData::getInstance()->getGunNormal();
					op1 = GameData::getInstance()->getGunOption1();
					op2 = GameData::getInstance()->getGunOption2();
				}
				if (towerList[i]->getTowerType() == 2){
					normal = GameData::getInstance()->getMissileNormal();
					op1 = GameData::getInstance()->getMissileOption1();
					op2 = GameData::getInstance()->getMissileOption2();
				}
				if (towerList[i]->getTowerType() == 3){
					normal = GameData::getInstance()->getAtNormal();
					op1 = GameData::getInstance()->getAtOption1();
					op2 = GameData::getInstance()->getAtOption2();
				}
				if (towerList[i]->checkOptionClick(touch->getLocation()) == 1 && towerList[i]->getTowerTimeBuild() <= 0) {
					if (towerList[i]->getRank() <= 3 && gold >= Tower::getGoldBuild(towerList[i]->getTowerType(), towerList[i]->getRank() + 1)){
						if (towerList[i]->getRank() == 3 && op1 == 1){
							gold -= Tower::getGoldBuild(towerList[i]->getTowerType(), towerList[i]->getRank() + 1);
							towerList[i]->upRank(1);
							towerList[i]->hideOption();
							towerList[i]->showOption();
						}
						if (towerList[i]->getRank() < 3 && towerList[i]->getRank() + 1 <= normal){
							gold -= Tower::getGoldBuild(towerList[i]->getTowerType(), towerList[i]->getRank() + 1);
							towerList[i]->upRank(1);
							towerList[i]->hideOption();
							towerList[i]->showOption();
						}
						
					}
				}
				if (towerList[i]->checkOptionClick(touch->getLocation()) == 2 && towerList[i]->getTowerTimeBuild() <= 0) {
					if (op2 == 1 && towerList[i]->getRank() == 3 && gold >= Tower::getGoldBuild(towerList[i]->getTowerType(), towerList[i]->getRank() + 2)){
						gold -= Tower::getGoldBuild(towerList[i]->getTowerType(), towerList[i]->getRank() + 2);
						towerList[i]->upRank(2);
						towerList[i]->hideOption();
						towerList[i]->showOption();
					}
				}
				if (towerList[i]->checkOptionClick(touch->getLocation()) == 3){
					gold += towerList[i]->getGoldSell();
					Tower *tower = towerList[i];
					towerList.erase(towerList.begin() + i);
					tower->destroy();
				}
			}
		}
	}


	return false;
}

void GamePlayScene::onTouchMoved(Touch* touch, Event  *event){
	if (flag == 1 || flag == 2 || flag == 3 || flag == 4){
		tempSprite->setPosition(touch->getLocation());
	}
}

bool GamePlayScene::checkTower(Vec2 pos){
	for (int i = 0; i < towerList.size(); i++){
		if (towerList[i]->getPos() == pos){
			return true;
		}
	}
	return false;
}

void GamePlayScene::onTouchEnded(Touch* touch, Event  *event){
	if (flag == 1 || flag == 2 || flag == 3){
		for (int i = 0; i < towerLocation.size(); i++){
			if (!checkTower(Vec2(towerLocation[i].at("x").asInt(), towerLocation[i].at("y").asInt())) && tempSprite->getBoundingBox().containsPoint(Vec2(towerLocation[i].at("x").asInt(), towerLocation[i].at("y").asInt()))){
				buildTower(flag, Vec2(towerLocation[i].at("x").asInt(), towerLocation[i].at("y").asInt()));
				break;
			}
		}
	}

	if (flag == 4) {
		if (UILayer->checkSkillUsed(touch->getLocation()) != 1){
			for (int i = 0; i < (tempGround).size(); i++){
				if (tempGround[i]->getVisible()){
					Unit *tempUnit = tempGround[i];
					if (tempSprite->getBoundingBox().containsPoint(tempUnit->getPos())){
						tempUnit->setLDBVisible(true);
						tempUnit->setHp(tempUnit->getHp() - 200);
						if (tempUnit->getHp() <= 0){
							(tempGround).erase(tempGround.begin() + i);
							i--;
							tempUnit->updatePosHB();
							tempUnit->die();
							level->listWave[wave]->decTotalUnit();
							gold += tempUnit->getGold();
						}
					}

				}
			}
			skillHeroDelay = 180;
		}
	}
	tempSprite->setVisible(false);
	flag = 0;
}

void GamePlayScene::buildTower(int type, Vec2 pos){

	if (gold >= Tower::getGoldBuild(type, 0)){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/build_tower.mp3");
		Tower* tower = new Tower(type, pos, this);
		gold -= tower->getGoldBuild();
		towerList.push_back(tower);
	}
	else {

	}

}

void GamePlayScene::updateUI(){
	UILayer->getGoldLabel()->setString(StringUtils::toString(gold));
	UILayer->getLiveLabel()->setString(StringUtils::toString(lives));
	UILayer->getHpLabel()->setString(StringUtils::toString(myHero->getHp()) + "/" + StringUtils::toString(myHero->getBaseHp()));
	UILayer->getHpBar()->setPercent(myHero->getHp());
	UILayer->getWaveLabel()->setString("Wave " + StringUtils::toString(wave + 1) + "/" + StringUtils::toString(level->numOfWave));
	if (skillHeroDelay <= 0) {
		UILayer->getSkillHeroLabel()->setString("Ready");
	}
	else {
		UILayer->getSkillHeroLabel()->setString("CD " + StringUtils::toString((int)skillHeroDelay) + " S");
	}
	if (skillGameDelay <= 0) {
		UILayer->getSkillGameLabel()->setString("500 BP");
	}
	else {
		UILayer->getSkillGameLabel()->setString("CD " + StringUtils::toString((int)skillGameDelay) + " S");
	}
}

void GamePlayScene::updateUnit(float delta)
{
	for (int i = 0; i < (tempGround).size(); i++)
	{
		if (tempGround[i]->getVisible())
		{
			if (myHero->getStatus()){
				tempGround[i]->updateTimeWait(delta);
				if (tempGround[i]->checkTimeWait()){
					float distance = getDistance(tempGround[i]->getPositionX(), tempGround[i]->getPositionY(), myHero->getPositionX(), myHero->getPositionY());
					if (distance < tempGround[i]->getRange()){
						tempGround[i]->shoot(myHero->getPositionX(), myHero->getPositionY());
						myHero->setHp(myHero->getHp() - tempGround[i]->getDmg());
					}
				}
			}

			tempGround[i]->updatePosHB();
		}
	}

	for (int i = 0; i < (tempSky).size(); i++)
	{
		if (tempSky[i]->getVisible())
		{
			tempSky[i]->updatePosHB();
		}
	}
	unitDelayGround += delta;
	unitDelaySky += delta;
	if (unitDelayGround > level->listWave[wave]->getDelayTimeGround())
	{
		unitDelayGround = 0;
		for (int i = 0; i < tempGround.size(); i++)
		{
			auto startPos = locationPoint[0];
			auto x = startPos.at("x").asInt();
			auto y = startPos.at("y").asInt();
			if ((tempGround[i]->getVisible() == false) && (tempGround[i]->getPos().x == x) && (tempGround[i]->getPos().y == y))
			{
				tempGround[i]->setStatus(true);
				tempGround[i]->setVisible(true);
				tempGround[i]->setFreeFlag(true);
				tempGround[i]->setLocationPoint(tempGround[i]->getLocationPoint() + 1);
				break;
			}
		}
	}
	if (unitDelaySky > level->listWave[wave]->getDelayTimeSky())
	{
		unitDelaySky = 0;
		for (int i = 0; i < tempSky.size(); i++)
		{
			auto startPos = locationPointSky[0];
			auto x = startPos.at("x").asInt();
			auto y = startPos.at("y").asInt();
			if ((tempSky[i]->getVisible() == false) && (tempSky[i]->getPos().x == x) && (tempSky[i]->getPos().y == y))
			{
				tempSky[i]->setStatus(true);
				tempSky[i]->setVisible(true);
				tempSky[i]->setFreeFlag(true);
				tempSky[i]->setLocationPoint(tempSky[i]->getLocationPoint() + 1);
				break;
			}
		}
	}
	for (int i = 0; i < tempGround.size(); i++)
	{
		if ((tempGround[i]->getVisible()) && (tempGround[i]->getFreeFlag()))
		{
			tempGround[i]->setFreeFlag(false);
			auto movePos = locationPoint[tempGround[i]->getLocationPoint()];
			auto x = movePos.at("x").asInt();
			auto y = movePos.at("y").asInt();
			auto move = MoveTo::create(0.03 * getDistance(tempGround[i]->getPos().x, tempGround[i]->getPos().y, x, y), Vec2(x, y));
			auto rotate = RotateTo::create(0, getCorner(tempGround[i]->getPos().x, tempGround[i]->getPos().y, x, y));
			auto unit = tempGround[i];

			auto callback = CallFunc::create(CC_CALLBACK_0(GamePlayScene::actionUnitFinished, this, unit));
			auto sequence = Sequence::create(rotate, move, callback, NULL);
			tempGround[i]->runAct(sequence);
			tempGround[i]->setLocationPoint(tempGround[i]->getLocationPoint() + 1);
		}
		if (tempGround[i]->getLocationPoint() == locationPoint.size())
		{
			lives--;
			Unit *unit5 = tempGround[i];
			tempGround.erase(tempGround.begin() + i);
			unit5->setLocationPoint(0);
			unit5->die();
			level->listWave[wave]->decTotalUnit();
		}
	}
	for (int i = 0; i < tempSky.size(); i++)
	{
		if ((tempSky[i]->getVisible()) && (tempSky[i]->getFreeFlag()))
		{
			tempSky[i]->setFreeFlag(false);
			auto movePos = locationPointSky[tempSky[i]->getLocationPoint()];
			auto x = movePos.at("x").asInt();
			auto y = movePos.at("y").asInt();
			auto move = MoveBy::create(0.02 * getDistance(tempSky[i]->getPos().x, tempSky[i]->getPos().y, x, y), Vec2(x - tempSky[i]->getPos().x, y - tempSky[i]->getPos().y));
			auto rotate = RotateTo::create(0, getCorner(tempSky[i]->getPos().x, tempSky[i]->getPos().y, x, y));
			auto unit = tempSky[i];

			auto callback = CallFunc::create(CC_CALLBACK_0(GamePlayScene::actionUnitFinished, this, unit));
			auto sequence = Sequence::create(rotate, move, callback, NULL);
			tempSky[i]->runAct(sequence);
			tempSky[i]->setLocationPoint(tempSky[i]->getLocationPoint() + 1);
		}
		if (tempSky[i]->getLocationPoint() == locationPointSky.size())
		{
			lives--;
			Unit *unit5 = tempSky[i];
			unit5->setLocationPoint(0);
			tempSky.erase(tempSky.begin() + i);
			unit5->setLocationPoint(0);
			unit5->die();
			level->listWave[wave]->decTotalUnit();
		}

	}
}

void GamePlayScene::update(float dt){
	if (myHero->getHp() <= 0) myHero->die();

	if (skillHeroDelay > 0) skillHeroDelay -= dt;
	if (skillGameDelay > 0) skillGameDelay -= dt;

	if (myHero->getStatus()){
		heroController();
		for (int i = 0; i < towerList.size(); i++){
			if (towerList[i]->getBox().intersectsRect(myHero->getSprite()->getBoundingBox()) && (towerList[i]->getTowerTimeBuild() != 0)){
				towerList[i]->updateTowerTimeBuild(dt);
			}
		}
		heroDetectEnemy(dt);
	}

	if (level->listWave[wave]->getTotalUnit() == 0 && (wave + 1 < level->numOfWave)){
		wave++;
		tempGround = level->listWave[wave]->createListUnitGround(this, Vec2(locationPoint[0].at("x").asInt(), locationPoint[0].at("y").asInt()));
		tempSky = level->listWave[wave]->createListUnitSky(this, Vec2(locationPointSky[0].at("x").asInt(), locationPointSky[0].at("y").asInt()));
	}


	towerDetectEnemy(dt);
	updateUnit(dt);
	updateUI();

	if (lives <= 0)
	{
		createMissionFail();
	}
	if (lives > 0 && level->listWave[wave]->getTotalUnit() == 0 && wave == level->numOfWave - 1)
	{
		createMissionSuccess();
	}
}

void GamePlayScene::heroController(){
	if ((UILayer->getJoytick()->getVelocity().x != 0) || (UILayer->getJoytick()->getVelocity().y != 0))
	{
		myHero->getSprite()->getPhysicsBody()->setVelocity(Vec2(myHero->getMoveSpeed() * UILayer->getJoytick()->getVelocity().x, myHero->getMoveSpeed() * UILayer->getJoytick()->getVelocity().y));
		if (myHero->getFreeFlag()){
			myHero->rotate(UILayer->getJoytick()->getVelocity());
		}
	}
	else {
		myHero->getSprite()->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}
}

float GamePlayScene::getCorner(float xa, float ya, float xb, float yb)
{
	float dx = xb - xa;
	float dy = yb - ya;
	float val = 180.0 / M_PI;
	float conrner = dy / dx;
	//CCLOG("%d,%f,%f", i++,conrner, atan(conrner)*val);
	//if(yb>ya)
	return -atan(conrner)*val;
}

float GamePlayScene::getDistance(float xa, float ya, float xb, float yb)
{
	float dx = xb - xa;
	float dy = yb - ya;
	return sqrt(dx*dx + dy * dy);
}

void GamePlayScene::actionUnitFinished(Unit* unit)
{
	unit->setFreeFlag(true);
	//unit->getSprite()->stopAllActions();
}

void GamePlayScene::towerDetectEnemy(float dt){
	for (int i = 0; i < towerList.size(); i++){
		if (towerList[i]->getTowerTimeBuild() <= 0){
			towerList[i]->updateTimeWait(dt);
			if (towerList[i]->checkTimeWait()){
				if (towerList[i]->getTowerType() == 1 || towerList[i]->getTowerType() == 3){
					for (int j = 0; j< tempGround.size(); j++){
						Unit *unit = tempGround[j];
						float distance = getDistance(unit->getPos().x, unit->getPos().y, towerList[i]->getPos().x, towerList[i]->getPos().y);

						if (distance < towerList[i]->getTowerRangeAttack() && unit->getStatus()){
							towerList[i]->shoot(unit->getPos().x, unit->getPos().y);
							unit->setLDBVisible(true);
							unit->setHp(unit->getHp() - towerList[i]->getTowerDmg());
							if (unit->getHp() <= 0){
								(tempGround).erase(tempGround.begin() + j);
								unit->updatePosHB();
								unit->die();
								level->listWave[wave]->decTotalUnit();
								gold += unit->getGold();
							}
						/*	if (towerList[i]->getTowerType() == 3){
								if (j > 1 && tempGround[j - 1]->getStatus()) {
									tempGround[j - 1]->setLDBVisible(true);
									tempGround[j - 1]->setHp(tempGround[j - 1]->getHp() - towerList[i]->getTowerDmg() / 2);
									if (tempGround[j - 1]->getHp() <= 0){
										Unit *unit1 = tempGround[j - 1];
										(tempGround).erase(tempGround.begin() + j - 1);
										unit1->die();
										level->listWave[wave]->decTotalUnit();
										gold += unit1->getGold();
									}
								}
								if (j < tempGround.size() - 1 && tempGround[j + 1]->getStatus()) {
									tempGround[j + 1]->setLDBVisible(true);
									tempGround[j + 1]->setHp(tempGround[j + 1]->getHp() - towerList[i]->getTowerDmg() / 2);
									if (tempGround[j + 1]->getHp() <= 0){
										Unit *unit2 = tempGround[j + 1];
										(tempGround).erase(tempGround.begin() + j + 1);
										unit2->die();
										level->listWave[wave]->decTotalUnit();
										gold += unit2->getGold();
									}
								}
							}*/
							break;

						}
					}
				}
				if (towerList[i]->getTowerType() == 2){
					for (int j = 0; j< tempSky.size(); j++){
						Unit *unit3 = tempSky[j];
						float distance = getDistance(unit3->getPos().x, unit3->getPos().y, towerList[i]->getPos().x, towerList[i]->getPos().y);

						if (distance < towerList[i]->getTowerRangeAttack() && unit3->getStatus()){
							towerList[i]->shoot(unit3->getPositionX(), unit3->getPositionY());
							unit3->setLDBVisible(true);
							unit3->setHp(unit3->getHp() - towerList[i]->getTowerDmg());
							if (unit3->getHp() <= 0){
								tempSky.erase(tempSky.begin() + j);
								unit3->updatePosHB();
								unit3->die();
								level->listWave[wave]->decTotalUnit();
								gold += unit3->getGold();
							}
							break;
						}
					}
				}
			}
		}
	}
}

void GamePlayScene::heroDetectEnemy(float dt){
	myHero->updateTimeWait(dt);
	if (myHero->checkTimeWait()){
		for (int i = 0; i < tempGround.size(); i++){
			if (tempGround[i]->getVisible() && tempGround[i]->getStatus()){
				float distance = getDistance(tempGround[i]->getPositionX(), tempGround[i]->getPositionY(), myHero->getPositionX(), myHero->getPositionY());
				if (myHero->getWeapon()->getRangeAttack() > distance) {
					myHero->setFreeFlag(false);
					myHero->shoot(tempGround[i]->getPositionX(), tempGround[i]->getPositionY());
					tempGround[i]->setLDBVisible(true);
					tempGround[i]->setHp(tempGround[i]->getHp() - myHero->getWeapon()->getDmg());
					if (tempGround[i]->getHp() <= 0){
						Unit *unit4 = tempGround[i];
						tempGround.erase(tempGround.begin() + i);
						unit4->updatePosHB();
						unit4->die();
						level->listWave[wave]->decTotalUnit();
						gold += unit4->getGold();
					}
					break;
				}
				else myHero->setFreeFlag(true);
			}
		}
	}
}




void GamePlayScene::createMissionSuccess()
{
	
	

	//Director::getInstance()->pause();
	this->unscheduleUpdate();

	auto backGround = Sprite::create("mission-bg.png");
	backGround->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
	backGround->setScale(0.7, 1.2);
	MissionSuccessLayer->addChild(backGround);

	auto lableSuccess = Label::create("Mission Success !!!! ", "fonts/kenvector_future.ttf ", 40);
	lableSuccess->setPosition(Vec2(backGround->getPosition().x, backGround->getPosition().y + backGround->getContentSize().height / 3));
	lableSuccess->setAnchorPoint(Vec2(0.5, 0));
	MissionSuccessLayer->addChild(lableSuccess);


	auto nextBtn = ui::Button::create("UI/nextlever-button.png","UI/nextlever2-button.png");
	nextBtn->setPosition(Vec2(lableSuccess->getPosition().x, lableSuccess->getPosition().y - 1.5 * nextBtn->getContentSize().height));
	MissionSuccessLayer->addChild(nextBtn);

	auto replaceBtn = ui::Button::create("UI/replay-button.png","UI/replay2-button.png");
	replaceBtn->setPosition(Vec2(lableSuccess->getPosition().x, lableSuccess->getPosition().y - 3 * nextBtn->getContentSize().height));
	MissionSuccessLayer->addChild(replaceBtn);
	replaceBtn->addClickEventListener([&](Ref* event) {
		//Director::getInstance()->resume();
		Director::getInstance()->replaceScene(GamePlayScene::createScene(level->numoflevel));
	});

	auto missionBtn = ui::Button::create("UI/mission-button.png", "UI/mission2-button.png");
	missionBtn->setPosition(Vec2(lableSuccess->getPosition().x, lableSuccess->getPosition().y - 4.5 * nextBtn->getContentSize().height));
	MissionSuccessLayer->addChild(missionBtn);
	missionBtn->addClickEventListener([&](Ref* event) {
		//Director::getInstance()->resume();
		Director::getInstance()->replaceScene(MissionScene::create());
	});

	auto quitBtn = ui::Button::create("UI/quit-button.png","UI/quit2-button.png");
	quitBtn->setPosition(Vec2(lableSuccess->getPosition().x, lableSuccess->getPosition().y - 6 * nextBtn->getContentSize().height));
	quitBtn->addClickEventListener([&](Ref* event) {
		//Director::getInstance()->resume();
		Director::getInstance()->replaceScene(MainMenuScene::create());
	});
	MissionSuccessLayer->addChild(quitBtn);
	//Director::getInstance()->resume();
}

void GamePlayScene::createMissionFail()
{
	

	//Director::getInstance()->pause();
	this->unscheduleUpdate();
	stopAllMove();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto backGround = Sprite::create("mission-bg.png");
	backGround->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	backGround->setScale(0.7, 1.2);
	MissionFailLayer->addChild(backGround);

	auto lableFail = Label::create("Mission Fail !!!!! ", "fonts/kenvector_future.ttf ", 40);
	lableFail->setPosition(Vec2(backGround->getPosition().x, backGround->getPosition().y + backGround->getContentSize().height / 3));
	lableFail->setAnchorPoint(Vec2(0.5, 0));
	MissionFailLayer->addChild(lableFail);

	auto replaceBtn = ui::Button::create("UI/replay-button.png","UI/replay2-button.png");
	replaceBtn->setPosition(Vec2(lableFail->getPosition().x, lableFail->getPosition().y - 1.5 * replaceBtn->getContentSize().height));
	MissionFailLayer->addChild(replaceBtn);
	replaceBtn->addClickEventListener([&](Ref* event) {
		//Director::getInstance()->resume();
		Director::getInstance()->replaceScene(GamePlayScene::createScene(level->numoflevel));
	});
	auto missionBtn = ui::Button::create("UI/mission-button.png","UI/mission2-button.png");
	missionBtn->setPosition(Vec2(lableFail->getPosition().x, lableFail->getPosition().y - 3 * replaceBtn->getContentSize().height));
	MissionFailLayer->addChild(missionBtn);
	missionBtn->addClickEventListener([&](Ref* event) {
		//Director::getInstance()->resume();
		Director::getInstance()->replaceScene(MissionScene::create());
	});
	auto quitBtn = ui::Button::create("UI/quit-button.png","UI/quit2-button.png");
	quitBtn->setPosition(Vec2(lableFail->getPosition().x, lableFail->getPosition().y - 4.5 * replaceBtn->getContentSize().height));
	quitBtn->addClickEventListener([&](Ref* event) {
		//Director::getInstance()->resume();
		Director::getInstance()->replaceScene(MainMenuScene::create());
	});
	MissionFailLayer->addChild(quitBtn);
	//Director::getInstance()->resume();
}

void GamePlayScene::stopAllMove(){
	for (int i = 0; i < tempGround.size();i++){
		tempGround[i]->stop();
	}
	for (int i = 0; i < tempSky.size(); i++){
		tempSky[i]->stop();
	}
}