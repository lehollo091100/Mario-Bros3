#pragma once
#include"GameObject.h"
#define BBOX_WIDTH	17
#define STATE_STAR	100
#define STATE_MUSHROOM	200
#define STATE_PLANT	300
#define STATE_COLLISION	400
#define STATE_STAR_UP	500
#define STATE_MUSHROOM_UP	600
#define STATE_PLANT_UP	700
#define STATE_BOARD		800
#define MAX_TIME	500
#define STAR_ANI	0
#define MUSHROOM_ANI	1
#define PLANT_ANI	2
#define STAR_UP_ANI	3
#define MUSHROOM_UP_ANI	4
#define PLANT_UP_ANI	5
#define BOARD_ANI	6
#define SPEED_UP	0.05f
#define RANGE	30


class EndGameObject:public CGameObject
{
	float STARTY;
public:
	EndGameObject() {
		STARTY = 0;
		type=GType::ENDGAMEITEM;
		time = 0;
		SetHealth(2);
		health = 2;
		this->vx = this->vy = 0;
		SetState(STATE_STAR);
		boardtime = 0;
	};
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	DWORD time;
	DWORD boardtime;
	//int time;
};

