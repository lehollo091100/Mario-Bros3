#include "Turtle.h"
#include"Utils.h"



void Turtle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < BoomList.size(); i++)
	{
		BoomList[i]->Update(dt, coObjects);
	}
	if (startX == 0)
	{
		startX = x;
	}
	//int a;
	if (this->x > startX + RANGE_X || this->x < startX - RANGE_X)
	{
		nx = -nx;
		vx = nx * VX;
		SetState(TURTLE_STATE_ATTACK);
		a+=1;
		
	}
	CGameObject::Update(dt, coObjects);
	x += dx;
	vy += GRAVITY * dt;
	//y += dy;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	coEventsResult.clear();




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
		//if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == GType::BOOMERANG)
			{
				if (e->obj==BoomList[0])
				{
					BoomList[0]->health = 0;
				}
				if (e->obj == BoomList[1])
				{
					BoomList[1]->health = 0;
				}
			}

		}
	}
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i) != NULL && coObjects->at(i)->GetHealth() > 0)
			if ((isCollisionWithObj(coObjects->at(i))->t > 0) && isCollisionWithObj(coObjects->at(i))->t < 1)
			{
				switch (coObjects->at(i)->GetType())
				{
				case GType::BOOMERANG:
				{
					if (state == BOOMERANG_STATE_MOVING_BACK)
					{

						/*if (coObjects->at(i) == BoomList[0])
						{
							BoomList[0]->SetHealth(0);
						}
						if (coObjects->at(i) == BoomList[1])
						{
							BoomList[1]->SetHealth(0);
						}*/
					}
					break;
				}
				}
			}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
	//DebugOut(L"health1:%d\n", BoomList[0]->health);
	//DebugOut(L"health2:%d\n", BoomList[1]->health);
}
void Turtle::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TURTLE_STATE_WALKING:
	{
		this->state = TURTLE_STATE_WALKING;
		break;
	}
	case TURTLE_STATE_ATTACK:
	{
		this->state = TURTLE_STATE_ATTACK;
		if (BoomList[0]->GetHealth() == 0)
		{
			BoomList[0]->SetHealth(1);
			BoomList[0]->IsDie = false;
			BoomList[0]->SetPosition(x - 10, y - 8);
			//DebugOut(L"num:%d\n", a);
			//BoomList[0]->SetDirection(1);
		}
		else if (BoomList[1]->GetHealth() == 0)
		{
			BoomList[1]->SetHealth(1);
			BoomList[0]->IsDie = false;
			BoomList[1]->SetPosition(x - 10, y-8);
			//BoomList[1]->SetDirection(nx);
		}
		//SetState(TURTLE_STATE_WALKING);
		break;
	}
	default:
		break;
	}
}
void Turtle::Render()
{
	for (int i = 0; i < BoomList.size(); i++)
	{
		if(BoomList[i]->health>0)
			BoomList[i]->Render();
	}
	int ani = 0;
	/*if (state==TURTLE_STATE_ATTACK)
	{
		ani = TURTLE_ANI_ATTACK;
	}*/
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();

}

void Turtle::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = l + TURTLE_BBOX_WIDTH;
	b = t + TURTLE_BBOX_HEIGHT;
}
