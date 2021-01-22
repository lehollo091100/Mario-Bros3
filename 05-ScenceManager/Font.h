#pragma once
#include"Sprites.h"
#include"Animations.h"
#include"GameObject.h"
class Font:public CGameObject
{
	//CSprite* sprite;
	//CTextures *textures=CTextures::GetInstance();
	
public:
	Font(float x,float y);
	float l, t, r, b;
	int id;
	void Render1(float x, float y, string str);
	virtual void Render() {
		return;
	};
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) { return; };
	~Font();
};

