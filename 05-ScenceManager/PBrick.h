#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Utils.h"
#include"Mario.h"
#include"Goomba.h"
#define CPBRICK_STATE_COLLISION	0
#define CPBRICK_STATE_NORMAL	1
#define CPBRICK_STATE_NOTHINGLEFT	2
#define CPBRICK_ANI_NORMAL	0
#define CPBRICK_ANI_NOTHINGLEFT	1
#define CPBRICK_BBOX_WIDTH	16

class CPBrick : public CGameObject
{

public:
	CPBrick() {
		type = GType::CPBRICK;
		SetHealth(1);
		SetState(CPBRICK_STATE_NORMAL);
		vy = 0;
	};
	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

