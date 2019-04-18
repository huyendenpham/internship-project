#pragma once
#include "cocos2d.h"
USING_NS_CC;
class _Object
{
public:
	_Object();
	~_Object();
	Sprite *getSprite();
	/*virtual void update(float) = 0;*/
	float getPositionX();
	float getPositionY();
	void setPosition(float x, float y);
	bool getVisible();
	void setVisible(bool v);
	Size getSize();
	void setName(char *name);
	char *getName();
	/////////////////////////////////////////
	// math event
	float getCorner(float xa, float ya, float xb, float yb);
	float getCorner2(float xa, float ya, float xb, float yb);
	float getDistance(float xa, float ya, float xb, float yb);
protected:
	Sprite *m_sprite;
	
	//Vec2 m_position;
};

