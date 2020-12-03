#include "FirePirahaPlant.h"
#include "Utils.h"


void FirePirahaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"y:%f\n", y);
	//DebugOut(L"startY:%f\n", startY);
	DebugOut(L"bulletvy:%f\n", bulletvy);
	if (startY == 0)
		startY = y;
	//DebugOut(L"MARIOX:%f\n", marioX);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	//DebugOut(L"vy%f\n:", vy);

	//DebugOut(L"VY:%f\n", vy);
	CalAttackPos();
	if (startY - y >= 31)
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
	}

	if (IsHidden && !IsMoving)
	{
		if (StopTime == 0)
		{
			StopTime = GetTickCount64();

		}
		if (GetTickCount64() - StopTime >= 3000)
		{
			StopTime = 0;
			vy = -FIREPIRANHAPLANT_SPEED_Y;
			IsHidden = false;
			IsMoving = true;
		}
	}

	if (IsAttacking && !IsMoving)
	{
		//vy = 0;
		if (AttackTime == 0)
		{
			if (IsInZone)
			{
				FireBullet* bullet = new FireBullet(bulletnx, bulletvx, bulletvy);
				Lstbullet.push_back(bullet);
				SetState(FIREPIRAHAPLANT_ATTACKSTATE);
			}
			AttackTime = GetTickCount64();
			//DebugOut(L"Line 44\n");
		}
		if (GetTickCount64() - AttackTime >= 3000)
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
		Lstbullet[i]->Update(dt, coObjects);
	}
}

void FirePirahaPlant::Render()
{
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
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void FirePirahaPlant::SetState(int state)
{
	switch (state)
	{
	case FIREPIRAHAPLANT_APPEARSTATE:
		vy = -FIREPIRANHAPLANT_SPEED_Y;
		break;
	case FIREPIRAHAPLANT_HIDDENSTATE:
		vy = FIREPIRANHAPLANT_SPEED_Y;
		break;
	case FIREPIRAHAPLANT_ATTACKSTATE:
		if (Lstbullet[0]->GetHealth()==0)
		{
			Lstbullet[0]->SetHealth(1);
			Lstbullet[0]->SetPosition(this->x,this->y);
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
	right = x + FIREPIRAHAPLANT_WIDTH;


	bottom = y + FIREPIRAHAPLANT_HEIGHT;

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
	if (abs(x-m->x) <=80)
	{
		if (m->y>y)
		{
			bulletvy = BULLETVY_NEAR;
		}
		else
		{
			bulletvy = -BULLETVY_NEAR;
		}
	}
	else {
		if (m->y > y)
		{
			bulletvy = BULLETVY_FAR;
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
	}
	else
	{
		IsInZone = false;
	}
}