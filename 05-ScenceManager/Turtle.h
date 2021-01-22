#pragma once
#include"GameObject.h"
#include"Boomerang.h"
#define TURTLE_STATE_WALKING	100
#define TURTLE_STATE_ATTACK		200
#define TURTLE_ANI_WALKING		0
#define TURTLE_ANI_ATTACK		1
#define TURTLE_BBOX_HEIGHT	25
#define TURTLE_BBOX_WIDTH	16
#define RANGE_X		30
#define VX		0.02f
#define GRAVITY	0.0009f

class Turtle:public CGameObject
{
	float startX;
	int a;
	vector<LPBOOMERANG> BoomList;
public:
	Turtle() {
		type = GType::TURTLE;
		SetHealth(1);
		SetState(TURTLE_STATE_WALKING);
		nx = 1;
		vx = nx * VX;
		CBoomerang *boom1 = new CBoomerang();
		CBoomerang *boom2 = new CBoomerang();
		BoomList.push_back(boom1);
		BoomList.push_back(boom2);

	};
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

