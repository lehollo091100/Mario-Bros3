#include "GreenPlant.h"

void GreenPlant::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + GREENPLANT_WIDTH;
	bottom = y + GREENPLANT_HEIGHT;
}

void GreenPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health <= 0)
		IsDie = true;
	if (IsDie)
		return;
	if (startY == 0)
		startY = y;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (startY - y >= GREENPLANT_HEIGHT)
	{
		if (vy < 0) {
			if (IsMoving && !IsAttacking)
			{
				vy = 0;
				IsAttacking = true;
				IsMoving = false;
				//DebugOut(L"line 25");
			}
		}
	}

	if (y >= startY)
	{

		if (vy > 0) {
			if (IsMoving && !IsHidden)
			{
				//DebugOut(L"LINE 33\n");
				vy = 0;
				IsHidden = true;
				IsMoving = false;
			}
		}
		if ((m->x<this->x + 25 && m->x>this->x) || (m->x > this->x - 25 && m->x < this->x))
		{
			//vy = 0;
			IsNextTo = true;
			if (IsMoving)
				statebackup = 1;
			else if (IsHidden)
				statebackup = 0;
		}
		else
		{
			IsNextTo = false;
			//DebugOut(L"NOT NEXT TO\n");
		}

	}
	//DebugOut(L"mx  this->x %f %f:\n", m->x, this->x);

	if (IsHidden && !IsMoving)
	{
		if (StopTime == 0)
		{
			StopTime = GetTickCount64();

		}
		if (GetTickCount64() - StopTime >= GREENLANT_STOPTIME)
		{
			if (IsNextTo)
			{
				vy = 0;
			}
			else {
				StopTime = 0;
				vy = -GREENPLANT_SPEEDY;
				IsHidden = false;
				IsMoving = true;
			}
		}
	}

	if (IsAttacking && !IsMoving)
	{
		//vy = 0;
		if (AttackTime == 0)
		{
			AttackTime = GetTickCount64();
			//DebugOut(L"Line 44\n");
		}
		if (GetTickCount64() - AttackTime >= GREENLANT_STOPTIME)
		{
			//DebugOut(L"LINE 63");
			AttackTime = 0;
			vy = GREENPLANT_SPEEDY;
			IsAttacking = false;
			IsMoving = true;
		}
	}
	CGameObject::Update(dt, coObjects);
	y += dy;
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		//x -= dx;
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
			/*if (e->obj->GetType() == GType::PIPE)
			{
				if (this->vy != 0)
					y += dy;
			}*/

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void GreenPlant::Render()
{
	if (IsDie)
		return;
	int ani = GREENPLANT_ANI_STRAIGHT;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
