#include "Turtle.h"
#include"Utils.h"



void Turtle::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (health == 0)
	{
		IsDie = true;
	}
	if (IsDie)
	{
		return;
	}
	for (int i = 0; i < BoomList.size(); i++)
	{
		//DebugOut(L"size:%d\n", objs.size());
		BoomList[i]->Update(dt,&objs);
	}
	if (startX == 0)
	{
		startX = x;
	}
	if (startY == 0)
	{
		startY = y;
	}

	//int a;
	if (state==TURTLE_STATE_DIE)
	{
		vx = VX;
		if (y>=startY+ RANGE_X*3)
		{
			//health = 0;
		}
	}
	if (state!= TURTLE_STATE_DIE)
	{
		/*if ()
		{

		}*/
		if ((x >= startX + RANGE_X) || (x <= startX - RANGE_X))
		{
			nx = -nx;
			vx = nx * VX;
			SetState(TURTLE_STATE_ATTACK);
		}
	}
	
	CGameObject::Update(dt, coObjects);
	vy += GRAVITY/2 * dt; 
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	//DebugOut(L"turtle collid size:%d\n", coEvents.size());
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
		vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			/*if (e->obj->GetType() == GType::BOOMERANG)
			{
				if (e->obj==BoomList[0])
				{
					BoomList[0]->health = 0;
				}
				if (e->obj == BoomList[1])
				{
					BoomList[1]->health = 0;
				}
			}*/
			if (e->nx != 0)
			{
				if (e->obj->GetType() == GType::BRICK)
				{
					x += e->nx*0.4f;
				}
			}
			if (e->ny != 0)
			{
				if (e->obj->GetType() == GType::BRICK)
				{
					y += e->nx*0.4f;
					vy = 0;
				}
			}


		}
	}
	//DebugOut(L"TURTLE HEALTH:%d\n", health);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i) != NULL && coObjects->at(i)->GetHealth() > 0)
			if ((isCollisionWithObj(coObjects->at(i))->t > 0) && isCollisionWithObj(coObjects->at(i))->t < 1)
			{
				switch (coObjects->at(i)->GetType())
				{
					case GType::BRICK: {
						if (state==TURTLE_STATE_DIE&&health==1)
						{
							x += dx;
							y += dy;
							break;
						}
						else
						{
							y += -0.4f;				
						}
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
		//this->state = TURTLE_STATE_WALKING;
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
			BoomList[0]->SetDirection(nx);
			BoomList[0]->SetState(BOOMERANG_STATE_ATTACK);
		}
		/*if (BoomList[1]->GetHealth() == 0)
		{
			BoomList[1]->SetHealth(1);
			BoomList[1]->IsDie = false;
			BoomList[1]->SetPosition(x - 10, y-8);
			BoomList[1]->SetDirection(nx);
			BoomList[1]->SetState(BOOMERANG_STATE_ATTACK);
		}*/
		SetState(TURTLE_STATE_WALKING);
		break;
	}
	case TURTLE_STATE_DIE:
	{
		health = 1;
		this->vy -= VYDIE;
		vx = VX;
		break;
	}
	default:
		break;
	}
}
void Turtle::Render()
{
	if (IsDie)
		return;
	for (int i = 0; i < BoomList.size(); i++)
	{
		if(BoomList[i]->health>0)
			BoomList[i]->Render();
	}
	int ani = 0;
	if (state==TURTLE_STATE_ATTACK)
	{
		ani = TURTLE_ANI_ATTACK;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();

}

void Turtle::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x+ TURTLE_BBOX_WIDTH;
	b = y + TURTLE_BBOX_HEIGHT;
}
