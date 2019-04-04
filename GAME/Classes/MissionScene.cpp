#include "MissionScene.h"
#include "GamePlay.h"

USING_NS_CC;

Layer *BgLayer;
Layer *ItermLayer;
Layer *MissionLevelLayer;
Scene* MissionScene::createScene(){
	return MissionScene::create();
}

bool MissionScene::init(){
	if (!Scene::init())
	{
		return false;
	}
	BgLayer = Layer::create();
	this->addChild(BgLayer);
	ItermLayer = Layer::create();
	this->addChild(ItermLayer);
	MissionLevelLayer = Layer::create();
	this->addChild(MissionLevelLayer);
	createUIMission();
	return true;
}
void MissionScene::createUIMission()
{
	auto bg = Sprite::create("background.png");
	bg->setPosition(Vec2(0, 0));
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setOpacity(180);
	BgLayer->addChild(bg);
	auto missionBg = Sprite::create("mission-bg.png");
	missionBg->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2 - Director::getInstance()->getVisibleSize().height / 10));
	//missionBg->setScale(1.3 ,1.1);
	//missionBg->setAnchorPoint(Vec2(0, 0));
	BgLayer->addChild(missionBg);
	auto label = Label::createWithTTF("SELECT MISSION ", "fonts/kenvector_future_thin.ttf", 60);
	label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height  - Director::getInstance()->getVisibleSize().height / 8));
	BgLayer->addChild(label);
	auto iterm = Sprite::create("mission-not-available.png");
	iterm->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - missionBg->getContentSize().width / 2 + 4 * iterm->getContentSize().width / 5, Director::getInstance()->getVisibleSize().height / 2 - Director::getInstance()->getVisibleSize().height / 10 + missionBg->getContentSize().height / 2 - iterm->getContentSize().height/2));
	iterm->setAnchorPoint(Vec2(0, 1));
	ItermLayer->addChild(iterm);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			auto iterm1 = Sprite::create("mission-not-available.png");
			iterm1->setPosition(Vec2(iterm->getPosition().x + 2 * i * iterm->getContentSize().width + i * iterm->getContentSize().width, iterm->getPosition().y - j * iterm->getContentSize().height - j * iterm->getContentSize().height / 3));
			iterm1->setAnchorPoint(Vec2(0, 1));
			ItermLayer->addChild(iterm1);

		}
	}
	auto mission1 = ui::Button::create("mission-1.png");
	mission1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - missionBg->getContentSize().width / 2 + 4 * iterm->getContentSize().width / 5, Director::getInstance()->getVisibleSize().height / 2 - Director::getInstance()->getVisibleSize().height / 10 + missionBg->getContentSize().height / 2 - iterm->getContentSize().height / 2));
	mission1->setAnchorPoint(Vec2(0, 1));
	mission1->addClickEventListener([&](Ref* event)
	{
		Director::getInstance()->replaceScene(GamePlayScene::createScene(1));
	});
	ItermLayer->addChild(mission1);

	auto mission2 = ui::Button::create("mission-2.png");
	mission2->setPosition(Vec2(mission1->getPositionX() + 3*(mission1->getContentSize().width), mission1->getPositionY()));
	mission2->setAnchorPoint(Vec2(0, 1));
	mission2->addClickEventListener([&](Ref* event)
	{
		Director::getInstance()->replaceScene(GamePlayScene::createScene(2));
	});
	ItermLayer->addChild(mission2);
}
