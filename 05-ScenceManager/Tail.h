#pragma once
#include"GameObject.h" 
#include "Utils.h"
#include "Game.h"
#define TAILLENGHT	12
#define TAILHEIGHT	6
#define TAIL_STATE_NORMAL	0
#define TAIL_STATE_ATTACK	100
#define EIGHTTEEN	18
class Tail: public CGameObject
{
public:
	Tail();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void SetState(int state);
	virtual void Render();
	~Tail() {}
};
typedef Tail *LPTAIL;
