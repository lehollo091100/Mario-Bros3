#include "Tail.h"
#include "Koopas.h"
Tail::Tail()
{
	health = 1;
	vx = vy = 0;
	state = TAIL_STATE_NORMAL;

}
void Tail::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	
}
void Tail::Render()
{
	RenderBoundingBox();
}
void Tail::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{

	default:
		break;
	}
}
void Tail::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = l + TAILLENGHT;
	b = t + TAILHEIGHT;
}