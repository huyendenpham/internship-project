#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "LoadingScene.h"
#include <vector>
#include <iostream>
#include "MainMenuScene.h"

USING_NS_CC;
#define NUM_OF_TIPS 5
using namespace std;
Scene* LoadingScene::createScene()
{
	auto scene = LoadingScene::create();
	return scene;
}
Sprite *LoadingScene::createSprite()
{
	return tank;
}
bool LoadingScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// create LoadingBar
	//////////////////////////////////////////////////////////////////////
	loadingBarGB = Sprite::create("UI/loadingBarBG.png");
	loadingBarGB->setPosition(Vec2(visibleSize.width / 2, loadingBarGB->getContentSize().height));
	addChild(loadingBarGB);
	loadingbar = ui::LoadingBar::create("UI/loadingBar.png");
	loadingbar->setPercent(0);
	loadingbar->setDirection(ui::LoadingBar::Direction::LEFT);
	loadingbar->setPosition(loadingBarGB->getPosition());
	addChild(loadingbar);
	//////////////////////////////////////////////////////////////////////
	//create icon tank
	tank = Sprite::create("unit/plane2bg.png");
	tank->setAnchorPoint(Vec2(1.0f, 0.5f));
	tank->setPosition(Vec2(loadingBarGB->getPositionX() - loadingBarGB->getContentSize().width / 2, loadingBarGB->getPositionY()));
	this->addChild(tank);
	plane= Sprite::create("unit/plane2.png");
	plane->setAnchorPoint(Vec2(1.0f, 0.5f));
	plane->setPosition(Vec2(tank->getPositionX()+10,tank->getPositionY()+10));
	this->addChild(plane);
	//////////////////////////////////////////////////////////////////////
	/*auto sequenceRunUpdateLoadingBar = Sequence::createWithTwoActions(CallFunc::create(CC_CALLBACK_0(LoadingScene::updateLoadingBar, this)), DelayTime::create(0.1f));
	auto repeat = Repeat::create(sequenceRunUpdateLoadingBar, 101);
	loadingbar->runAction(repeat);*/
	//////////////////////////////////////////////////////////////////////
	//create background
	auto BG = Sprite::create("background.png");
	BG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	BG->setScale(visibleSize.width / BG->getContentSize().width);
	BG->setOpacity(150);
	this->addChild(BG, -1);
	//////////////////////////////////////////////////////////////////////
	//create label
	labelPercent = Label::createWithTTF("0", "fonts/kenvector_future.ttf", 40);
	labelPercent->setPosition(Vec2(visibleSize.width / 2, loadingbar->getPositionY() + loadingbar->getContentSize().height / 2));
	this->addChild(labelPercent);
	//////////////////////////////////////////////////////////////////////
	labelNext = Label::createWithTTF(">>>>>>>>>>>>>>>>>>>> Touch screen to start !!! >>>>>>>>>>>>>>>>>>>>>>", "fonts/Marker Felt.ttf", 30);
	labelNext->setPosition(Vec2(visibleSize.width / 2, loadingbar->getPositionY()));
	labelNext->setVisible(false);
	//////////////////////////////////////////////////////////////////////
	auto sequen = Sequence::create(FadeOut::create(1.0f), FadeIn::create(1.0f), NULL);
	labelNext->runAction(RepeatForever::create(sequen));
	this->addChild(labelNext);
	//////////////////////////////////////////////////////////////////////
	//create Tip
	for (int i = 0;i < NUM_OF_TIPS;i++)
	{
		nameOfTips.push_back(StringUtils::format("Tips/tip%d.png", i));
	}
	indexTip = RandomHelper::random_int(0, 4);
	TipSprite = Sprite::create(nameOfTips[indexTip]);
	
	TipSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 30));
	TipSprite->setScale((float)2 / 3);
	this->addChild(TipSprite, 2);

	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(LoadingScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(LoadingScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(LoadingScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();

	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/bgm.mp3");
	audio->preloadBackgroundMusic("music/bgm1.mp3");
	audio->preloadEffect("music/unit_dead.mp3");
	audio->preloadEffect("music/explosion.mp3");
	audio->preloadEffect("music/missile_shot.mp3");
	audio->preloadEffect("music/tank_fire.mp3");
	audio->preloadEffect("music/tower_up.mp3");
	audio->preloadEffect("music/build_tower.mp3");
	audio->preloadEffect("music/gun_shot.mp3");
	audio->preloadEffect("music/sell_tower.mp3");
	audio->preloadEffect("music/soldier_shot.mp3");
	return true;
}
void LoadingScene::updateLoadingBar()
{
	if (loadingbar->getPercent() < 50)
	{
		loadingbar->setPercent(loadingbar->getPercent() + 1);
		float x = loadingbar->getPositionX() + loadingbar->getContentSize().width / 2;
		x -= ((100 - loadingbar->getPercent()) / 100) * loadingbar->getContentSize().width;
		tank->setPosition(x, loadingbar->getPositionY() - 6);
		plane->setPosition(Vec2(tank->getPositionX() -10, tank->getPositionY() + 20));
		labelPercent->setString(StringUtils::format("%.0f %s", loadingbar->getPercent(), "%"));
	}
	else if (loadingbar->getPercent() == 50)
	{
		//nextScene = GamePlayScene::createScene();
		loadingbar->setPercent(loadingbar->getPercent() + 1);
		float x = loadingbar->getPositionX() + loadingbar->getContentSize().width / 2;
		x -= ((100 - loadingbar->getPercent()) / 100) * loadingbar->getContentSize().width;
		tank->setPosition(x, loadingbar->getPositionY() - 6);
		plane->setPosition(Vec2(tank->getPositionX() - 10, tank->getPositionY() + 20));
		labelPercent->setString(StringUtils::format("%.0f %s", loadingbar->getPercent(), "%"));
	}
	else if ((loadingbar->getPercent() > 50) && (loadingbar->getPercent() < 100))
	{
		loadingbar->setPercent(loadingbar->getPercent() + 1);
		float x = loadingbar->getPositionX() + loadingbar->getContentSize().width / 2;
		x -= ((100 - loadingbar->getPercent()) / 100) * loadingbar->getContentSize().width;
		tank->setPosition(x, loadingbar->getPositionY() - 6);
		plane->setPosition(Vec2(tank->getPositionX() - 10, tank->getPositionY() + 20));
		labelPercent->setString(StringUtils::format("%.0f %s", loadingbar->getPercent(), "%"));
	}
	else
	{
		loadingbar->setVisible(false);
		loadingBarGB->setVisible(false);
		tank->setVisible(false);
		labelPercent->setVisible(false);
		labelNext->setVisible(true);
		plane->setVisible(false);
		nextFlag = true;
	}
}
void LoadingScene::update(float dt){
	updateLoadingBar();
}
void LoadingScene::changeTip(int n)
{
	TipSprite->setTexture(nameOfTips[n]);
}
bool LoadingScene::onTouchBegan(Touch* touch, Event  *event) {
	nextTip = touch->getLocation().x;
	
	return true;
}
void LoadingScene::onTouchMoved(Touch* touch, Event  *event) {

}
void LoadingScene::onTouchEnded(Touch* touch, Event  *event) {
	
	
	if (nextFlag == true)
	{
		//nextScene = GamePlayScene::createScene();
		Director::getInstance()->replaceScene(MainMenuScene::createScene());

	}
	if (nextTip > touch->getLocation().x) 
	{
		indexTip--;
		if (indexTip < 0) indexTip = nameOfTips.size() - 1;
		auto fadein = FadeIn::create(0.5f);
		auto fadeout = FadeOut::create(0.5);
		auto callfunc = CallFunc::create(CC_CALLBACK_0(LoadingScene::changeTip, this, indexTip));
		auto sequen = Sequence::create(fadeout, callfunc, fadein, NULL);
		TipSprite->runAction(sequen);
	}
	if (nextTip < touch->getLocation().x)
	{
		{
			indexTip++;
			if (indexTip >= nameOfTips.size()) indexTip = 0;
			auto fadein = FadeIn::create(0.5f);
			auto fadeout = FadeOut::create(0.5f);
			auto callfunc = CallFunc::create(CC_CALLBACK_0(LoadingScene::changeTip, this, indexTip));
			auto sequen = Sequence::create(fadeout, callfunc, fadein, NULL);
			TipSprite->runAction(sequen);
		}
	}
}
