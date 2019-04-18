#include "MainMenuScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "UpgradeScene.h"
#include <vector>
#include "GameData.h"
#include "MissionScene.h"
using namespace CocosDenshion;
USING_NS_CC;
using namespace std;
TMXTiledMap *mainMenuTile;
float scaleMainMenu;
vector<ValueMap> location;
Layer *MainMenuBackground;
Layer *MainMenuUILayer;
Layer *MainMenuStartLayer;
Layer *TopUILayer;
Layer *QuitLayer;
Layer *SettingLayer;
Layer *AboutLayer;
Layer *UpgradeLayer;

float delayTime;
Sprite *logo;
Sprite *note;
ui::Button *StartButton;
ui::Button *UpgradeButton;
ui::Button *SettingButton;
ui::Button *AboutButton;
ui::Button *QuitButton;
ui::Button *BackButton;
ui::Button *YesButton;
ui::Button *NoButton;
ui::Button *MuteButton;
ui::Button *NotMuteButton;
ui::TextField *textFieldChangeName;
Label *error;
Label *success;
Label *contentAbout;
Label  *myName;
ui::Slider *audioSlider;
Label *textPercent;
int sliderPercent;
char buf[50] = { 0 };
Sprite* nameError;

Scene* MainMenuScene::createScene()
{
	/*auto scene = Scene::create();
	auto layer = AboutScene::create();
	scene->addChild(layer);
	return scene;*/
	return MainMenuScene::create();
}
bool MainMenuScene::init()
{


	if (!Scene::init())
	{
		return false;
	}
	srand(time(NULL));
	auto visibleSize = Director::getInstance()->getVisibleSize();

	GameData *readData = GameData::getInstance();

	MainMenuBackground = Layer::create();
	MainMenuUILayer = Layer::create();
	MainMenuStartLayer = Layer::create();
	TopUILayer = Layer::create();
	TopUILayer->setVisible(false);
	QuitLayer = Layer::create();
	QuitLayer->setVisible(false);
	SettingLayer = Layer::create();
	SettingLayer->setVisible(false);
	AboutLayer = Layer::create();
	AboutLayer->setVisible(false);
	UpgradeLayer = Layer::create();
	UpgradeLayer->setVisible(false);


	this->addChild(MainMenuBackground,-1);
	this->addChild(MainMenuUILayer);
	this->addChild(MainMenuStartLayer);
	this->addChild(TopUILayer, 1);
	this->addChild(QuitLayer);
	this->addChild(SettingLayer);
	this->addChild(AboutLayer);
	this->addChild(UpgradeLayer);

	createMainMenuTiles();
	auto playerTile = mainMenuTile->getObjectGroup("Location");
	ValueMap Point;
	auto object = playerTile->getObjects();
	int size = 1;
	do {
		char s[5] = "";
		sprintf(s, "LP%d", size);
		Point = playerTile->objectNamed(s);
		location.push_back(Point);
		//CCLOG("%d", locationPoint.size());
		size++;
	} while (size <= object.size());

	nameError = Sprite::create("inputNameError.png");
	nameError->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	nameError->setVisible(false);
	this->addChild(nameError);
	

	createUI();
	scheduleUpdate();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);


	return true;
}
void MainMenuScene::createMainMenuTiles()
{
	mainMenuTile = TMXTiledMap::create("map/Menu.tmx");
	mainMenuTile->setPosition(Vec2(0, 0));
	mainMenuTile->setAnchorPoint(Vec2(0, 0));
	scaleMainMenu = Director::getInstance()->getVisibleSize().height / mainMenuTile->getContentSize().height;
	mainMenuTile->setScale(scaleMainMenu);
	MainMenuBackground->addChild(mainMenuTile);
}
void MainMenuScene::createUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	logo = Sprite::create("gamelogo.png");
	logo->setScale(scaleMainMenu);
	logo->setPosition(Vec2(visibleSize.width - scaleMainMenu*logo->getContentSize().width/2, visibleSize.height - scaleMainMenu*logo->getContentSize().height/2));
	MainMenuBackground->addChild(logo);

	StartButton = ui::Button::create("UI/btnStart.png", "UI/btnStartPress.png");
	StartButton->setPosition(Vec2(logo->getPositionX(), logo->getPositionY() - scaleMainMenu * logo->getContentSize().height + 20));
	StartButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::Starts, this));
	MainMenuUILayer->addChild(StartButton);

	UpgradeButton = ui::Button::create("UI/btnUpgrade.png", "UI/btnUpgradePress.png");
	UpgradeButton->setPosition(Vec2(logo->getPositionX(), StartButton->getPositionY()-StartButton->getContentSize().height*1.5));
	UpgradeButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::Upgrade, this));
	MainMenuUILayer->addChild(UpgradeButton);

	SettingButton = ui::Button::create("UI/btnSetting.png", "UI/btnSettingPress.png");
	SettingButton->setPosition(Vec2(logo->getPositionX(), StartButton->getPositionY() - StartButton->getContentSize().height*3));
	SettingButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::Settings, this));
	MainMenuUILayer->addChild(SettingButton);

	AboutButton = ui::Button::create("UI/btnAbout.png", "UI/btnAboutPress.png");
	AboutButton->setPosition(Vec2(logo->getPositionX(), StartButton->getPositionY() - StartButton->getContentSize().height * 4.5));
	AboutButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::Abouts, this));
	MainMenuUILayer->addChild(AboutButton);

	QuitButton = ui::Button::create("UI/btnQuit.png", "UI/btnQuitPress.png");
	QuitButton->setPosition(Vec2(logo->getPositionX(), StartButton->getPositionY() - StartButton->getContentSize().height * 6));
	QuitButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::Quits, this));
	MainMenuUILayer->addChild(QuitButton);

	auto flagIcon=Sprite::create("animation/flag.png");
	
	Vector<SpriteFrame*> AnimFrames;
	AnimFrames.pushBack(SpriteFrame::create("animation/flag1.png", Rect(0, 0, 128, 64)));
	AnimFrames.pushBack(SpriteFrame::create("animation/flag2.png", Rect(0, 0, 128, 64)));
	AnimFrames.pushBack(SpriteFrame::create("animation/flag3.png", Rect(0, 0, 128, 64)));
	AnimFrames.pushBack(SpriteFrame::create("animation/flag4.png", Rect(0, 0, 128, 64)));
	AnimFrames.pushBack(SpriteFrame::create("animation/flag5.png", Rect(0, 0, 128, 64)));
	AnimFrames.pushBack(SpriteFrame::create("animation/flag6.png", Rect(0, 0, 128, 64)));
	AnimFrames.pushBack(SpriteFrame::create("animation/flag7.png", Rect(0, 0, 128, 64)));
	AnimFrames.pushBack(SpriteFrame::create("animation/flag8.png", Rect(0, 0, 128, 64)));
	AnimFrames.pushBack(SpriteFrame::create("animation/flag9.png", Rect(0, 0, 128, 64)));
	AnimFrames.pushBack(SpriteFrame::create("animation/flag10.png", Rect(0, 0, 128, 64)));
	AnimFrames.pushBack(SpriteFrame::create("animation/flag11.png", Rect(0, 0, 128, 64)));

	auto animation2 = Animation::createWithSpriteFrames(AnimFrames, 0.15f);
	auto animate2 = Animate::create(animation2);
	flagIcon->runAction(RepeatForever::create(animate2));

	auto FlagTile = mainMenuTile->getObjectGroup("Flag");
	auto object = FlagTile->getObjects();
	auto flag = FlagTile->objectNamed("Flag");
	flagIcon->setPosition(Vec2(flag.at("x").asInt(), flag.at("y").asInt()));
	MainMenuBackground->addChild(flagIcon);

	BackButton = ui::Button::create("UI/btnBack.png", "UI/btnBackPress.png");
	BackButton->setPosition(Vec2(BackButton->getContentSize().width + Director::getInstance()->getVisibleSize().width / 3 , visibleSize.height - BackButton->getContentSize().height));
	BackButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::Back, this));
	TopUILayer->addChild(BackButton);

	note = Sprite::create("UI/lableNote.png");
	note->setPosition(Vec2(mainMenuTile->getContentSize().width / 3 - mainMenuTile->getContentSize().width / 50, mainMenuTile->getContentSize().height / 2));
	note->setScale(1.5);
	QuitLayer->addChild(note , 2);
	YesButton = ui::Button::create("UI/btnYes.png", "UI/btnYesPress.png");
	YesButton->setPosition(Vec2(note->getPosition().x - note->getContentSize().width / 3, note->getPosition().y - note->getContentSize().height / 3));
	YesButton->setScale(1.2);
	YesButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::funcQuits, this));
	QuitLayer->addChild(YesButton , 2);
	NoButton = ui::Button::create("UI/btnNo.png", "UI/btnNoPress.png");
	NoButton->addClickEventListener([&](Ref* event) {
		QuitLayer->setVisible(false);
	});
	NoButton->setPosition(Vec2(note->getPosition().x + note->getContentSize().width / 3, note->getPosition().y - note->getContentSize().height / 3));
	NoButton->setScale(1.2);
	QuitLayer->addChild(NoButton , 2);
	auto QuitStartButton = ui::Button::create("UI/btnStart.png", "UI/btnStart.png");
	QuitStartButton->setPosition(Vec2(logo->getPositionX(), logo->getPositionY() - scaleMainMenu * logo->getContentSize().height + 20));
	QuitLayer->addChild(QuitStartButton);
	auto QuitUpgradeButton = ui::Button::create("UI/btnUpgrade.png", "UI/btnUpgrade.png");
	QuitUpgradeButton->setPosition(Vec2(logo->getPositionX(), StartButton->getPositionY() - StartButton->getContentSize().height*1.5));
	QuitLayer->addChild(QuitUpgradeButton);
	auto QuitSettingButton = ui::Button::create("UI/btnSetting.png", "UI/btnSetting.png");
	QuitSettingButton->setPosition(Vec2(logo->getPositionX(), StartButton->getPositionY() - StartButton->getContentSize().height * 3));
	QuitLayer->addChild(QuitSettingButton);
	auto QuitAboutButton = ui::Button::create("UI/btnAbout.png", "UI/btnAbout.png");
	QuitAboutButton->setPosition(Vec2(logo->getPositionX(), StartButton->getPositionY() - StartButton->getContentSize().height * 4.5));
	QuitLayer->addChild(QuitAboutButton);
	auto QuitQuitButton = ui::Button::create("UI/btnQuit.png", "UI/btnQuit.png");
	QuitQuitButton->setPosition(Vec2(logo->getPositionX(), StartButton->getPositionY() - StartButton->getContentSize().height * 6));
	QuitLayer->addChild(QuitQuitButton );

	auto bg_opacity = Sprite::create("bg_opacity.png");
	bg_opacity->setPosition(0, 0);
	bg_opacity->setAnchorPoint(Vec2(0, 0));
	bg_opacity->setOpacity(50);
	QuitLayer->addChild(bg_opacity,1);
	
	
	myName = Label::createWithTTF("Change Name:", "fonts/kenvector_future_thin.ttf", 40);
	myName->setPosition(Vec2(mainMenuTile->getContentSize().width / 3 + Director::getInstance()->getVisibleSize().width / 3 - mainMenuTile->getContentSize().width / 10, mainMenuTile->getContentSize().height / 2 + mainMenuTile->getContentSize().height / 20));
	SettingLayer->addChild(myName);
	textFieldChangeName = ui::TextField::create(strcmp(GameData::getInstance()->getNameCharacter().c_str(), "") == 0 ? "Input your name" : GameData::getInstance()->getNameCharacter(), "Arial", 40);
	textFieldChangeName->setMaxLengthEnabled(true);
	textFieldChangeName->setMaxLength(8);
	textFieldChangeName->setPosition(Vec2(myName->getPosition().x + myName->getContentSize().width + mainMenuTile->getContentSize().width / 20, myName->getPosition().y));
	textFieldChangeName->setColor(cocos2d::Color3B(Color3B::BLACK));
	SettingLayer->addChild(textFieldChangeName);
	error = Label::createWithTTF("Please input name ! ","fonts/arial.ttf", 30);
	error->setPosition(myName->getPosition().x + myName->getContentSize().width + mainMenuTile->getContentSize().width / 20, myName->getPosition().y - textFieldChangeName->getContentSize().height);
	error->setVisible(false);
	SettingLayer->addChild(error);
	success = Label::createWithTTF("Your name is changed successfully. ", "fonts/arial.ttf", 30);
	success->setPosition(myName->getPosition().x + myName->getContentSize().width + mainMenuTile->getContentSize().width / 20, myName->getPosition().y - textFieldChangeName->getContentSize().height);
	success->setVisible(false);
	SettingLayer->addChild(success);
	auto btnCheck = ui::Button::create("UI/btnYes.png", "UI/btnYesPress.png");
	btnCheck->setPosition(Vec2(textFieldChangeName->getPosition().x + textFieldChangeName->getContentSize().width / 2 + mainMenuTile->getContentSize().width / 30, myName->getPosition().y + myName->getContentSize().height/3));
	btnCheck->addClickEventListener(CC_CALLBACK_0(MainMenuScene::ChangeName,this)); 
	btnCheck->setScale(1.2);
	SettingLayer->addChild(btnCheck);
	auto myAudio = Label::createWithTTF("Audio:", "fonts/kenvector_future_thin.ttf", 40);
	myAudio->setPosition(Vec2(myName->getPosition().x - myName->getContentSize().width/4,myName->getPosition().y - 3* myName->getContentSize().height));
	SettingLayer->addChild(myAudio);
	audioSlider = ui::Slider::create();
	audioSlider->loadBarTexture("UI/slider_bar_bg.png");
	audioSlider->loadSlidBallTextures("UI/slider_ball_normal.png", "unit/plane2.png", "UI/slider_ball_disable.png");
	audioSlider->loadProgressBarTexture("UI/slider_bar_pressed.png");
	audioSlider->setPercent(GameData::getInstance()->getVolume());

	audioSlider->setPosition(Vec2(myAudio->getPosition().x + myName->getContentSize().width + mainMenuTile->getContentSize().width / 20, myName->getPosition().y - 3 * textFieldChangeName->getContentSize().height));

	textPercent = Label::createWithTTF("", "fonts/kenvector_future_thin.ttf", 40);
	textPercent->setString(StringUtils::format("%d %c", audioSlider->getPercent(), '%'));
	
	textPercent->setPosition(Vec2(audioSlider->getPosition().x, audioSlider->getPosition().y - audioSlider->getContentSize().height * 2));
	SettingLayer->addChild(textPercent);
	audioSlider->addEventListener(CC_CALLBACK_0(MainMenuScene::ChangedSlider , this));

	SettingLayer->addChild(audioSlider);

	MuteButton = ui::Button::create("UI/mute.png");
	MuteButton->setPosition(Vec2(btnCheck->getPosition().x + btnCheck->getContentSize().width / 2 + 50, audioSlider->getPosition().y));
	MuteButton->setVisible(false);
	MuteButton->setScale(1.4);
	SettingLayer->addChild(MuteButton);
	NotMuteButton = ui::Button::create("UI/notMute.png");
	NotMuteButton->setPosition(Vec2(btnCheck->getPosition().x + btnCheck->getContentSize().width / 2 + 50, audioSlider->getPosition().y));
	NotMuteButton->setVisible(true);
	NotMuteButton->setScale(1.4);
	SettingLayer->addChild(NotMuteButton);
	MuteButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::Mute,this));
	NotMuteButton->addClickEventListener(CC_CALLBACK_0(MainMenuScene::Mute,this));
	contentAbout = Label::createWithTTF("World  War  III:  Vietnam  Islands\n\nGameloft  Summer  Intership 2018\n\nGroup1  Project", "fonts/kenvector_future.ttf", 20);
	contentAbout->setHorizontalAlignment(CCTextAlignment::CENTER);
	contentAbout->setPosition(Vec2(mainMenuTile->getContentSize().width / 3 + Director::getInstance()->getVisibleSize().width / 3 + mainMenuTile->getContentSize().width / 20, mainMenuTile->getContentSize().height / 3 + mainMenuTile->getContentSize().height / 10));
	AboutLayer->addChild(contentAbout);

}
void MainMenuScene::update(float delta)
{
	delayTime += delta;
	if (delayTime > 1)
	{
		delayTime = 0;
		int randn = rand() % (location.size() - 1);
		auto mc = location[randn];
		auto x = mc.at("x").asInt();
		auto y = mc.at("y").asInt();
		auto particle = ParticleSystemQuad::create("particle/smoke.plist");
		particle->setScale(0.3);
		particle->setDuration(2);
		particle->setPosition(Vec2(x,y));
		MainMenuBackground->addChild(particle);
	}

}
void MainMenuScene::Back()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	TopUILayer->setVisible(false);
	MainMenuUILayer->setVisible(true);
	QuitLayer->setVisible(false);
	SettingLayer->setVisible(false);
	AboutLayer->setVisible(false);
	MainMenuBackground->runAction(MoveBy::create(0.5, Vec2(Director::getInstance()->getVisibleSize().width / 3, 0)));
	MainMenuUILayer->runAction(MoveBy::create(0.5, Vec2(Director::getInstance()->getVisibleSize().width / 3, 0)));
	logo->runAction(MoveTo::create(0.5, Vec2(visibleSize.width - scaleMainMenu*logo->getContentSize().width / 2, visibleSize.height - scaleMainMenu*logo->getContentSize().height / 2)));
	//MainMenuBackground->addChild(logo);
	//SettingLayer->runAction(MoveTo::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width +300 / 3, 0)));
	SettingLayer->runAction(MoveTo::create(0.5, Vec2(0, 0)));
	AboutLayer->runAction(MoveTo::create(0.5 , Vec2(0, 0)));
	TopUILayer->runAction(MoveBy::create(0.5, Vec2(Director::getInstance()->getVisibleSize().width / 3, 0)));
}
void MainMenuScene::Starts()
{
	if (!strcmp(GameData::getInstance()->getNameCharacter().c_str(), "")){
		if (!nameError->isVisible()){
			nameError->setVisible(true);
			auto fo = FadeOut::create(2);
			auto cf = CallFunc::create([&](){
				nameError->setVisible(false);
				nameError->setOpacity(248);
			});
			nameError->runAction(Sequence::create(DelayTime::create(1),fo, cf,nullptr));
		}
	} else
	Director::getInstance()->replaceScene(MissionScene::create());
	
}
void MainMenuScene::Settings()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	SettingLayer->setVisible(true);
	TopUILayer->setVisible(true);
	//SettingLayer->runAction(MoveTo::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 3, 0)));

	//SettingLayer->runAction(MoveTo::create(0.5),)
	MainMenuUILayer->setVisible(false);
	MainMenuBackground->runAction(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 3, 0)));
	MainMenuUILayer->runAction(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 3, 0)));
	logo->setPosition(Vec2(visibleSize.width - scaleMainMenu*logo->getContentSize().width / 2 + 250, visibleSize.height - scaleMainMenu*logo->getContentSize().height / 2));
	//logo->runAction(MoveTo::create(0.5, Vec2(visibleSize.width - scaleMainMenu*logo->getContentSize().width / 2 + 250, visibleSize.height - scaleMainMenu*logo->getContentSize().height / 2)));
	SettingLayer->runAction(MoveBy::create(0.5, Vec2(- Director::getInstance()->getVisibleSize().width / 3 , 0)));
	TopUILayer->runAction(MoveBy::create(0.5, Vec2(- Director::getInstance()->getVisibleSize().width / 3, 0)));
}

void MainMenuScene::Abouts()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	AboutLayer->setVisible(true);
	TopUILayer->setVisible(true);
	MainMenuUILayer->setVisible(false);
	MainMenuBackground->runAction(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 3, 0)));
	MainMenuUILayer->runAction(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 3, 0)));
	logo->runAction(MoveTo::create(0.5, Vec2(visibleSize.width - scaleMainMenu*logo->getContentSize().width / 2 + 250, visibleSize.height - scaleMainMenu*logo->getContentSize().height / 2)));
	TopUILayer->runAction(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 3, 0)));
	AboutLayer->runAction(MoveBy::create(0.5, Vec2(- Director::getInstance()->getVisibleSize().width / 3,0)));
}
void MainMenuScene::Quits()
{
	QuitLayer->setVisible(true);


}
void MainMenuScene::Upgrade()
{
	Director::getInstance()->replaceScene(UpgradeScene::createScene());
	/*auto visibleSize = Director::getInstance()->getVisibleSize();
	TopUILayer->setVisible(true);
	MainMenuUILayer->setVisible(false);
	MainMenuBackground->runAction(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 3, 0)));
	MainMenuUILayer->runAction(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 3, 0)));
	logo->runAction(MoveTo::create(0.5, Vec2(visibleSize.width - scaleMainMenu*logo->getContentSize().width / 2 + 250, visibleSize.height - scaleMainMenu*logo->getContentSize().height / 2)));
	TopUILayer->runAction(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width / 3, 0)));*/
}
void MainMenuScene::funcQuits()
{
	Director::getInstance()->end();
}
void MainMenuScene::ChangeName()
{
	if (textFieldChangeName->getString().size() != 0)
	{
		GameData::getInstance()->setNameCharacter(textFieldChangeName->getString());
		success->setVisible(true);
		error->setVisible(false);
	}
	else
	{
		success->setVisible(false);
		error->setVisible(true);
	}

	
}
void MainMenuScene::ChangedSlider()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(audioSlider->getPercent() / 100));
	textPercent->setString(StringUtils::format("%d %c", audioSlider->getPercent(),'%'));
	sliderPercent = audioSlider->getPercent();
	GameData::getInstance()->setVolume(audioSlider->getPercent());
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(audioSlider->getPercent());
	SimpleAudioEngine::getInstance()->setEffectsVolume(audioSlider->getPercent());
	if (audioSlider->getPercent() > 0)
	{
		NotMuteButton->setVisible(true);
		MuteButton->setVisible(false);
	}
	else
	{
		NotMuteButton->setVisible(false);
		MuteButton->setVisible(true);
	}
}
void MainMenuScene::Mute()
{
	if (NotMuteButton->isVisible())
	{
		MuteButton->setVisible(true);
		NotMuteButton->setVisible(false);
		audioSlider->setPercent(0);
		GameData::getInstance()->setVolume(audioSlider->getPercent());
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(audioSlider->getPercent());
		SimpleAudioEngine::getInstance()->setEffectsVolume(audioSlider->getPercent());
		textPercent->setString(StringUtils::format("%d %c", 0, '%'));
	}
	else
	{
		MuteButton->setVisible(false);
		NotMuteButton->setVisible(true);
		audioSlider->setPercent(sliderPercent);
		GameData::getInstance()->setVolume(audioSlider->getPercent());
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(audioSlider->getPercent());
		SimpleAudioEngine::getInstance()->setEffectsVolume(audioSlider->getPercent());
		textPercent->setString(StringUtils::format("%d %c", audioSlider->getPercent(), '%'));
	}
}
int MainMenuScene::stringToIntMainMenu(string s) {
	if (s.length() == 0)
		return 0;

	int p = 0;
	int l = 0;
	if (s[0] == '-')
		l++;

	for (int i = l; i < s.length(); i++) {
		int tmp = s[i] - '0';
		p = 10 * p + tmp;
	}

	return p;
}