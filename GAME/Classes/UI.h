#ifndef __UI_H__
#define __UI_H__


#include <cocos2d.h>
#include <ui\cocosGUI.h>
#include "SneakyJoystickSkinnedBase.h"


class UI : public cocos2d::Layer{

	private:
		bool menuOpened;
		int level;
		SneakyJoystick *leftJoystick;
		Sprite *skillGame1, *skillHero1, *skillGameBg, *skillHeroBg, *towershopAT, *towershopMi, *towershopGun, *MenuBg;
		MenuItem  *pauseButton, *playButton, *x2Button, *menuButton, *replayButton;
		Label *goldLabel, *liveLabel, *nameLabel, *hpLabel, *equipLabel, *myAudioGame, *wave, *skillGameTime, *skillHeroTime;
		ui::LoadingBar *hpBar;
		Layer *GamePlayMenuBg, *GamePlayMenuBtn, *SettingGameLayer, *ReplayGameLayer, *MissionGameLayer, *QuitLevelLayer;
		ui::Button *reSumeGame, *settingGameBtn, *upgradeGame, *missionGame, *quitGamePlay, *muteSt,*notMuteSt;
		ui::Slider *audioSliderSetting;
		int sliderPercentGame;
		char bufSt[50];

	public:
		UI(int level);
		SneakyJoystick* getJoytick();
		Sprite* getAT();
		Sprite* getMi();
		Sprite* getGun();
		Label* getGoldLabel();
		Label* getLiveLabel();
		Label* getNameLabel();
		Label* getWaveLabel();
		Label* getHpLabel();
		Label* getEquipLabel();
		Label *textSettingPer;
		ui::LoadingBar* getHpBar();
		Label* getSkillHeroLabel();
		Label* getSkillGameLabel();

		bool isMenuOpen();



		// check skill func
		int checkSkillUsed(Vec2 pos);

		// menu function
		void createMenuLayer();
		void ResumePlayGame();
		void GamePlayMenu();
		void SettingInGame();
		void readFileSettingGame(std::string path);
		void ChangedSliderGame();
		void ReplayGame();
		int stringToInt(std::string s);
		void createSettingLayer();
		void MuteSt();

};

#endif