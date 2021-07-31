#include "CounterBrick.h"
CounterBrick::CounterBrick(int t) {
	type = GType::COUNTERBRICK;
	health = 1;
	startX = 0;
	state = COUNTERBRICK_STATE_NORMAL;
	width = COUNTERBRICK_WIDTH;
	nx = 1;
	HiddenItem = t;
	IsMovingObj = false;
}
void CounterBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) {
	CGameObject::Update(dt);
	x += dx;
	if (startX == 0)
	{
		startX = x;
	}
	if (state == COUNTERBRICK_STATE_CHANGE)
	{
		
			if (x - startX >= RANGE || startX-x>=RANGE)
			{
				vx = -nx * COUNTERBRICK_SPEED;

			}
			if (vx*nx<0)
			{
				if ((nx > 0 && x <= startX)|| (nx < 0 && x >= startX))
				{
						SetState(COUNTERBRICK_STATE_NORMAL);
				}
			}
		
	}
}
void CounterBrick::Render() {
	int ani = COUNTERBRICK_ANI_NORMAL;
	if (state == COUNTERBRICK_STATE_CHANGE) {
		ani = COUNTERBRICK_ANI_BLANK;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void CounterBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case COUNTERBRICK_STATE_NORMAL:
	{
		vx = 0;
		if (startX != x)
		{
			x = startX;
		}
		break;
	}
	case COUNTERBRICK_STATE_CHANGE:
	{
		vx = nx * COUNTERBRICK_SPEED;
		break;
	}
	default:
		break;
	}
}
void CounterBrick::GetBoundingBox(float &l, float &t, float &r, float &b) {
	l = x+1;
	t = y;
	r = x + width;
	b = y + width;
}