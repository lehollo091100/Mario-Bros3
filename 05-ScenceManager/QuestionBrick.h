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
#define BRICK_STATE_COLISSION 1
#define BRICK_STATE_NOTHINGLEFT 2

#define QUESTIONBRICK_TYPE_COIN	0
#define QUESTIONBRICK_TYPE_MOVING	1

#define TYPE_COIN	0
#define TYPE_LEAF	1
#define TYPE_MUSHROOM	2

class CQuestionBrick : public CGameObject
{
public:
	int width, height,startY;
	int bricktype;
	int typeitem;
	
public:
	CQuestionBrick(int BType);
	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};