#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
	int width, height;
public:
	CBrick(int W, int H) {
		type = GType::BRICK;
		width = W;
		height = H;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};