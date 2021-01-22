#pragma once
#include"GameObject.h"
#include"Mario.h"
#define GREENPLANT_HEIGHT	24
#define GREENPLANT_WIDTH	16
#define GREENLANT_STOPTIME	2000
#define GREENPLANT_SPEEDY	0.035f
#define GREENPLANT_ANI_STRAIGHT		0
class GreenPlant:public CGameObject
{
public:
	bool IsHidden, IsAttacking, IsAppearing, IsInZone, IsMoving;
	DWORD AttackTime, MovingTime;
	DWORD StopTime;
	int count;
	float startY;
	float marioX, marioY;
	bool IsNextTo;
	int statebackup;
	CMario*m;
public:
	GreenPlant(CMario *mario)
	{
		m = mario;
		SetHealth(1);
		type = GType::GREENPLANT;
		IsHidden = true;
		IsAttacking = false;
		IsMoving = false;
	}
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	//virtual void SetState(int state);
};

