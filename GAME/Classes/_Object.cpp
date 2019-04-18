#include "_Object.h"
#define _PI_ 3.141592654

_Object::_Object()
{
}

Sprite *_Object::getSprite()
{
	return m_sprite;
}


float _Object::getPositionX() {
	return m_sprite->getPositionX();
}

float _Object::getPositionY() {
	return m_sprite->getPositionY();
}

void _Object::setPosition(float x, float y) {
	m_sprite->setPosition(x,y);
}

bool _Object::getVisible() {
	return m_sprite->isVisible();
}

void _Object::setVisible(bool v) {
	m_sprite->setVisible(v);
}

Size _Object::getSize() {
	return m_sprite->getContentSize();
}
float _Object::getCorner(float xa, float ya, float xb, float yb)
{
	float dx = xb - xa;
	float dy = yb - ya;
	float val = 180.0 / _PI_;
	float conrner = dy / dx;
	//CCLOG("%d,%f,%f", i++,conrner, atan(conrner)*val);
	//if(yb>ya)
	return -atan(conrner)*val;
}
float _Object::getCorner2(float xa, float ya, float xb, float yb)
{
	float dx = xb - xa;
	float dy = yb - ya;
	float val = 180.0 / _PI_;
	float conrner = dy / dx;
	//CCLOG("%d,%f,%f", i++,conrner, atan(conrner)*val);
	//if(yb>ya)
	if (xb>xa)
		return -atan(conrner)*val;
	else
		return 180 - atan(conrner)*val;
}
float _Object::getDistance(float xa, float ya, float xb, float yb)
{
	float dx = xb - xa;
	float dy = yb - ya;
	return sqrt(dx*dx + dy * dy);
}
_Object::~_Object()
{
}
