#pragma once
#include"GameObject.h"
class ColorBrick : public CGameObject
{
public:
	int height, width;

public:
	ColorBrick(int W,int H)
	{
		width = W;
		height = H;
		type = GType::COLORBRICK;
		
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

