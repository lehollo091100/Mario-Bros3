#pragma once

#include "GameObject.h"
#include "Game.h"
#include "Mario.h"


#define FLYKOOPAS_WALKING_SPEED 0.035f
#define FLYKOOPAS_ATTACK_SPEED		0.2f
#define FLYKOOPAS_GRAVITY		0.0009f
#define FLYKOOPAS_DEFLECT_SPEED	0.25f

#define FLYKOOPAS_BBOX_WIDTH		16
#define FLYKOOPAS_BBOX_HEIGHT		26
#define FLYKOOPAS_BBOX_HEIGHT_DIE		16

//#define FLYKOOPAS_STATE_WALKING		100
//#define FLYKOOPAS_STATE_DEFEND		200
//#define FLYKOOPAS_STATE_ATTACK		300
//#define FLYKOOPAS_STATE_UP		400
//#define FLYKOOPAS_STATE_UP_ATTACK	500

#define FLYKOOPAS_STATE_FLY		120
#define FLYKOOPAS_STATE_DISAPPEAR	150

#define FLYKOOPAS_ANI_FLY	0

//#define FLYKOOPAS_LEVEL_NORMAL			1
//#define FLYKOOPAS_LEVEL_DEFEND		2
class CFlyKoopas : public CGameObject
{
public:
	int range;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	float StartPX;

public:
	DWORD time;
	int level;
	bool IsWalking, IsAttacking, IsDefending, IsHeld, IsUp, IsUpAttacking;
	CMario* mario;
	CFlyKoopas(CMario *m, int range = 50);
	virtual void SetState(int state);
	int GetState() {
		return state;
	}
	int GetType() { return GType::FLYKOOPAS; }
	void SetLevel(int l)
	{
		level = l;
	}
	int GetLevel() { return level; }
	float GetX() {
		return x;
	}
};
