#include "UI.h"
#include "SimpleAudioEngine.h"
#include "MainMenuScene.h"
#include "MissionScene.h"
#include "GameData.h"
#include "GamePlay.h"
#include <fstream>
#include "json/writer.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <fstream>

using namespace rapidjson;
using namespace std;



UI::UI(int level){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->level = level;
	// add wave numb
	wave = Label::createWithTTF("Wave x/y", "fonts/kenvector_future.ttf", 30);
	wave->enableShadow();
	wave->setPosition(visibleSize.width*2 / 5 + origin.x, wave->getContentSize().height);
	this->addChild(wave);

	// add joy tick
	Rect joystickBaseDimensions;
	joystickBaseDimensions = Rect(0, 0, 300.0f, 300.0f);

	Point joystickBasePosition;
	joystickBasePosition = Vec2(visibleSize.height*0.2f + origin.x, visibleSize.height*0.3f + origin.y);

	SneakyJoystickSkinnedBase *joystickBase = new SneakyJoystickSkinnedBase();
	joystickBase->init();
	joystickBase->setPosition(joystickBasePosition);
	joystickBase->setBackgroundSprite(Sprite::create("res/joystick-back.png"));
	joystickBase->getBackgroundSprite()->setOpacity(50.0f);
	joystickBase->setThumbSprite(Sprite::create("res/stick.png"));
	joystickBase->getThumbSprite()->setOpacity(50.0f);
	joystickBase->setContentSize(Size(visibleSize.height*0.3f, visibleSize.height*0.3f));
	SneakyJoystick *aJoystick = new SneakyJoystick();
	aJoystick->initWithRect(joystickBaseDimensions);

	aJoystick->autorelease();
	joystickBase->setJoystick(aJoystick);
	joystickBase->setPosition(joystickBasePosition);

	leftJoystick = joystickBase->getJoystick();
	leftJoystick->retain();
	this->addChild(joystickBase);

	// add tower shop
	auto towershopBG = Sprite::create("towershop-bg.png");
	towershopBG->setAnchorPoint(Vec2(0, 0));
	towershopBG->setPosition(visibleSize.width + origin.x - towershopBG->getContentSize().width, origin.y);
	this->addChild(towershopBG);

	towershopAT = Sprite::create("towershop-at.png");
	towershopAT->setAnchorPoint(Vec2(0, 0));
	towershopAT->setPosition(visibleSize.width + origin.x - towershopAT->getContentSize().width, origin.y);
	this->addChild(towershopAT);

	towershopMi = Sprite::create("towershop-missile.png");
	towershopMi->setAnchorPoint(Vec2(0, 0));
	towershopMi->setPosition(visibleSize.width + origin.x - towershopAT->getContentSize().width - towershopMi->getContentSize().width - 25, origin.y);
	this->addChild(towershopMi);

	towershopGun = Sprite::create("towershop-gun.png");
	towershopGun->setAnchorPoint(Vec2(0, 0));
	towershopGun->setPosition(visibleSize.width + origin.x - towershopAT->getContentSize().width - towershopMi->getContentSize().width - towershopGun->getContentSize().width - 50, origin.y);
	this->addChild(towershopGun);

	// add game & hero skill

	skillGame1 = Sprite::create("game-skill-1.png");
	skillGame1->setPosition(visibleSize.width + origin.x - skillGame1->getContentSize().width, visibleSize.height / 2 + 50 + origin.y);
	this->addChild(skillGame1);

	skillGameBg = Sprite::create("UI/labelBg.png");
	skillGameBg->setPosition(skillGame1->getPositionX(), skillGame1->getPositionY() - 25);
	this->addChild(skillGameBg);

	skillGameTime = Label::create("", "fonts/kenvector_future.ttf", 12);
	skillGameTime->setPosition(skillGameBg->getPosition());
	skillGameTime->setTextColor(Color4B(0, 0, 0, 255));
	this->addChild(skillGameTime);


	skillHero1 = Sprite::create("hero-skill-1.png");
	skillHero1->setPosition(skillGame1->getPositionX(), skillGame1->getPositionY() - skillGame1->getContentSize().height*1.5);
	this->addChild(skillHero1);

	skillHeroBg = Sprite::create("UI/labelBg.png");
	skillHeroBg->setPosition(skillHero1->getPositionX(), skillHero1->getPositionY() - 25);

	this->addChild(skillHeroBg);

	skillHeroTime = Label::create("", "fonts/kenvector_future.ttf", 12);
	skillHeroTime->setPosition(skillHeroBg->getPosition());
	skillHeroTime->setTextColor(Color4B(0, 0, 0, 255));
	this->addChild(skillHeroTime);

	// add menu buttons
	Vector<MenuItem*> menuItems;
	/*pauseButton = MenuItemImage::create("button-pause.png", "button-pause.png");
	menuItems.pushBack(pauseButton);*/
	/*playButton = MenuItemImage::create("button-play.png", "button-play.png");
	playButton->setVisible(false);
	menuItems.pushBack(playButton);*/
	/*x2Button = MenuItemImage::create("button-x2.png", "button-x2.png");
	x2Button->setPosition(pauseButton->getPositionX() + origin.x - pauseButton->getContentSize().width - 25, pauseButton->getPositionY());
	menuItems.pushBack(x2Button);*/
	menuButton = MenuItemImage::create("UI/button-menu.png", "UI/button-menu.png", CC_CALLBACK_0(UI::GamePlayMenu, this));
	//menuButton->setPosition(pauseButton->getPositionX() + origin.x - x2Button->getContentSize().width - pauseButton->getContentSize().width - 25, pauseButton->getPositionY());
	menuItems.pushBack(menuButton);

	replayButton = MenuItemImage::create("UI/restart.png", "UI/restart.png", CC_CALLBACK_0(UI::ReplayGame, this));
	replayButton->setPosition(menuButton->getPositionX() + origin.x - menuButton->getContentSize().width - 25, menuButton->getPositionY());
	menuItems.pushBack(replayButton);


	auto menuButtons = Menu::createWithArray(menuItems);
	menuButtons->setPosition(visibleSize.width + origin.x - menuButton->getContentSize().width, visibleSize.height + origin.y - menuButton->getContentSize().height*0.75);
	this->addChild(menuButtons);

	// add info bar

	auto infoBarBG = Sprite::create("UI/info-bar.png");
	infoBarBG->setPosition(25 + origin.x, origin.y + visibleSize.height - infoBarBG->getContentSize().height - 25);
	infoBarBG->setAnchorPoint(Vec2(0, 0));
	this->addChild(infoBarBG);

	goldLabel = Label::create("100", "/fonts/kenvector_future.ttf", 18);
	goldLabel->setAnchorPoint(Vec2(0.5, 0.5));
	goldLabel->setPosition(infoBarBG->getPositionX() + 90, infoBarBG->getPositionY() + 15);
	this->addChild(goldLabel);

	liveLabel = Label::create("10", "/fonts/kenvector_future.ttf", 18);
	liveLabel->setAnchorPoint(Vec2(0.5, 0.5));
	liveLabel->setPosition(infoBarBG->getPositionX() + 167, infoBarBG->getPositionY() + 15);
	this->addChild(liveLabel);


	nameLabel = Label::create(GameData::getInstance()->getNameCharacter(), "/fonts/kenvector_future.ttf", 16);
	nameLabel->setAnchorPoint(Vec2(0.5, 0.5));
	nameLabel->setDimensions(60, 0);
	nameLabel->setHorizontalAlignment(CCTextAlignment::CENTER);
	nameLabel->setPosition(infoBarBG->getPositionX() + 60, infoBarBG->getPositionY() + 55);
	this->addChild(nameLabel);

	hpLabel = Label::create("100/100", "/fonts/kenvector_future.ttf", 17);
	hpLabel->setAnchorPoint(Vec2(0.5, 0.5));
	hpLabel->setPosition(infoBarBG->getPositionX() + 300, infoBarBG->getPositionY() + 30);
	this->addChild(hpLabel);

	equipLabel = Label::create("Lv.1", "/fonts/kenvector_future.ttf", 17);
	equipLabel->setAnchorPoint(Vec2(0.5, 0.5));
	equipLabel->setPosition(infoBarBG->getPositionX() + 377, infoBarBG->getPositionY() + 30);
	this->addChild(equipLabel);


	hpBar = ui::LoadingBar::create("UI/hp-bar.png");
	hpBar->setDirection(ui::LoadingBar::Direction::LEFT);
	hpBar->setAnchorPoint(Vec2(0, 0));
	hpBar->setPercent(100);
	hpBar->setPosition(Vec2(infoBarBG->getPositionX() + 94, infoBarBG->getPositionY() + 49));
	this->addChild(hpBar);

	createMenuLayer();
	menuOpened = false;

}

SneakyJoystick* UI::getJoytick(){
	return leftJoystick;
};

Sprite* UI::getAT(){
	return towershopAT;
};
Sprite* UI::getMi(){
	return towershopMi;
};
Sprite* UI::getGun(){
	return towershopGun;
};
Label* UI::getGoldLabel(){
	return goldLabel;
};
Label* UI::getLiveLabel(){
	return liveLabel;
};
Label* UI::getNameLabel(){
	return nameLabel;
};
Label* UI::getHpLabel(){
	return hpLabel;
};
Label* UI::getEquipLabel(){
	return equipLabel;
};
Label* UI::getWaveLabel(){
	return wave;
}
ui::LoadingBar* UI::getHpBar(){
	return hpBar;
};


void UI::ResumePlayGame()
{
	Director::getInstance()->resume();
	menuOpened = false;

	replayButton->setEnabled(true);
	menuButton->setEnabled(true);
	GamePlayMenuBg->setVisible(false);
	GamePlayMenuBtn->setVisible(false);
}
void UI::GamePlayMenu()
{
	Director::getInstance()->pause();

	menuOpened = true;

	replayButton->setEnabled(false);
	menuButton->setEnabled(false);
	GamePlayMenuBg->setVisible(true);
	GamePlayMenuBtn->setVisible(true);
}
void UI::SettingInGame()
{
	SettingGameLayer->setVisible(true);
	GamePlayMenuBtn->setVisible(false);
	GamePlayMenuBg->setVisible(false);

}

void UI::readFileSettingGame(std::string path)
{
	FILE *fp = fopen(path.c_str(), "r");


	if (!fp)
	{
		return;
	}

	fgets(bufSt, 50, fp);

	fclose(fp);

}

void UI::ChangedSliderGame()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(audioSliderSetting->getPercent() / 100));
	sliderPercentGame = audioSliderSetting->getPercent();
	GameData::getInstance()->setVolume(audioSliderSetting->getPercent());
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(audioSliderSetting->getPercent());
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(audioSliderSetting->getPercent());
	textSettingPer->setString(StringUtils::format("%d %c", audioSliderSetting->getPercent(), '%'));

}
void UI::ReplayGame()
{
	Director::getInstance()->pause();
	ReplayGameLayer->setVisible(true);
	menuButton->setEnabled(false);
	replayButton->setEnabled(false);
	/*
	Director::getInstance()->resume();
	Director::getInstance()->replaceScene(GamePlayScene::createScene(1));*/
}


void UI::createMenuLayer(){
	// menu layer
	GamePlayMenuBg = Layer::create();
	GamePlayMenuBg->setVisible(false);
	this->addChild(GamePlayMenuBg, 1);
	GamePlayMenuBtn = Layer::create();
	GamePlayMenuBtn->setVisible(false);
	this->addChild(GamePlayMenuBtn, 2);
	SettingGameLayer = Layer::create();
	this->addChild(SettingGameLayer, 3);
	SettingGameLayer->setVisible(false);
	ReplayGameLayer = Layer::create();
	this->addChild(ReplayGameLayer);
	ReplayGameLayer->setVisible(false);
	MissionGameLayer = Layer::create();
	this->addChild(MissionGameLayer);
	MissionGameLayer->setVisible(false);
	QuitLevelLayer = Layer::create();
	this->addChild(QuitLevelLayer);
	QuitLevelLayer->setVisible(false);

	MenuBg = Sprite::create("gameplay_menu_bg.png");
	MenuBg->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
	GamePlayMenuBg->addChild(MenuBg);
	GamePlayMenuBg->setVisible(false);

	reSumeGame = ui::Button::create("UI/resume-button.png", "UI/resume2-button.png");
	reSumeGame->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2 + MenuBg->getContentSize().height/2 - 2 * reSumeGame->getContentSize().height/2));
	GamePlayMenuBtn->addChild(reSumeGame);
	reSumeGame->setAnchorPoint(Vec2(0.5, 1));
	reSumeGame->addClickEventListener(CC_CALLBACK_0(UI::ResumePlayGame, this));

	
	missionGame = ui::Button::create("UI/mission-button.png", "UI/mission2-button.png");
	missionGame->setPosition(Vec2(reSumeGame->getPosition().x, reSumeGame->getPosition().y - 2 * reSumeGame->getContentSize().height));
	missionGame->addClickEventListener([&](Ref* event) {
		MissionGameLayer->setVisible(true);
		GamePlayMenuBtn->setVisible(false);
		GamePlayMenuBg->setVisible(false);

		//Director::getInstance()->resume();// phai resum toan bo chuong trinh truong truoc khi quay ve mainmenuscene
		//Director::getInstance()->replaceScene(MissionScene::create());
	});
	GamePlayMenuBtn->addChild(missionGame);

	settingGameBtn = ui::Button::create("UI/settings-button.png", "UI/settings2-button.png");
	settingGameBtn->setPosition(Vec2(reSumeGame->getPosition().x, reSumeGame->getPosition().y - 3.5 * reSumeGame->getContentSize().height));
	GamePlayMenuBtn->addChild(settingGameBtn);
	settingGameBtn->addClickEventListener(CC_CALLBACK_0(UI::SettingInGame, this));

	


	quitGamePlay = ui::Button::create("UI/quit-button.png", "UI/quit2-button.png");
	quitGamePlay->setPosition(Vec2(reSumeGame->getPosition().x, reSumeGame->getPosition().y - 5 * reSumeGame->getContentSize().height));
	GamePlayMenuBtn->addChild(quitGamePlay);
	quitGamePlay->addClickEventListener([&](Ref* event) {
		QuitLevelLayer->setVisible(true);
		GamePlayMenuBtn->setVisible(false);
		GamePlayMenuBg->setVisible(false);

		//Director::getInstance()->resume();// phai resum toan bo chuong trinh truong truoc khi quay ve mainmenuscene
		//Director::getInstance()->replaceScene(MainMenuScene::create());
	});
	createSettingLayer();

	auto bg_opacity = Sprite::create("bg_opacity.png");
	bg_opacity->setPosition(0, 0);
	bg_opacity->setAnchorPoint(Vec2(0, 0));
	bg_opacity->setOpacity(120);
	GamePlayMenuBg->addChild(bg_opacity, 1);
	auto bg_opacity1 = Sprite::create("bg_opacity.png");
	bg_opacity1->setPosition(0, 0);
	bg_opacity1->setAnchorPoint(Vec2(0, 0));
	bg_opacity1->setOpacity(120);
	ReplayGameLayer->addChild(bg_opacity1);
	auto bg_opacity2 = Sprite::create("bg_opacity.png");
	bg_opacity2->setPosition(0, 0);
	bg_opacity2->setAnchorPoint(Vec2(0, 0));
	bg_opacity2->setOpacity(120);
	MissionGameLayer->addChild(bg_opacity2, 1);
	auto bg_opacity3 = Sprite::create("bg_opacity.png");
	bg_opacity3->setPosition(0, 0);
	bg_opacity3->setAnchorPoint(Vec2(0, 0));
	bg_opacity3->setOpacity(120);
	QuitLevelLayer->addChild(bg_opacity3, 1);


	auto note = Sprite::create("UI/replay-note.png");
	note->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
	note->setScale(1.5);
	ReplayGameLayer->addChild(note, 2);
	auto YesButton = ui::Button::create("UI/btnYes.png", "UI/btnYesPress.png");
	YesButton->setPosition(Vec2(note->getPosition().x - note->getContentSize().width / 3, note->getPosition().y - note->getContentSize().height / 3));
	YesButton->setScale(1.2);
	YesButton->addClickEventListener([&](Ref* event){
		Director::getInstance()->resume();
		Director::getInstance()->replaceScene(GamePlayScene::createScene(level));
		menuButton->setEnabled(true);
		replayButton->setEnabled(true);
	});
	ReplayGameLayer->addChild(YesButton, 2);
	auto NoButton = ui::Button::create("UI/btnNo.png", "UI/btnNoPress.png");
	NoButton->addClickEventListener([&](Ref* event) {
		Director::getInstance()->resume();
		ReplayGameLayer->setVisible(false);
		menuButton->setEnabled(true);
		replayButton->setEnabled(true);
	});
	NoButton->setPosition(Vec2(note->getPosition().x + note->getContentSize().width / 3, note->getPosition().y - note->getContentSize().height / 3));
	NoButton->setScale(1.2);
	ReplayGameLayer->addChild(NoButton, 2);

	auto noteMission = Sprite::create("UI/select-mission.png");
	noteMission->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
	noteMission->setScale(1.5);
	MissionGameLayer->addChild(noteMission, 3);
	auto YesButtonMission = ui::Button::create("UI/btnYes.png", "UI/btnYesPress.png");
	YesButtonMission->setPosition(Vec2(noteMission->getPosition().x - noteMission->getContentSize().width / 3, noteMission->getPosition().y - noteMission->getContentSize().height / 3));
	YesButtonMission->setScale(1.2);
	YesButtonMission->addClickEventListener([&](Ref* event){
		Director::getInstance()->resume();// phai resum toan bo chuong trinh truong truoc khi quay ve mainmenuscene
		Director::getInstance()->replaceScene(MissionScene::create());
		//Director::getInstance()->resume();
		//Director::getInstance()->replaceScene(GamePlayScene::createScene(1));
		menuButton->setEnabled(true);
		replayButton->setEnabled(true);
	});
	MissionGameLayer->addChild(YesButtonMission, 3);
	auto NoButtonMission = ui::Button::create("UI/btnNo.png", "UI/btnNoPress.png");
	NoButtonMission->addClickEventListener([&](Ref* event) {
		MissionGameLayer->setVisible(false);
		GamePlayMenuBtn->setVisible(true);
		GamePlayMenuBg->setVisible(true);
	});
	NoButtonMission->setPosition(Vec2(noteMission->getPosition().x + noteMission->getContentSize().width / 3, noteMission->getPosition().y - noteMission->getContentSize().height / 3));
	NoButtonMission->setScale(1.2);
	MissionGameLayer->addChild(NoButtonMission, 3);

	auto noteQuiLevel = Sprite::create("UI/back-mainmenu.png");
	noteQuiLevel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
	noteQuiLevel->setScale(1.5);
	QuitLevelLayer->addChild(noteQuiLevel, 4);
	auto YesQuitLevel = ui::Button::create("UI/btnYes.png", "UI/btnYesPress.png");
	YesQuitLevel->setPosition(Vec2(noteMission->getPosition().x - noteMission->getContentSize().width / 3, noteMission->getPosition().y - noteMission->getContentSize().height / 3));
	YesQuitLevel->setScale(1.2);
	YesQuitLevel->addClickEventListener([&](Ref* event){
		Director::getInstance()->resume();// phai resum toan bo chuong trinh truong truoc khi quay ve mainmenuscene
		Director::getInstance()->replaceScene(MainMenuScene::create());
		//Director::getInstance()->resume();
		//Director::getInstance()->replaceScene(GamePlayScene::createScene(1));
		menuButton->setEnabled(true);
		replayButton->setEnabled(true);
	});
	QuitLevelLayer->addChild(YesQuitLevel, 4);
	auto NoButtonQuit = ui::Button::create("UI/btnNo.png", "UI/btnNoPress.png");
	NoButtonQuit->addClickEventListener([&](Ref* event) {
		QuitLevelLayer->setVisible(false);
		GamePlayMenuBtn->setVisible(true);
		GamePlayMenuBg->setVisible(true);
	});
	NoButtonQuit->setPosition(Vec2(noteMission->getPosition().x + noteMission->getContentSize().width / 3, noteMission->getPosition().y - noteMission->getContentSize().height / 3));
	NoButtonQuit->setScale(1.2);
	QuitLevelLayer->addChild(NoButtonQuit, 4);
}
void UI::createSettingLayer()
{
	auto bgSetting = Sprite::create("mission-bg.png");
	bgSetting->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
	SettingGameLayer->addChild(bgSetting);

	myAudioGame = Label::createWithTTF("Audio:", "fonts/kenvector_future_thin.ttf", 40);
	myAudioGame->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2 + MenuBg->getContentSize().height / 4));
	SettingGameLayer->addChild(myAudioGame, 3);
	//myAudioGame->setScale(0.5);
	audioSliderSetting = ui::Slider::create();
	audioSliderSetting->loadBarTexture("UI/slider_bar_bg.png");
	audioSliderSetting->loadSlidBallTextures("UI/slider_ball_normal.png", "unit/plane2.png", "UI/slider_ball_disable.png");

	audioSliderSetting->loadProgressBarTexture("UI/slider_bar_pressed.png");
	audioSliderSetting->setPercent(GameData::getInstance()->getVolume());
	sliderPercentGame = audioSliderSetting->getPercent();
	audioSliderSetting->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));

	audioSliderSetting->addEventListener(CC_CALLBACK_0(UI::ChangedSliderGame, this));

	SettingGameLayer->addChild(audioSliderSetting);

	textSettingPer = Label::createWithTTF("", "fonts/kenvector_future_thin.ttf", 40);
	textSettingPer->setString(StringUtils::format("%d %c", audioSliderSetting->getPercent(), '%'));
	//textPercent->setInsertText(percent);
	textSettingPer->setPosition(Vec2(audioSliderSetting->getPosition().x, audioSliderSetting->getPosition().y - audioSliderSetting->getContentSize().height * 2));
	SettingGameLayer->addChild(textSettingPer);


	//audioSliderSetting->setScale(0.5);
	auto backMenu = ui::Button::create("UI/backArrow.png");
	backMenu->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - 4 * bgSetting->getContentSize().width / 10, Director::getInstance()->getVisibleSize().height / 2 - 4 * bgSetting->getContentSize().height / 10));
	backMenu->setScale(0.5);
	SettingGameLayer->addChild(backMenu);
	backMenu->addClickEventListener([&](Ref*)
	{
		SettingGameLayer->setVisible(false);
		GamePlayMenuBtn->setVisible(true);
		GamePlayMenuBg->setVisible(true);

	});

	muteSt = ui::Button::create("UI/mute.png");
	muteSt->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + 4 * bgSetting->getContentSize().width / 10, Director::getInstance()->getVisibleSize().height / 2 - 4 * bgSetting->getContentSize().height / 10));
	muteSt->setVisible(false);
	muteSt->setScale(1.5);
	SettingGameLayer->addChild(muteSt);
	notMuteSt = ui::Button::create("UI/notMute.png");
	notMuteSt->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + 4 * bgSetting->getContentSize().width / 10, Director::getInstance()->getVisibleSize().height / 2 - 4 * bgSetting->getContentSize().height / 10));
	notMuteSt->setVisible(true);
	notMuteSt->setScale(1.5);
	SettingGameLayer->addChild(notMuteSt);
	muteSt->addClickEventListener(CC_CALLBACK_0(UI::MuteSt, this));
	notMuteSt->addClickEventListener(CC_CALLBACK_0(UI::MuteSt, this));

}

void UI::MuteSt()
{
	if (notMuteSt->isVisible())
	{
		muteSt->setVisible(true);
		notMuteSt->setVisible(false);
		audioSliderSetting->setPercent(0);
		GameData::getInstance()->setVolume(audioSliderSetting->getPercent());
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(audioSliderSetting->getPercent());
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(audioSliderSetting->getPercent());
		textSettingPer->setString(StringUtils::format("%d %c", 0, '%'));

	}
	else
	{
		muteSt->setVisible(false);
		notMuteSt->setVisible(true);
		audioSliderSetting->setPercent(sliderPercentGame);
		GameData::getInstance()->setVolume(audioSliderSetting->getPercent());
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(audioSliderSetting->getPercent());
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(audioSliderSetting->getPercent());
		textSettingPer->setString(StringUtils::format("%d %c", audioSliderSetting->getPercent(), '%'));

	}
}

int UI::checkSkillUsed(Vec2 pos){
	if (skillHero1->getBoundingBox().containsPoint(pos)){
		return 1;
	}
	if (skillGame1->getBoundingBox().containsPoint(pos)){
		return 2;
	}
	return 0;
}

Label* UI::getSkillHeroLabel(){
	return skillHeroTime;
}

Label* UI::getSkillGameLabel(){
	return skillGameTime;
}

bool UI::isMenuOpen(){
	return menuOpened;
}
