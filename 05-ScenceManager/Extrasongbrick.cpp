#include "Extrasongbrick.h"
Extrasongbrick::Extrasongbrick() {
	type = GType::EXTRASONGBRICK;
	IsMovingObj = false;
	health = 1;
	startX = startY = 0;
	state = EXSONGBRICK_STATE_NORMAL;
	//vy= vx = 0;
	ny = 1;
	nx = 1;
}
void Extrasongbrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (startX == 0)
	{
		startX = x;
	}
	if (startY == 0)
	{
		startY = y;
	}
	//DebugOut(L"startY:%f\n", startY);
	CGameObject::Update(dt,coObjects);
	y += dy;
	x += dx;
	if (state == EXSONGBRICK_STATE_CHANGE)
	{
		if ((x >= startX + RANGEX && nx == 1) || (x <= startX - RANGEX && nx == -1))
		{
			vx = -nx * VX;
		}
		if (vx*nx < 0)
		{
			if ((nx > 0 && x <= startX) || (nx < 0 && x >= startX))
			{
				SetState(EXSONGBRICK_STATE_NORMAL);
			}
		}
	}
	if (state==EXSONGBRICK_STATE_HITUP)
	{
		if (vy < 0)
		{
			if (y <= startY-RANGEYUP)
			{
				vy = VY;
			}
		}
		if (vy>0)
		{
			if (y >= startY)
			{
				SetState(EXSONGBRICK_STATE_NORMAL);
			}
		}
	}
		if (state == EXSONGBRICK_STATE_JUMPON)
		{
			if (vy > 0)
			{
				if (y >= startY+RANGEYDOWN)
				{
					vy = -VY;
				}
			}
			if (vy < 0)
			{
				if (y <= startY)
				{
					SetState(EXSONGBRICK_STATE_NORMAL);
				}
			}
		}

}
void Extrasongbrick::Render() {
	int ani = 1;
	if (state != EXSONGBRICK_STATE_HIDDEN)
	{
		ani = 1;

	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void Extrasongbrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case EXSONGBRICK_STATE_HIDDEN: {
		break;
	}
	case EXSONGBRICK_STATE_NORMAL: {
		vx = 0;
		vy = 0;
		if (x != startX)
		{
			x = startX;
		}
		if (y != startY)
		{
			y = startY;
		}
		break;
	}
	case EXSONGBRICK_STATE_HITUP: {
		vy = -VY;
		break;
	}
	case EXSONGBRICK_STATE_JUMPON: {
		vy = VY;
		break;
	}
	case EXSONGBRICK_STATE_CHANGE: {
		vx = nx * VX;
		break;
	}
	default:
		break;
	}
}
void Extrasongbrick::GetBoundingBox(float &left, float &top, float &right, float &bottom) {
	left = x;
	top = y;
	right = x + EXSONGBRICK_WIDTH;
	bottom = y + EXSONGBRICK_WIDTH;
}