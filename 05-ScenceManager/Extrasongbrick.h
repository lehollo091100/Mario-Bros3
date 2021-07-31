#pragma once
#include "GameObject.h"
#include "Utils.h"
#define EXSONGBRICK_STATE_HIDDEN	0
#define EXSONGBRICK_STATE_NORMAL	100
#define EXSONGBRICK_STATE_HITUP		200
#define EXSONGBRICK_STATE_JUMPON	300
#define EXSONGBRICK_STATE_CHANGE	400 //THAY DOI THEO CHIEU X
#define EXSONGBRICK_ANI_HIDDEN	0
#define EXSONGBRICK_ANI_NORMAL	1
#define EXSONGBRICK_WIDTH	16
#define RANGEX	8
#define RANGEYUP	5
#define RANGEYDOWN	10
#define VX	0.1f
#define VY	0.05f
class Extrasongbrick:public CGameObject
{
		float startX, startY;
	public:
		Extrasongbrick();
		void Render();
		void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
		void GetBoundingBox(float &left, float &top, float &right, float &bottom);
		void SetState(int state);
};

