#include "Wave.h"
#include <iostream>
#include "json/rapidjson.h"
#include "json/document.h"
#include "cocos2d.h"


USING_NS_CC;
using namespace std;
using namespace rapidjson;

Wave::Wave(string waveName)
{
	this->waveName = waveName;

	Document read_Json_Wave;
	ssize_t size2;
	char* buf2 = (char*)FileUtils::getInstance()->getFileData("data/dataWave.json", "r", &size2);
	string content2(buf2, size2);
	read_Json_Wave.Parse(content2.c_str());

	totalUnit = 0;
	setDelayTimeGround(read_Json_Wave[this->waveName.c_str()]["delayTimeGround"].GetFloat());
	setDelayTimeSky(read_Json_Wave[this->waveName.c_str()]["delayTimeSky"].GetFloat());

	setSl1Numb(read_Json_Wave[this->waveName.c_str()]["sl1Numb"].GetInt());
	setSl2Numb(read_Json_Wave[this->waveName.c_str()]["sl2Numb"].GetInt());
	setSl3Numb(read_Json_Wave[this->waveName.c_str()]["sl3Numb"].GetInt());
	setTank1Numb(read_Json_Wave[this->waveName.c_str()]["tank1Numb"].GetInt());
	setTank2Numb(read_Json_Wave[this->waveName.c_str()]["tank2Numb"].GetInt());
	setPlane1Numb(read_Json_Wave[this->waveName.c_str()]["plane1Numb"].GetInt());
	setPlane2Numb(read_Json_Wave[this->waveName.c_str()]["plane2Numb"].GetInt());
}

Wave::Wave()
{

}

void Wave::decTotalUnit(){
	this->totalUnit--;
}
int Wave::getTotalUnit(){
	return totalUnit;
}

void Wave::setDelayTimeGround(float delayTime){
	this->delayTimeGround = delayTime;
}
float Wave::getDelayTimeGround(){
	return delayTimeGround;
}
void Wave::setDelayTimeSky(float delayTime){
	this->delayTimeSky = delayTime;
}
float Wave::getDelayTimeSky(){
	return delayTimeSky;
}
void Wave::setSl1Numb(int n){
	this->sl1Numb = n;
}
int Wave::getSl1Numb(){
	return sl1Numb;
}
void Wave::setSl2Numb(int n){
	this->sl2Numb = n;
}
int Wave::getSl2Numb(){
	return sl2Numb;
}
void Wave::setSl3Numb(int n){
	this->sl3Numb = n;
}
int Wave::getSl3Numb(){
	return sl3Numb;
}
void Wave::setTank1Numb(int n){
	this->tank1Numb = n;
}
int Wave::getTank1Numb(){
	return tank1Numb;
}
void Wave::setTank2Numb(int n){
	this->tank2Numb = n;
}
int Wave::getTank2Numb(){
	return tank2Numb;
}
void Wave::setPlane1Numb(int n){
	this->plane1Numb = n;
}
int Wave::getPlane1Numb(){
	return plane1Numb;
}
void Wave::setPlane2Numb(int n){
	this->plane2Numb = n;
}
int Wave::getPlane2Numb(){
	return plane2Numb;
}

vector<Unit*> Wave::createListUnitGround(Layer* scene, Vec2 posGround){
	vector<Unit*> temp;
	for (int i = 0; i < sl1Numb; i++) {
		auto unit = new Unit(scene, "sl1");
		unit->setPos(posGround);
		temp.push_back(unit);
		totalUnit++;
	}
	for (int i = 0; i < sl2Numb; i++) {
		auto unit = new Unit(scene, "sl2");
		unit->setPos(posGround);
		temp.push_back(unit);
		totalUnit++;
	}
	for (int i = 0; i < sl3Numb; i++) {
		auto unit = new Unit(scene, "sl3");
		unit->setPos(posGround);
		temp.push_back(unit);
		totalUnit++;
	}
	for (int i = 0; i < tank1Numb; i++) {
		auto unit = new Unit(scene, "tank1");
		unit->setPos(posGround);
		temp.push_back(unit);
		totalUnit++;
	}
	for (int i = 0; i < tank2Numb; i++) {
		auto unit = new Unit(scene, "tank2");
		unit->setPos(posGround);
		temp.push_back(unit);
		totalUnit++;
	}

	return temp;
}

vector<Unit*> Wave::createListUnitSky(Layer* scene, Vec2 posSky){
	vector<Unit*> temp;

	for (int i = 0; i < plane1Numb; i++) {
		auto unit = new Unit(scene, "plane1");
		unit->setPos(posSky);
		temp.push_back(unit);
		totalUnit++;
	}
	for (int i = 0; i < plane2Numb; i++) {
		auto unit = new Unit(scene, "plane2");
		unit->setPos(posSky);
		temp.push_back(unit);
		totalUnit++;
	}

	return temp;
}
