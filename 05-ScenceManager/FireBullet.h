#pragma once
#include"GameObject.h" 
#include "Utils.h"
#include "Game.h"
//#define FIRE_GRAVITY			0.0015f
#define FIRE_ANI_SHOOT			0
#define FIRE_BULLET_BBOX	10
class FireBullet : public CGameObject
{
private:
	int ani;
public:
	FireBullet();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, LPGAMEOBJECT mario);
	void SetDirection(int d,float v1,float v2) {
		nx = d;
		//vx = nx * 0.19f;
		this->vx = v1;
		this->vy = v2;

	}
	virtual void Render();
	~FireBullet() {}
};
typedef FireBullet *LPFireBullet;