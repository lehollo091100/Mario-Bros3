#pragma once

#include "GameObject.h"
#include "Game.h"
#include "Mario.h"


#define KOOPAS_WALKING_SPEED 0.03f
#define KOOPAS_ATTACK_SPEED		0.2f

#define KOOPAS_BBOX_WIDTH		16
#define KOOPAS_BBOX_HEIGHT		26
#define KOOPAS_BBOX_HEIGHT_DIE		16

#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_DEFEND		200
#define KOOPAS_STATE_ATTACK		300

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_DIE 2
#define KOOPAS_LEVEL_NORMAL			1
#define KOOPAS_LEVEL_DEFEND		2
class CKoopas : public CGameObject
{
	int range;
	
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	float StartPX;

public:
	int level;
	bool IsWalking,IsAttacking,IsDefending,IsHeld;
	CMario* mario;
	CKoopas(CMario *m ,int range=50 );
	virtual void SetState(int state);
	int GetState() {
		return state;
	}
	int GetType() { return GType::KOOPAS; }
	void SetLevel(int l)
	{
		level = l;
	}
	int GetLevel() { return level; }
	float GetX() {
		return x;
	}
};