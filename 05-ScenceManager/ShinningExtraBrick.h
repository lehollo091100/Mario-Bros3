#pragma once
#include"GameObject.h"
#include"CoinInBrick.h"
#include"Mario.h"
#include"Leaf.h"
#define EXTRABRICK_STATE_NORMAL	100
#define EXTRABRICK_STATE_COLLISION	200
#define EXTRABRICK_STATE_NOTHINGLEFT	300
#define EXTRABRICK_ANI_NORMAL	0
#define EXTRABRICK_ANI_NOTHINGLEFT	1
#define EXTRABIRCK_MAX_HEIGHT	5
#define BRICK_SPEED_Y	0.05
#define EXTRABRICK_BBOX_WIDTH  16
#define EXTRABRICK_BBOX_HEIGHT 16

class ShinningExtraBrick:public CGameObject
{
public:
	int startY;
	int number;
	int kind;
	vector< LPCOININBRICK> listcoin;
	vector<LPLEAF> listleaf;
public:
	ShinningExtraBrick(int num_coin,int kind) {
		this->number = num_coin;
		this->kind = kind;
		vy = 0;
		type = GType::SHINNINGEXTRABRICK;
		SetHealth(1);
		health = 1;
		IsDie = false;
		state = EXTRABRICK_STATE_NORMAL;
		IsMovingObj = false;
	};
	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

