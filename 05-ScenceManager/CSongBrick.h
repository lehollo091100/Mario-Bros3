#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Utils.h"
#include "Mario.h"
#define SONGBRICK_HITUP_SPEED	0.25
#define SONGBRICK_STATE_NORMAL	100
#define SONGBRICK_STATE_HITUP	200
#define SONGBRICK_STATE_JUMPEDON	300
#define SONGBRICK_SPEED_UP	0.05
#define RANGE	5
class CSongBrick:public CGameObject
{

	 void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	 void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	 void Render();
	 void SetState(int state);
public:
	CMario* mario;
	float width,startY;

	CSongBrick(CMario *mario);
	
};

