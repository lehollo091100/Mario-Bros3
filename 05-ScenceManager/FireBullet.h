#pragma once
#include"GameObject.h" 
#include "Utils.h"
#include "Game.h"
//#define FIRE_GRAVITY			0.0015f
#define FIRE_ANI_SHOOT			0
class FireBullet : public CGameObject
{
private:
	int ani;
public:
	FireBullet(int nx,float vx,float vy);
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void SetDirection(int d) {
		nx = d;
		//vx = nx * 0.19f;
		vy = 0;

	}
	void Render();
	~FireBullet() {}
};
