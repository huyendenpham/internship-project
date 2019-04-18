#include "Bullet.h"
#include "Tower.h"
#include "cocos2d.h"
Bullet::Bullet()
{
}
Bullet::Bullet(int type, cocos2d::Layer *scene)
{
	switch (type){
	case 1: // handgun
		m_sprite = cocos2d::Sprite::create("bullet/bullet1.png");
		setSpeed(0.001);
		break;
	case 2: // rifle
		m_sprite = cocos2d::Sprite::create("bullet/bullet2.png");
		setSpeed(0.001);
		break;
	case 3: // sniper
		m_sprite = cocos2d::Sprite::create("bullet/bullet3.png");
		setSpeed(0.0005);
		break;
	case 4: // rpg
		m_sprite = cocos2d::Sprite::create("bullet/bullet4.png");
		setSpeed(0.001);
		break;
	case 5: // machine gun
		m_sprite = cocos2d::Sprite::create("bullet/bullet2.png");
		setSpeed(0.001);
		break;
	case 6: // missile
		m_sprite = cocos2d::Sprite::create("bullet/bullet6.png");
		setSpeed(0.001);
		break;
	case 7: // at cannon
		m_sprite = cocos2d::Sprite::create("bullet/bullet4.png");
		setSpeed(0.001);
		break;
	}
	m_sprite->setVisible(false);
	scene->addChild(m_sprite,1);
}

void Bullet::setSpeed(float speed){
	this->speed = speed;
}

float Bullet::getSpeed(){
	return speed;
}

void Bullet::destroy(){
	m_sprite->removeFromParent();
}