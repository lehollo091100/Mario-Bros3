#pragma once
#include"GameObject.h"
#define COIN_WIDTH 16
#define COIN_ANI_APPEAR	0
class CBrownCoin:public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
public:
	//virtual void SetState(int state);
	CBrownCoin() {
		SetHealth(1);
		IsDie = false;
		type = GType::BROWNCOIN;
	};
};

