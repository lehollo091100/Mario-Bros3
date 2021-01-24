#pragma once
#include"Sprites.h"
#include"Game.h"
#include"GameObject.h"
#include"Textures.h"
class BrokenBrick :public CGameObject
{
	float  x2, x3, x4, y2, y3, y4;
	float vx, vy, vx2, vy2, vx3, vy3, vx4, vy4;
	CTextures* tex;
	CSprite* sprite;
	CSprite* sprite2; CSprite* sprite3;
	CSprite* sprite4;
public:
	BrokenBrick(float X, float Y);
	virtual void Render();
	virtual void Update(DWORD dt);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b) {
		return;
	};
	~BrokenBrick() {};
	//virtual
};

