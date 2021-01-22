#pragma once
#include "Utils.h"
#include"Mario.h"
#include"Goomba.h"
#define PITEM_STATE_NORMAL	0
#define PITEM_STATE_COLLISION	1
#define PITEM_ANI_NORMAL	0
#define PITEM_ANI_NOTHINGLEFT	1
#define PITEM_BBOX_WIDTH	16
class CPItem : public CGameObject
{

public:
	CPItem() {
		type = GType::PITEM;
		SetHealth(1);
		SetState(PITEM_STATE_NORMAL);
		vy = 0;
		
	};
	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

