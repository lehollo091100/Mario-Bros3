#pragma once
#include"GameObject.h"
class ColorBrick : public CGameObject
{
public:
	float height, width;
	float X;

public:
	ColorBrick(float W,float H,float x)
	{
		width = W;
		height = H;
		type = GType::COLORBRICK;
		X = x;
		health = 1;
		IsMovingObj = false;
	}
	float GetWidth() {
		return width;
	}
	float GetXOfColorBrick() {
		return X;
	}
	;

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

