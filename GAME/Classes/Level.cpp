#include "Level.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "Wave.h"

USING_NS_CC;
using namespace rapidjson;

Level::Level(int n)
{
	numoflevel = n;
	Document read_Json_Level;
	ssize_t size;

	char* buf = (char*)FileUtils::getInstance()->getFileData("data/dataLevel.json", "r", &size);
	string content(buf, size);

	read_Json_Level.Parse(content.c_str());
	string nameLevel = StringUtils::format("%d", this->numoflevel);
	this->mapPath = read_Json_Level[nameLevel.c_str()]["MapPath"].GetString();
	this->goldBegin = read_Json_Level[nameLevel.c_str()]["GoldBegin"].GetInt();
	this->livesBegin = read_Json_Level[nameLevel.c_str()]["LivesBegin"].GetInt();
	this->numOfWave = read_Json_Level[nameLevel.c_str()]["NumOfWave"].GetInt();
	for (int i = 0; i < this->numOfWave; i++)
	{
		Wave *wave = new Wave(read_Json_Level[nameLevel.c_str()]["ListWave"][i].GetString());
		this->listWave.push_back(wave);
	}
}
Level::Level()
{
}

int Level::getGold(){
	return goldBegin;
}

int Level::getLives(){
	return livesBegin;
}