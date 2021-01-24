#pragma once
#include"GameObject.h"
#define WOOD_VX	0.02f
#define WOOD_ANI	0
#define WOOD_STATE_NORMAL	100
#define WOOD_STATE_DOWN		200

class CUpDownWood:public CGameObject
{
	int Width, Height;
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	int GetWidth() {
		return this->Width;
	};
	int GetHeight() {
		return this->Height;
	};
	CUpDownWood(int width,int height) {
		Width = width;
		this->Height = height;
		type = GType::UPDOWNWOOD;
		SetHealth(1);
		nx = -1;
		//vx = 0;
		vx = nx * WOOD_VX;
		vy = 0;
		SetState(WOOD_STATE_NORMAL);
	};
};

