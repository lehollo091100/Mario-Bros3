#pragma once

#include "GameObject.h"
#include "Game.h"
#include "Mario.h"


#define KOOPASBROWN_WALKING_SPEED 0.03f
#define KOOPASBROWN_ATTACK_SPEED		0.2f

#define KOOPASBROWN_BBOX_WIDTH		16
#define KOOPASBROWN_BBOX_HEIGHT		26
#define KOOPASBROWN_BBOX_HEIGHT_DIE		16

#define KOOPASBROWN_STATE_WALKING		100
#define KOOPASBROWN_STATE_DEFEND		200
#define KOOPASBROWN_STATE_ATTACK		300
#define KOOPASBROWN_STATE_UP		400
#define KOOPASBROWN_STATE_UP_ATTACK	500

#define KOOPASBROWN_ANI_WALKING_LEFT 0
#define KOOPASBROWN_ANI_WALKING_RIGHT 1
#define KOOPASBROWN_ANI_DIE 2
#define KOOPASBROWN_ANI_DIE_ATTACK	3
#define KOOPASBROWN_ANI_DEFEND	4
#define KOOPASBROWN_ANI_ATTACK	5


#define KOOPASBROWN_LEVEL_NORMAL			1
#define KOOPASBROWN_LEVEL_DEFEND		2
class CKoopasBrown : public CGameObject
{
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
	CKoopasBrown(CMario *m, int range = 150);
	virtual void SetState(int state);
	int GetState() {
		return state;
	}
	int GetType() { return GType::KOOPASBROWN; }
	void SetLevel(int l)
	{
		level = l;
	}
	int GetLevel() { return level; }
	float GetX() {
		return x;
	}
};