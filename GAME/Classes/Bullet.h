#ifndef _BULLET_H_
#define _BULLET_H_
#include "_Object.h"
#include "cocos2d.h"
using namespace std;
class Bullet : public _Object
{
private:
	float speed;
public :
	Bullet();
	Bullet(int type, cocos2d::Layer *scene);
	void setSpeed(float speed);
	float getSpeed();
	void destroy();
};
#endif // _BULLET_H_

