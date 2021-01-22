#include "EndGameObject.h"
#include "Utils.h"

void EndGameObject::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (health==2)
	{
		top = y;
		left = x;
		right = left + BBOX_WIDTH;
		bottom = top + BBOX_WIDTH;
	}
	else
	{
		return;
	}
}

void EndGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health<=0)
	{
		IsDie = true;
	}
	
	if (y<=230)
	{
		health = 0;
	}
	CGameObject::Update(dt, coObjects);
	y += dy;
	if (health == 2)
	{
		if (state == STATE_STAR)
		{
			if (time == 0)
				time = GetTickCount64();
		}
		if (state == STATE_MUSHROOM)
		{
			if (time == 0)
				time = GetTickCount64();
		}
		if (state == STATE_PLANT)
		{
			if (time == 0)
				time = GetTickCount64();
		}
		if (GetTickCount64() - time >= MAX_TIME)
		{
			time = 0;
			if (state == STATE_STAR)
				SetState(STATE_MUSHROOM);
			else if (state == STATE_MUSHROOM)
				SetState(STATE_PLANT);
			else if (state == STATE_PLANT)
				SetState(STATE_STAR);
		}
	}

}

void EndGameObject::Render()
{
	if (health <= 0)
		return;
	int ani = 0;
	if (health == 2) {
		if (state == STATE_STAR)
			ani = STAR_ANI;
		if (state == STATE_MUSHROOM)
		{
			ani = MUSHROOM_ANI;
		}
		if (state == STATE_PLANT)
		{
			ani = PLANT_ANI;
		}
	}
	if (health == 1)
	{
		if (state == STATE_STAR_UP)
			ani = STAR_UP_ANI;
		if (state == STATE_MUSHROOM_UP)
		{
			ani = MUSHROOM_UP_ANI;
		}
		if (state == STATE_PLANT_UP)
		{
			ani = PLANT_UP_ANI;
		}
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void EndGameObject::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_STAR:
	{
		this->state = STATE_STAR;
		break;
	}
	case STATE_MUSHROOM:
	{
		this->state = STATE_MUSHROOM;
		break;
	}
	case STATE_PLANT:
	{
		this->state = STATE_PLANT;
		break;
	}
	case STATE_STAR_UP:
	{
		ny = -1;
		vy = ny * SPEED_UP;
		SubHealth(1);
		this->state = STATE_STAR_UP;
		break;
	}
	case STATE_MUSHROOM_UP:
	{
		ny = -1;
		vy = ny * SPEED_UP;
		SubHealth(1);
		this->state = STATE_MUSHROOM_UP;
		break;
	}
	case STATE_PLANT_UP:
	{
		ny = -1;
		vy = ny * SPEED_UP;
		SubHealth(1);
		this->state = STATE_PLANT_UP;
		break;
	}
	default:
		break;
	}
}
