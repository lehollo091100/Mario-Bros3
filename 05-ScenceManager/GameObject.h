#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Base.h"
#include "Sprites.h"
#include "Animations.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject;
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:
	GType type;
	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;
	int ny;
	int state;
	bool IsDie;
	DWORD dt;

	LPANIMATION_SET animation_set;
	int health;
	int HiddenItem;
	float EnemyX, EnemyY;
	bool IsInGrid;
	bool IsMovingObj;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	int GetState() { return this->state; }
	void SubHealth(int dm) { health -= dm; }
	void PlusHealth(int dm) {
		health += dm;
	}
	void SetHealth(int h) {
		health = h;
	}
	/*void SetX(float x) {
		this->x = x;
	}*/
	float GetX() { return this->x; }
	float GetY() { return this->y; }
	int GetHealth() { return this->health; }
	void RenderBoundingBox();
	virtual void SetDirection(int d) { nx = d;}
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	GType GetType() { return type; }
	void SetType(GType t) {
		type = t;
	}
	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents,
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny,
		float &rdx,
		float &rdy);
	LPCOLLISIONEVENT isCollisionWithObj(CGameObject* obj);
	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	

	~CGameObject();
};

