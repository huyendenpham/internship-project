#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Wave.h"
#include "cocos2d.h"
using namespace std;
class Level
{
private:
	int goldBegin;
	int livesBegin;
public :
	Level();
	Level(int n);
	int numoflevel;
	string mapPath;
	int numOfWave;
	vector<Wave *> listWave;
	int getGold();
	int getLives();
};
#endif //_LEVEL_H