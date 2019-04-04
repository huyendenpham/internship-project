#ifndef _WAVE_H_
#define _WAVE_H_
#include <iostream>
#include "Unit.h"

using namespace std;
class Wave
{
private:
	string waveName;
	float delayTimeGround, delayTimeSky;
	int sl1Numb, sl2Numb, sl3Numb, tank1Numb, tank2Numb, plane1Numb, plane2Numb, totalUnit;
public:
	Wave();
	Wave(string waveName);

	vector<Unit*> createListUnitGround(Layer* scene, Vec2 posGround);
	vector<Unit*> createListUnitSky(Layer* scene, Vec2 posSky);

	void decTotalUnit();
	int getTotalUnit();
	void setDelayTimeGround(float delayTime);
	float getDelayTimeGround();
	void setDelayTimeSky(float delayTime);
	float getDelayTimeSky();
	void setSl1Numb(int n);
	int getSl1Numb();
	void setSl2Numb(int n);
	int getSl2Numb();
	void setSl3Numb(int n);
	int getSl3Numb();
	void setTank1Numb(int n);
	int getTank1Numb();
	void setTank2Numb(int n);
	int getTank2Numb();
	void setPlane1Numb(int n);
	int getPlane1Numb();
	void setPlane2Numb(int n);
	int getPlane2Numb();
};
#endif //_WAVE_H_
