#pragma once
#include "GameObject.h"
#define PIPE_ANI_SMALL	0
#define PIPE_ANI_MEDIUM	1
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CPipe : public CGameObject
{
public:
	int width, height,Ani ;
	int IsPortal;
public:
	CPipe(int W, int H, int ani,int isportal) {
		type = GType::PIPE;
		width = W;
		height = H;
		Ani = ani;
		IsPortal = isportal;
		health = 1;
		IsMovingObj = false;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};