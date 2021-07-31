#pragma once
#include"GameObject.h"
#include"Boomerang.h"
#include "Mario.h"
#define TURTLE_STATE_WALKING	100
#define TURTLE_STATE_ATTACK		200
#define TURTLE_STATE_DIE	300
#define TURTLE_ANI_WALKING		0
#define TURTLE_ANI_ATTACK		1
#define TURTLE_BBOX_HEIGHT	24
#define TURTLE_BBOX_WIDTH	16
#define RANGE_X		30
#define VX		0.02f
#define GRAVITY	0.01f
#define VYDIE	0.15f

class Turtle : public CGameObject
{
	float startX,startY;
	int a;
	vector<LPBOOMERANG> BoomList;
	vector<LPGAMEOBJECT> objs;
	CMario *mario;
public:
	Turtle(CMario *m) {
		mario = m;
		objs.push_back(this);
		objs.push_back(mario);
		type = GType::TURTLE;
		health = 2;
		SetState(TURTLE_STATE_WALKING);
		state = TURTLE_STATE_WALKING;
		nx = 1;
		startY = 0;
		startX = 0;
		vx = nx * VX;
		IsMovingObj = true;
		CBoomerang *boom1 = new CBoomerang();
		CBoomerang *boom2 = new CBoomerang();
		BoomList.push_back(boom1);
		//BoomList.push_back(boom2);

	};
	 void SetState(int state);
	 void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	 void Render();
	 void GetBoundingBox(float &l, float &t, float &r, float &b);
};

