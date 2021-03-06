#include "GameObject.h"
#include "Utils.h"

#define GOOMBA_WALKING_SPEED 0.05f;

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define ITEM_STATE_WALKING 100
#define ITEM_STATE_DIE 200

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1



class CItem : public CGameObject
{ 
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	DWORD diestart;
	int time;
	

public:
	bool IsWalking, Isdie;
	CItem();
	virtual void SetState(int state);
};

