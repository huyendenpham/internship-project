#include "UpgradeScene.h"
#include "MainMenuScene.h"
#include "GameData.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"

using namespace rapidjson;

ui::Button *rank1Gun, *rank2Gun, *rank3Gun, *rank4Gun, *rank5Gun,
			*rank1Mi, *rank2Mi, *rank3Mi, *rank4Mi, *rank5Mi, 
			*rank1At, *rank2At, *rank3At, *rank4At, *rank5At,
			*priceBtn;
Label * BP, *infoLabel, *priceLabel;
int price;


Scene* UpgradeScene::createScene(){
	return UpgradeScene::create();
}

bool UpgradeScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto background = Sprite::create("background.png");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	background->setOpacity(100);
	this->addChild(background);

	createTowerLayer();
	createHeroLayer();

	return true;
}

void UpgradeScene::createTowerLayer(){
	TowerLayer = Layer::create();
	this->addChild(TowerLayer);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto towerBackground = Sprite::create("upgrade_rank/towerBg.png");
	towerBackground->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	auto towerHero = ui::Button::create("upgrade_rank/tower_hero.png", "upgrade_rank/tower_hero.png");
	towerHero->setAnchorPoint(Vec2(0, 0));
	towerHero->setPosition(Vec2(towerBackground->getPositionX() - 216, towerBackground->getPositionY() + 203));
	towerHero->addClickEventListener([&](Ref* event)
	{
		TowerLayer->setVisible(false);
		HeroLayer->setVisible(true);
	});
	TowerLayer->addChild(towerHero);
	TowerLayer->addChild(towerBackground);

	auto infoBg = Sprite::create("upgrade_rank/InfoBg.png");
	infoBg->setAnchorPoint(Vec2(1, 1));
	infoBg->setPosition(towerBackground->getPositionX() + 378, towerBackground->getPositionY() + 185);
	TowerLayer->addChild(infoBg);

	infoLabel = Label::create("Machine Gun\nAnti Ground\n\nA.speed: 1\nA.Damage: 10\nRange: 150", "fonts/kenvector_future.ttf", 18);
	infoLabel->setAnchorPoint(Vec2(0, 1));
	infoLabel->setPosition(infoBg->getPositionX() - infoBg->getContentSize().width + 10, infoBg->getPositionY() - 50);
	TowerLayer->addChild(infoLabel);
	

	priceBtn = ui::Button::create("upgrade_rank/priceBtn.png", "upgrade_rank/priceBtn1.png");
	priceBtn->setPosition(Vec2(infoBg->getPositionX()-infoBg->getContentSize().width/2, infoBg->getPositionY() - infoBg->getContentSize().height + 20 + priceBtn->getContentSize().height));
	priceBtn->setScale(0.8, 1);
	TowerLayer->addChild(priceBtn);
	

	priceLabel = Label::create("", "fonts/kenvector_future.ttf", 20);
	priceLabel->setPosition(priceBtn->getPosition());
	TowerLayer->addChild(priceLabel);

	priceBtn->setVisible(false);
	priceLabel->setVisible(false);
	updateInfo(1, 0);

	auto btnBack = ui::Button::create("upgrade_rank/back.png");
	btnBack->setAnchorPoint(Vec2(1, 1));
	btnBack->setPosition(Vec2(infoBg->getPositionX(), infoBg->getPositionY()-357));
	btnBack->addClickEventListener([&](Ref* event)
	{
		Director::getInstance()->replaceScene(MainMenuScene::create());
	});
	TowerLayer->addChild(btnBack);

	auto BPBackground = Sprite::create("upgrade_rank/bp_amount_bg.png");
	BPBackground->setPosition(btnBack->getPositionX() - 211 - BPBackground->getContentSize().width / 2, btnBack->getPositionY() - BPBackground->getContentSize().height/2);
	TowerLayer->addChild(BPBackground);

	BP = Label::create(StringUtils::toString(GameData::getInstance()->getBP()), "fonts/kenvector_future.ttf", 20);
	BP->setPosition(BPBackground->getPosition());
	TowerLayer->addChild(BP);

	auto freeBtn = ui::Button::create("upgrade_rank/free.png");
	freeBtn->setPosition(Vec2(BPBackground->getPositionX()-134,BPBackground->getPositionY()-1));
	TowerLayer->addChild(freeBtn);

	auto buyBtn = ui::Button::create("upgrade_rank/buy.png");
	buyBtn->setPosition(Vec2(freeBtn->getPositionX() - 146, freeBtn->getPositionY()));
	TowerLayer->addChild(buyBtn);

	createGun();
	update(1);
	createMissile();
	createCannon();

	auto miBtn = ui::Button::create("upgrade_rank/mi.png");
	miBtn->setPosition(Vec2(towerBackground->getPositionX() - 305, towerBackground->getPositionY() + 13));
	miBtn->addClickEventListener([&](Ref* event)
	{
		GunLayer->setVisible(false);
		MissileLayer->setVisible(true);
		CannonLayer->setVisible(false);
		update(2);
		updateInfo(2, 0);
	});
	TowerLayer->addChild(miBtn, 2);


	auto gunBtn = ui::Button::create("upgrade_rank/gun.png");
	gunBtn->setPosition(Vec2(miBtn->getPositionX(), miBtn->getPositionY() + 115));
	gunBtn->addClickEventListener([&](Ref* event)
	{
		GunLayer->setVisible(true);
		MissileLayer->setVisible(false);
		CannonLayer->setVisible(false);
		update(1);
		updateInfo(1, 0);
	});
	TowerLayer->addChild(gunBtn, 2);

	auto atBtn = ui::Button::create("upgrade_rank/at.png");
	atBtn->setPosition(Vec2(miBtn->getPositionX(), miBtn->getPositionY() - 115));
	atBtn->addClickEventListener([&](Ref* event)
	{
		GunLayer->setVisible(false);
		MissileLayer->setVisible(false);
		CannonLayer->setVisible(true);
		update(3);
		updateInfo(3, 0);
	});
	TowerLayer->addChild(atBtn, 2);


}

void UpgradeScene::createHeroLayer(){
	HeroLayer = Layer::create();
	this->addChild(HeroLayer);
	HeroLayer->setVisible(false);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto heroBackground = Sprite::create("upgrade_rank/heroBg.png");
	heroBackground->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	auto heroTower = ui::Button::create("upgrade_rank/hero_tower.png", "upgrade_rank/hero_tower.png");
	heroTower->setAnchorPoint(Vec2(0, 0));
	heroTower->setPosition(Vec2(heroBackground->getPositionX() - 344, heroBackground->getPositionY() + 203));
	heroTower->addClickEventListener([&](Ref* event)
	{
		TowerLayer->setVisible(true);
		HeroLayer->setVisible(false);
	});
	HeroLayer->addChild(heroTower);
	HeroLayer->addChild(heroBackground);

	auto notAvailable = Sprite::create("upgrade_rank/not_available.png");
	notAvailable->setPosition(heroBackground->getPosition());
	HeroLayer->addChild(notAvailable);
}

void UpgradeScene::createGun(){
	GunLayer = Layer::create();

	TowerLayer->addChild(GunLayer, 1);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto techBg = Sprite::create("upgrade_rank/TechBg1.png");
	techBg->setPosition(visibleSize.width / 2 + 167 - techBg->getContentSize().width / 2, visibleSize.height / 2 + 185 - techBg->getContentSize().height / 2);
	GunLayer->addChild(techBg);

	rank1Gun = ui::Button::create("upgrade_rank/rank-1.png");
	rank1Gun->setPosition(Vec2(techBg->getPositionX() - 70, techBg->getPositionY() + 85));
	rank1Gun->addClickEventListener([&](Ref* event)
	{
		updateInfo(1, 1);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(1, 1, price);
		});

		int rank = GameData::getInstance()->getGunNormal();
		if (rank >= 1) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else priceBtn->setEnabled(true);
	});
	GunLayer->addChild(rank1Gun, 1);

	rank2Gun = ui::Button::create("upgrade_rank/rank-2.png");
	rank2Gun->setPosition(Vec2(rank1Gun->getPositionX() + 133, rank1Gun->getPositionY()));
	rank2Gun->addClickEventListener([&](Ref* event)
	{
		updateInfo(1, 2);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(1, 2, price);
		});
		int rank = GameData::getInstance()->getGunNormal();
		if (rank >= 2) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		} else
		if (rank + 1 < 2){
			priceBtn->setEnabled(false);
		}
		else priceBtn->setEnabled(true);
	});
	GunLayer->addChild(rank2Gun, 1);

	rank3Gun = ui::Button::create("upgrade_rank/rank-3.png");
	rank3Gun->setPosition(Vec2(rank2Gun->getPositionX() + 133, rank2Gun->getPositionY()));
	rank3Gun->addClickEventListener([&](Ref* event)
	{
		updateInfo(1, 3);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(1, 3, price);
		});
		int rank = GameData::getInstance()->getGunNormal();
		if (rank >= 3) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		} else
		if (rank + 1 < 3){
			priceBtn->setEnabled(false);
		}
		else priceBtn->setEnabled(true);
	});
	GunLayer->addChild(rank3Gun, 1);

	rank4Gun = ui::Button::create("upgrade_rank/rank-gun-1.png");
	rank4Gun->setPosition(Vec2(rank2Gun->getPositionX() - 66, rank2Gun->getPositionY() - 170));
	rank4Gun->addClickEventListener([&](Ref* event)
	{
		updateInfo(1, 4);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(1, 4, price);
		});
		int rank = GameData::getInstance()->getGunNormal();
		int rank1 = GameData::getInstance()->getGunOption1();
		if (rank1 == 1) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		} else
		if (rank <3){
			priceBtn->setEnabled(false);
		}
		else priceBtn->setEnabled(true);
	});
	GunLayer->addChild(rank4Gun, 1);

	rank5Gun = ui::Button::create("upgrade_rank/rank-gun-2.png");
	rank5Gun->setPosition(Vec2(rank4Gun->getPositionX() + 133, rank4Gun->getPositionY()));
	rank5Gun->addClickEventListener([&](Ref* event)
	{
		updateInfo(1, 5);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(1, 5, price);
		});
		int rank = GameData::getInstance()->getGunNormal();
		int rank1 = GameData::getInstance()->getGunOption2();
		if (rank1 == 1) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		} else
		if (rank <3){
			priceBtn->setEnabled(false);
		}
		else priceBtn->setEnabled(true);
	});
	GunLayer->addChild(rank5Gun, 1);

	auto line1 = Sprite::create("upgrade_rank/Line0.png");
	line1->setPosition(rank1Gun->getPositionX() - line1->getContentSize().width-7, rank1Gun->getPositionY()+16);
	GunLayer->addChild(line1);

	auto line2 = Sprite::create("upgrade_rank/Line2.png");
	line2->setPosition(rank2Gun->getPositionX() - line2->getContentSize().width - 1, rank2Gun->getPositionY() + 8);
	GunLayer->addChild(line2);

	auto line3 = Sprite::create("upgrade_rank/Line2.png");
	line3->setPosition(rank3Gun->getPositionX() - line2->getContentSize().width - 1, rank3Gun->getPositionY() + 8);
	GunLayer->addChild(line3);

	auto line4 = Sprite::create("upgrade_rank/Line4.png");
	line4->setPosition(rank3Gun->getPositionX() - line4->getContentSize().width / 2 + 4, rank3Gun->getPositionY() - line4->getContentSize().height / 2 - rank3Gun->getContentSize().height / 2 + 8);
	GunLayer->addChild(line4);

}

void UpgradeScene::createMissile(){
	MissileLayer = Layer::create();
	TowerLayer->addChild(MissileLayer,1);
	MissileLayer->setVisible(false);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto techBg = Sprite::create("upgrade_rank/TechBg2.png");
	techBg->setPosition(visibleSize.width/2 + 167 - techBg->getContentSize().width/2, visibleSize.height/2 + 185 - techBg->getContentSize().height/2);
	MissileLayer->addChild(techBg);

	rank1Mi = ui::Button::create("upgrade_rank/rank-1.png");
	rank1Mi->setPosition(Vec2(techBg->getPositionX() - 70, techBg->getPositionY() + 85));
	rank1Mi->addClickEventListener([&](Ref* event)
	{
		updateInfo(2, 1);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(2, 1, price);
		});
		int rank = GameData::getInstance()->getMissileNormal();
		if (rank >= 1) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else priceBtn->setEnabled(true);
	});
	MissileLayer->addChild(rank1Mi, 1);

	rank2Mi = ui::Button::create("upgrade_rank/rank-2.png");
	rank2Mi->setPosition(Vec2(rank1Mi->getPositionX() + 133, rank1Mi->getPositionY()));
	rank2Mi->addClickEventListener([&](Ref* event)
	{
		updateInfo(2, 2);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(2, 2, price);
		});
		int rank = GameData::getInstance()->getMissileNormal();
		if (rank >= 2) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else
			if (rank + 1 < 2){
				priceBtn->setEnabled(false);
			}
			else priceBtn->setEnabled(true);
	});
	MissileLayer->addChild(rank2Mi, 1);

	rank3Mi = ui::Button::create("upgrade_rank/rank-3.png");
	rank3Mi->setPosition(Vec2(rank2Mi->getPositionX() + 133, rank2Mi->getPositionY()));
	rank3Mi->addClickEventListener([&](Ref* event)
	{
		updateInfo(2, 3);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(2, 3, price);
		});
		int rank = GameData::getInstance()->getMissileNormal();
		if (rank >= 3) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else
			if (rank + 1 < 3){
				priceBtn->setEnabled(false);
			}
			else priceBtn->setEnabled(true);
	});
	MissileLayer->addChild(rank3Mi, 1);

	rank4Mi = ui::Button::create("upgrade_rank/rank-missile-1.png");
	rank4Mi->setPosition(Vec2(rank2Mi->getPositionX() - 66, rank2Mi->getPositionY() - 170));
	rank4Mi->addClickEventListener([&](Ref* event)
	{
		updateInfo(2, 4);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(2, 4, price);
		});
		int rank = GameData::getInstance()->getMissileNormal();
		int rank1 = GameData::getInstance()->getMissileOption1();
		if (rank1 == 1) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else
			if (rank <3){
				priceBtn->setEnabled(false);
			}
			else priceBtn->setEnabled(true);
	});
	MissileLayer->addChild(rank4Mi, 1);

	rank5Mi = ui::Button::create("upgrade_rank/rank-missile-2.png");
	rank5Mi->setPosition(Vec2(rank4Mi->getPositionX() + 133, rank4Mi->getPositionY()));
	rank5Mi->addClickEventListener([&](Ref* event)
	{
		updateInfo(2, 5);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(2, 5, price);
		});
		int rank = GameData::getInstance()->getMissileNormal();
		int rank1 = GameData::getInstance()->getMissileOption2();
		if (rank1 == 1) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else
			if (rank <3){
				priceBtn->setEnabled(false);
			}
			else priceBtn->setEnabled(true);
	});
	MissileLayer->addChild(rank5Mi, 1);

	auto line1 = Sprite::create("upgrade_rank/Line1.png");
	line1->setPosition(rank1Mi->getPositionX() - line1->getContentSize().width / 2 + 9, rank1Mi->getPositionY() - rank1Mi->getContentSize().height / 2 - 20);
	MissileLayer->addChild(line1);

	auto line2 = Sprite::create("upgrade_rank/Line2.png");
	line2->setPosition(rank2Mi->getPositionX() - line2->getContentSize().width - 1, rank2Mi->getPositionY() + 8);
	MissileLayer->addChild(line2);

	auto line3 = Sprite::create("upgrade_rank/Line2.png");
	line3->setPosition(rank3Mi->getPositionX() - line2->getContentSize().width - 1, rank3Mi->getPositionY() + 8);
	MissileLayer->addChild(line3);

	auto line4 = Sprite::create("upgrade_rank/Line4.png");
	line4->setPosition(rank3Mi->getPositionX() - line4->getContentSize().width / 2 + 4, rank3Mi->getPositionY() - line4->getContentSize().height / 2 - rank3Mi->getContentSize().height / 2 + 8);
	MissileLayer->addChild(line4);

}

void UpgradeScene::createCannon(){
	CannonLayer = Layer::create();
	TowerLayer->addChild(CannonLayer, 1);
	CannonLayer->setVisible(false);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto techBg = Sprite::create("upgrade_rank/TechBg3.png");
	techBg->setPosition(visibleSize.width / 2 + 167 - techBg->getContentSize().width / 2, visibleSize.height / 2 + 185 - techBg->getContentSize().height / 2);
	CannonLayer->addChild(techBg);


	rank1At = ui::Button::create("upgrade_rank/rank-1.png");
	rank1At->setPosition(Vec2(techBg->getPositionX() - 70, techBg->getPositionY() + 85));
	rank1At->addClickEventListener([&](Ref* event)
	{
		updateInfo(3, 1);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(3, 1, price);
		});
		int rank = GameData::getInstance()->getAtNormal();
		if (rank >= 1) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else priceBtn->setEnabled(true);
	});
	CannonLayer->addChild(rank1At, 1);

	rank2At = ui::Button::create("upgrade_rank/rank-2.png");
	rank2At->setPosition(Vec2(rank1At->getPositionX() + 133, rank1At->getPositionY()));
	rank2At->addClickEventListener([&](Ref* event)
	{
		updateInfo(3, 2);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(3, 2, price);
		});
		int rank = GameData::getInstance()->getAtNormal();
		if (rank >= 2) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else
			if (rank + 1 < 2){
				priceBtn->setEnabled(false);
			}
			else priceBtn->setEnabled(true);
	});
	CannonLayer->addChild(rank2At, 1);

	rank3At = ui::Button::create("upgrade_rank/rank-3.png");
	rank3At->setPosition(Vec2(rank2At->getPositionX() + 133, rank2At->getPositionY()));
	rank3At->addClickEventListener([&](Ref* event)
	{
		updateInfo(3, 3);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(3, 3, price);
		});
		int rank = GameData::getInstance()->getAtNormal();
		if (rank >= 3) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else
			if (rank + 1 < 3){
				priceBtn->setEnabled(false);
			}
			else priceBtn->setEnabled(true);
	});
	CannonLayer->addChild(rank3At, 1);

	rank4At = ui::Button::create("upgrade_rank/rank-cannon-1.png");
	rank4At->setPosition(Vec2(rank2At->getPositionX() - 66, rank2At->getPositionY() - 170));
	rank4At->addClickEventListener([&](Ref* event)
	{
		updateInfo(3, 4);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(3, 4, price);
		});
		int rank = GameData::getInstance()->getAtNormal();
		int rank1 = GameData::getInstance()->getAtOption1();
		if (rank1 == 1) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else
			if (rank <3){
				priceBtn->setEnabled(false);
			}
			else priceBtn->setEnabled(true);
	});
	CannonLayer->addChild(rank4At, 1);

	rank5At = ui::Button::create("upgrade_rank/rank-cannon-2.png");
	rank5At->setPosition(Vec2(rank4At->getPositionX() + 133, rank4At->getPositionY()));
	rank5At->addClickEventListener([&](Ref* event)
	{
		updateInfo(3, 5);
		priceBtn->addClickEventListener([&](Ref* event)
		{
			buyTech(3, 5, price);
		});
		int rank = GameData::getInstance()->getAtNormal();
		int rank1 = GameData::getInstance()->getAtOption2();
		if (rank1 == 1) {
			priceBtn->setEnabled(false);
			priceBtn->setVisible(false);
			priceLabel->setVisible(false);
		}
		else
			if (rank <3){
				priceBtn->setEnabled(false);
			}
			else priceBtn->setEnabled(true);
	});
	CannonLayer->addChild(rank5At, 1);

	auto line1 = Sprite::create("upgrade_rank/Line5.png");
	line1->setPosition(rank1At->getPositionX() - line1->getContentSize().width / 2 + 9, rank1At->getPositionY() - rank1At->getContentSize().height / 2 - 78);
	CannonLayer->addChild(line1);

	auto line2 = Sprite::create("upgrade_rank/Line2.png");
	line2->setPosition(rank2At->getPositionX() - line2->getContentSize().width - 1, rank2At->getPositionY() + 8);
	CannonLayer->addChild(line2);

	auto line3 = Sprite::create("upgrade_rank/Line2.png");
	line3->setPosition(rank3At->getPositionX() - line2->getContentSize().width - 1, rank3At->getPositionY() + 8);
	CannonLayer->addChild(line3);

	auto line4 = Sprite::create("upgrade_rank/Line4.png");
	line4->setPosition(rank3At->getPositionX() - line4->getContentSize().width / 2 + 4, rank3At->getPositionY() - line4->getContentSize().height / 2 - rank3At->getContentSize().height / 2 + 8);
	CannonLayer->addChild(line4);
}

void UpgradeScene::updateInfo(int type, int rank){
	Document read_Json_Tower;
	ssize_t size3;


	char* buf3 = (char*)FileUtils::getInstance()->getFileData("data/dataTower.json", "r", &size3);
	string content3(buf3, size3);

	string towerCode = StringUtils::toString(type * 10 + rank);

	read_Json_Tower.Parse(content3.c_str());

	string info = "";
	if (type == 1){
		info += "Machine Gun\nAnti Ground\n";
	}
	if (type == 2){
		info += "Missile\nAnti Air\n";
	}
	if (type == 3){
		info += "AT Cannon\nAnti Ground\n";
	}
	if (rank != 0 && rank <= 3) {
		info += "Lv.";
		info += StringUtils::toString(rank);
		info += "\n";
	}
	else if (rank == 4){
		info += "Option 1\n";
	}
	else if (rank == 5){
		info += "Option 2\n";
	}

	info += "\nA.Speed: ";
	info += StringUtils::toString(read_Json_Tower[towerCode.c_str()]["speed"].GetFloat());
	info += "\nA.Damage: ";
	info += StringUtils::toString(read_Json_Tower[towerCode.c_str()]["damage"].GetInt());
	info += "\nA.Range: ";
	info += StringUtils::toString(read_Json_Tower[towerCode.c_str()]["range"].GetInt());
	infoLabel->setString(info);
	if (rank != 0) {
		price = read_Json_Tower[towerCode.c_str()]["BP"].GetInt();
		priceBtn->setVisible(true);
		priceLabel->setVisible(true);
		priceLabel->setString(StringUtils::toString(price) + " BP");
	}
	else {
		priceBtn->setVisible(false);
		priceLabel->setVisible(false);
	}


}

void UpgradeScene::update(int type){
	switch (type){
	case 1:
		if (GameData::getInstance()->getGunNormal() == 0){
			rank1Gun->setOpacity(100);
			rank2Gun->setOpacity(100);
			rank3Gun->setOpacity(100);
			rank4Gun->setOpacity(100);
			rank5Gun->setOpacity(100);
		}
		if (GameData::getInstance()->getGunNormal() == 1){
			rank1Gun->setOpacity(248);
			rank2Gun->setOpacity(100);
			rank3Gun->setOpacity(100);
			rank4Gun->setOpacity(100);
			rank5Gun->setOpacity(100);
		}
		if (GameData::getInstance()->getGunNormal() == 2){
			rank1Gun->setOpacity(248);
			rank2Gun->setOpacity(248);
			rank3Gun->setOpacity(100);
			rank4Gun->setOpacity(100);
			rank5Gun->setOpacity(100);
		}
		if (GameData::getInstance()->getGunNormal() == 3){
			rank1Gun->setOpacity(248);
			rank2Gun->setOpacity(248);
			rank3Gun->setOpacity(248);
			rank4Gun->setOpacity(248);
			rank5Gun->setOpacity(248);
		}
		if (GameData::getInstance()->getGunOption1() == 0){
			rank4Gun->setOpacity(100);
		}
		if (GameData::getInstance()->getGunOption2() == 0){
			rank5Gun->setOpacity(100);
		}
		return;
		break;
	case 2:
		if (GameData::getInstance()->getMissileNormal() == 0){
			rank1Mi->setOpacity(100);
			rank2Mi->setOpacity(100);
			rank3Mi->setOpacity(100);
			rank4Mi->setOpacity(100);
			rank5Mi->setOpacity(100);
		}
		if (GameData::getInstance()->getMissileNormal() == 1){
			rank1Mi->setOpacity(248);
			rank2Mi->setOpacity(100);
			rank3Mi->setOpacity(100);
			rank4Mi->setOpacity(100);
			rank5Mi->setOpacity(100);
		}
		if (GameData::getInstance()->getMissileNormal() == 2){
			rank1Mi->setOpacity(248);
			rank2Mi->setOpacity(248);
			rank3Mi->setOpacity(100);
			rank4Mi->setOpacity(100);
			rank5Mi->setOpacity(100);
		}
		if (GameData::getInstance()->getMissileNormal() == 3){
			rank1Mi->setOpacity(248);
			rank2Mi->setOpacity(248);
			rank3Mi->setOpacity(248);
			rank4Mi->setOpacity(248);
			rank5Mi->setOpacity(248);
		}
		if (GameData::getInstance()->getMissileOption1() == 0){
			rank4Mi->setOpacity(100);
		}
		if (GameData::getInstance()->getMissileOption2() == 0){
			rank5Mi->setOpacity(100);
		}
		return;
		break;
	case 3:
		if (GameData::getInstance()->getAtNormal() == 0){
			rank1At->setOpacity(100);
			rank2At->setOpacity(100);
			rank3At->setOpacity(100);
			rank4At->setOpacity(100);
			rank5At->setOpacity(100);
		}
		if (GameData::getInstance()->getAtNormal() == 1){
			rank1At->setOpacity(248);
			rank2At->setOpacity(100);
			rank3At->setOpacity(100);
			rank4At->setOpacity(100);
			rank5At->setOpacity(100);
		}
		if (GameData::getInstance()->getAtNormal() == 2){
			rank1At->setOpacity(248);
			rank2At->setOpacity(248);
			rank3At->setOpacity(100);
			rank4At->setOpacity(100);
			rank5At->setOpacity(100);
		}
		if (GameData::getInstance()->getAtNormal() == 3){
			rank1At->setOpacity(248);
			rank2At->setOpacity(248);
			rank3At->setOpacity(248);
			rank4At->setOpacity(248);
			rank5At->setOpacity(248);
		}
		if (GameData::getInstance()->getAtOption1() == 0){
			rank4At->setOpacity(100);
		}
		if (GameData::getInstance()->getAtOption2() == 0){
			rank5At->setOpacity(100);
		}
		return;
		break;
	}
}

void UpgradeScene::updateBp(){
	BP->setString(StringUtils::toString(GameData::getInstance()->getBP()));
}

void UpgradeScene::buyTech(int type, int rank, int price){
	if (GameData::getInstance()->getBP() >= price){
		GameData::getInstance()->setBP(GameData::getInstance()->getBP() - price);
		if (type == 1){
			if (rank <= 3){
				GameData::getInstance()->setGunNormal(rank);
			}
			if (rank == 4){
				GameData::getInstance()->setGunOption1(1);
			}
			if (rank == 5){
				GameData::getInstance()->setGunOption2(1);
			}
		}
		if (type == 2){
			if (rank <= 3){
				GameData::getInstance()->setMissileNormal(rank);
			}
			if (rank == 4){
				GameData::getInstance()->setMissileOption1(1);
			}
			if (rank == 5){
				GameData::getInstance()->setMissileOption2(1);
			}
		}
		if (type == 3){
			if (rank <= 3){
				GameData::getInstance()->setAtNormal(rank);
			}
			if (rank == 4){
				GameData::getInstance()->setAtOption1(1);
			}
			if (rank == 5){
				GameData::getInstance()->setAtOption2(1);
			}
		}
		update(type);
		updateBp();
		priceBtn->setVisible(false);
		priceLabel->setVisible(false);
	}
}
