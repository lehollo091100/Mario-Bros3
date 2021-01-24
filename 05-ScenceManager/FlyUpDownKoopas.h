#pragma once
#include"GameObject.h"
#define KOOPAS_VY	0.03f
#define FLY_STATE	100
#define DISAPPEAR_STATE	200
#define RANGE		400
#define BBOX_WIDTH	16
#define BBOX_HEIGHT	25
#define RANGE	40
class FlyUpDownKoopas:public CGameObject
{
	float startY;
public:
	FlyUpDownKoopas() {

		type = GType::FLYUPDOWNKOOPAS;
		SetHealth(1);
		ny = 1;
		this->vy = ny*KOOPAS_VY;
		SetState(FLY_STATE);

	};
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

