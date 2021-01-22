#include "CoinInBrick.h"

void CCoinInBrick::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (IsDie == false) {
		left = x+1;
		right = left + COIN_WIDTH;
		top = y;
		bottom = y + COIN_HEIGHT;
	}
}

void CCoinInBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health == 0)
		IsDie = true;
	if (IsDie)
		return;
	
	CGameObject::Update(dt, coObjects);
	y += dy;
	if (startY == 0)
	{
		startY = y;
	}
	if (state == COIN_STATE_UP) {
		if (y <= startY - MAXHEIGHT)
			SetState(COIN_STATE_FALLING);
	}
	if (health == 1) {
		if (IsPoint)
		{
			if (y <= startY - MAXHEIGHT) {
				SubHealth(1);
				IsPoint = false;
			}
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//subhealth
			if (e->ny != 0) {
				/*if (e->obj->GetType() == GType::QUESTIONBRICK)
				{
					IsPoint = true;
					ny = -1;
					this->vy = ny * POINT_UP_SPEED;
					this->SubHealth(1);
				}*/
			}

		}

	}
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i) != NULL && coObjects->at(i)->GetHealth() > 0){
			if ((isCollisionWithObj(coObjects->at(i))->t > 0) && isCollisionWithObj(coObjects->at(i))->t < 1)
			{
				switch (coObjects->at(i)->GetType())
				{
				case GType::QUESTIONBRICK:
				{
					this->SetState(COIN_STATE_COLLISION);
					health--;
					//COIN_STATE_COLLISION
					break;
				}
				case GType::SHINNINGEXTRABRICK:
				{
					this->SetState(COIN_STATE_COLLISION);
					health--;
					//COIN_STATE_COLLISION
					break;
				}
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut(L"coinhealth:%d\n", health);
}

void CCoinInBrick::Render()
{
	if (health <= 0)
		return;
	int ani = 0;
	if(!IsPoint)
		 ani = COIN_ANI_APPEAR;
	if (IsPoint)
	{
		ani = POINT_100_ANI;
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CCoinInBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_UP:
		//diestart = GetTickCount64();
		//DebugOut(L"state goomba die\n");
		this->state = COIN_STATE_UP;
		IsUp = true;
		vy = -SPEEDUP;
		break;
	case COIN_STATE_FALLING:
		ny = 1;
		vy = ny * SPEEDDOWN;
		this->state = COIN_STATE_FALLING;
		IsFalling = true;
		IsUp = false;
		break;
		//vx = -GOOMBA_WALKING_SPEED;
	case COIN_STATE_COLLISION:
		this->state = COIN_STATE_COLLISION;
		ny = -1;
		vy = ny * POINT_UP_SPEED;
		IsFalling = false;
		IsPoint = true;
		//SubHealth(1);
		break;
	}
}
