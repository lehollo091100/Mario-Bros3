#pragma once
#include"Sprites.h"
#include"Animations.h"
#include"GameObject.h"
class StackSpeed :public CGameObject
{
	//CSprite* sprite;
	//CTextures *textures=CTextures::GetInstance();

public:
	StackSpeed();
	float l, t, r, b;
	int id;
	void Render1(float x, float y, int s);
	virtual void Render() {
		return;
	};
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) { return; };
	~StackSpeed();
};


