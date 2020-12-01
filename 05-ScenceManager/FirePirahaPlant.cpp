#include "FirePirahaPlant.h"
#include "Utils.h"


void FirePirahaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"startY:%f\n", startY);
	if (startY == 0)
		startY = y;
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	//DebugOut(L"vy%f\n:", vy);
	
	if (vy != 0) {
		if (startY - y >= 35)
		{

			if (IsMoving && !IsAttacking)
			{
				vy = 0;
				IsAttacking = true;
				IsMoving = false;
			}
		}
	}
	if (vy != 0) {
		if (y >= startY+35)
		{
			if (IsMoving && !IsHidden)
			{
				DebugOut(L"LINE 33\n");
				vy = 0;
				IsHidden = true;
				IsMoving = false;
			}
		}
	}
	//if (IsMoving)
	//{
	//	if (MovingTime == 0)
	//	{
	//		MovingTime = GetTickCount64();
	//	}
	//	if (GetTickCount64() - MovingTime >= 500)
	//	{
	//		MovingTime = 0;
	//		IsMoving = false;
	//		vy = 0;
	//		if (y >= startY)
	//		{
	//			IsHidden = true;
	//		}
	//		else 
	//			IsAttacking = true;
	//	}
	//}
	if (IsHidden&&!IsMoving)
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
			DebugOut(L"line 24\n");
		}

	}
	DebugOut(L"y:%f\n", y);

	if (IsAttacking && !IsMoving)
	{
		if (AttackTime == 0)
		{
			AttackTime = GetTickCount64();
			DebugOut(L"Line 44\n");
		}
		if (GetTickCount64() - AttackTime >= 3000)
		{
			DebugOut(L"LINE 63");
			AttackTime = 0;
			vy = FIREPIRANHAPLANT_SPEED_Y;
			IsAttacking = false;
			IsMoving = true;
		}
	}

	DebugOut(L"ATTACKTIME:%d\n", AttackTime);
	/*if (IsAttacking && AttackTime == 0)
	{
		AttackTime = GetTickCount64();
	}
	if (IsAttacking && GetTickCount64() - AttackTime >= 2000)
	{
		IsAttacking = false;
		AttackTime = 0;
	}
	if (MovingTime == 0)
	{
		MovingTime = GetTickCount64();
	}

	if (startY - y >= MAXHEIGHT_APPEAR)
	{
		vy = 0;
		if (!IsAttacking && GetTickCount64() - MovingTime >= 3000)
		{
			SetState(FIREPIRAHAPLANT_HIDDENSTATE);
			MovingTime = 0;
		}
		if (vy <= 0)
			CalAttackZone();
	}
	if (startY - y <= -MAXHEIGHT_HIDDEN)
	{
		vy = 0;
		if (GetTickCount64() - MovingTime >= 3000)
		{
			MovingTime = 0;
			SetState(FIREPIRAHAPLANT_APPEARSTATE);
		}
	}*/
	/*if (y<=startY)
	{
		IsHidden = true;
	}*/
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
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];


		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void FirePirahaPlant::Render()
{
	int ani =0;
	if (EnemyX > x)
	{
		DebugOut(L"Line 130\n");
		if (EnemyY < y)
			ani = FIREPIRAHAPLANT_ANI_UPRIGHT;
		else
			ani=FIREPIRAHAPLANT_ANI_DOWNRIGHT;
	}
	else
	{
		DebugOut(L"Line 136\n");
		if (EnemyY < y)
			animation_set->at(FIREPIRAHAPLANT_ANI_UPLEFT)->Render(x, y);
		else
			animation_set->at(FIREPIRAHAPLANT_ANI_DOWNLEFT)->Render(x, y);
	}
	//animation_set->at(ani)->Render(x, y);
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