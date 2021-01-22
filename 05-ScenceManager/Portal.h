#pragma once

#include "GameObject.h"
#include"Utils.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height;
	float nextX, nextY;
public:
	CPortal(float l, float t, float r, float b, int scene_id,int nextx,int nexty);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	int GetSceneId() { return scene_id; }
	float GetNextX()
	{
		return nextX;
	}
	float GetNextY()
	{
		return nextY;
	}
};