#include "FirePirahaPlant.h"
#include "Utils.h"


void FirePirahaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health <= 0)
		IsDie = true;
	if (IsDie)
		return;
	//DebugOut(L"startY:%f\n", startY);
	//DebugOut(L"bulletvy:%f\n", bulletvy);
	if (startY == 0)
		startY = y;
	//DebugOut(L"MARIOX:%f\n", marioX);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	CalAttackPos();
	CalAttackZone();
	if (startY - y >= FIREPIRAHAPLANT_HEIGHT)
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
		if ((m->x<this->x+25 && m->x>this->x)||(m->x>this->x-25 && m->x<this->x))
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
		if (GetTickCount64() - StopTime >= FIREPIRAHAPLANT_STOPTIME)
		{
			if (IsNextTo)
			{
				vy = 0;
			}
			else {
				StopTime = 0;
				vy = -FIREPIRANHAPLANT_SPEED_Y;
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
			if (IsInZone)
			{
				//DebugOut(L"Line 44\n");
				SetState(FIREPIRAHAPLANT_ATTACKSTATE);
			}
			AttackTime = GetTickCount64();
			//DebugOut(L"Line 44\n");
		}
		if (GetTickCount64() - AttackTime >= FIREPIRAHAPLANT_STOPTIME)
		{
			//DebugOut(L"LINE 63");
			AttackTime = 0;
			vy = FIREPIRANHAPLANT_SPEED_Y;
			IsAttacking = false;
			IsMoving = true;
		}
	}

	//DebugOut(L"ATTACKTIME:%d\n", AttackTime);

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
	for (int i = 0; i < Lstbullet.size(); i++) {
		Lstbullet[i]->Update(dt, m);
		
	}
}

void FirePirahaPlant::Render()
{
	if (IsDie)
		return;
	int ani = 0;
	if (IsMoving == false)
	{
		if (m->x > x)
		{
			//DebugOut(L"Line 130\n");
			if (m->y < y)
				ani = FIREPIRAHAPLANT_ANI_UPIDLERIGHT;
			else
				ani = FIREPIRAHAPLANT_ANI_DOWNIDLERIGHT;
		}
		else if (m->x < x)
		{
			//DebugOut(L"Line 136\n");
			if (m->y < y)
				ani = FIREPIRAHAPLANT_ANI_UPIDLELEFT;
			else
				ani = FIREPIRAHAPLANT_ANI_DOWNIDLELEFT;
		}
	}
	if (IsMoving) {
		if (m->x > x)
		{
			//DebugOut(L"Line 130\n");
			if (m->y < y)
				ani = FIREPIRAHAPLANT_ANI_UPRIGHT;
			else
				ani = FIREPIRAHAPLANT_ANI_DOWNRIGHT;
		}
		else if (m->x < x)
		{
			//DebugOut(L"Line 136\n");
			if (m->y < y)
				ani = FIREPIRAHAPLANT_ANI_UPLEFT;
			else
				ani = FIREPIRAHAPLANT_ANI_DOWNLEFT;
		}
	}

	for (int i = 0; i < Lstbullet.size(); i++)
	{
		Lstbullet[i]->Render();
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void FirePirahaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIREPIRAHAPLANT_APPEARSTATE:
		vy = -FIREPIRANHAPLANT_SPEED_Y;
		break;
	case FIREPIRAHAPLANT_HIDDENSTATE:
		vy = FIREPIRANHAPLANT_SPEED_Y;
		break;
	case FIREPIRAHAPLANT_ATTACKSTATE:
		if (Lstbullet[0]->GetHealth() == 0)
		{
			//DebugOut(L"state fireplant attack\n");
			Lstbullet[0]->SetHealth(1);
			Lstbullet[0]->SetPosition(this->x, this->y);
			Lstbullet[0]->SetDirection(bulletnx, bulletvx, bulletvy);
			this->state = FIREPIRAHAPLANT_ATTACKSTATE;


		}break;
	default:
		break;
	}
}
void FirePirahaPlant::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + FIREPIRAHAPLANT_WIDTH;

	bottom = top + FIREPIRAHAPLANT_HEIGHT;

}
void FirePirahaPlant::CalAttackPos()
{
	if (m->x > this->x)
	{
		bulletvx = 0.05;
		bulletnx = 1;
	}
	else if (m->x < this->x)
	{
		bulletvx = -0.05;
		bulletnx = -1;
	}
	if (abs(x - m->x) <= 80)
	{
		if (m->y > y)
		{
			bulletvy = 0.025f;
		}
		else
		{
			bulletvy = -BULLETVY_NEAR;
		}
	}
	else {
		if (m->y > y)
		{
			bulletvy = 0.025f;
		}
		else
		{
			bulletvy = -BULLETVY_FAR;
		}
	}


}
void FirePirahaPlant::CalAttackZone()
{
	if (abs(x - m->x) <= FIREPIRANHAPLANT_MAXX_ATK && abs(y - m->y) <= FIREPIRANHAPLANT_MAXY_ATK)
	{
		IsInZone = true;
		//DebugOut(L"true\n ");
	}
	else
	{
		IsInZone = false;
		//DebugOut(L"false\n ");
	}
}