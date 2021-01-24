#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Utils.h"
#include"Mario.h"
#include"Goomba.h"
#define QBRICK_BBOX_WIDTH  16
#define QBRICK_BBOX_HEIGHT 16
#define ANI_BRICK1	0
#define ANI_BRICK2	1

#define BRICK_SPEED_Y	0.05
#define MAX_HIGH	5
#define BRICK_STATE_NORMAL 1
#define SBRICK_STATE_NOTHINGLEFT 2
#define SBRICK_STATE_DISAPPEAR	3
#define SBRICK_STATE_TURN_COIN	4

class CShinningBrick : public CGameObject
{
	int width, height, startY;
	bool StartDisappear;
	int num;
	DWORD time;
	float x1, x2, x3, x4, y1, y2, y3, y4;
	float vx1, vx2, vx3, vx4, vy1, vy2, vy3, vy4;
public:
	CShinningBrick();
	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

