#include "Item.h"
#define GRAVITY	0.008f
#define ZEROZEROFORU	0.04f
#define DEFLECTSPEED	0.009f
CItem::CItem()
{
	SetState(ITEM_STATE_WALKING);
	SetHealth(1);
	type = GType::ITEM;
	nx = -1;
	vx = nx * GOOMBA_WALKING_SPEED;
	Isdie = false;
	time = 0;
	//state = s;

}


void CItem::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (health == 0)
		IsDie = true;
	if (IsDie)
		return;
	CGameObject::Update(dt, coObjects);
	vy += GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	DWORD now = GetTickCount64();
	//time = now - diestart;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	//x += dx;
	//y += dy;

	//DebugOut(L"time:%d\n", time);
	if (IsWalking) {
		//state = GOOMBA_STATE_WALKING;
		if (nx == -1)
			if (x <= 0)
			{
				nx = -nx;
				vx = nx * GOOMBA_WALKING_SPEED;
			}

	}

	if (health == 0)
	{
		if (IsWalking)
		{
			//DebugOut(L"line 50\n");
			IsWalking = false;
		}
	}
	if (Isdie)
	{
		vx = 0;
		//state = GOOMBA_STATE_DIE;
	}



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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == GType::COLORBRICK)
			{
				nx = -nx;
				vx = nx*GOOMBA_WALKING_SPEED;
			}
			if (e->obj->GetType() == GType::GOOMBA)
			{
				if(e->nx!=0)
					x += dx;
				//y += dy;
			}
			if (e->obj->GetType() == GType::SHINNINGEXTRABRICK)
			{
				y += -ny * ZEROZEROFORU;
				vy = -DEFLECTSPEED;
			}
			

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CItem::Render()
{
	if (IsDie)
		return;
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CItem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ITEM_STATE_DIE:
		//diestart = GetTickCount64();
		//DebugOut(L"state goomba die\n");
		Isdie = true;
		break;
	case ITEM_STATE_WALKING:
		IsWalking = true;
		break;
		//vx = -GOOMBA_WALKING_SPEED;
	}
}
void CItem::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;
	bottom = y + GOOMBA_BBOX_HEIGHT;
}

