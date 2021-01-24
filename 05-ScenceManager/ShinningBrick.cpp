#include "ShinningBrick.h"
CShinningBrick::CShinningBrick()
{
	type = GType::SHINNINGBRICK;
	width = QBRICK_BBOX_WIDTH;
	height = QBRICK_BBOX_HEIGHT;
	SetHealth(2);
	//state = 1;
	//vy = 0;
	SetState(BRICK_STATE_NORMAL);
	num = 0;
	vx1 = 0.08f;
	vx2 = -0.08f;
	vx3 =  0.08;
	vx4 = -0.08f;
	vy1 = -0.3f;
	vy2 = -0.3f;
	vy3 = -0.25f;
	vy4 = -0.25f;

}
void CShinningBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	DebugOut(L"health:%d\n", health);
	if (health == 0)
	{
		IsDie = true;
	}
	if (IsDie)
	{
		return;
	}
	if (state != SBRICK_STATE_NOTHINGLEFT) {
		vy = 0;
		CGameObject::Update(dt);
		y += dy;
		if (num == 0)
		{
			if (state == SBRICK_STATE_TURN_COIN)
			{
				if (time == 0)
				{
					time = GetTickCount64();
				}
				if (GetTickCount64() - time >= 5000)
				{
					time = 0;
					num = 1;
					SetState(BRICK_STATE_NORMAL);
				}
			}
		}
	}
	else
	{
		vy1 += 0.002f * dt;
		vy2 += 0.002f * dt;
		vy3 += 0.002f * dt;
		vy4 += 0.002f * dt;
		x1 += vx1 * dt;
		x2 += vx2 * dt;
		x3 += vx3 * dt;
		x4 += vx4 * dt;
		y1 += vy1 * dt;
		y2 += vy2 * dt;
		y3 += vy3 * dt;
		y4 += vy4 * dt;
		
		if (y1 > CGame::GetInstance()->GetCamY() + CGame::GetInstance()->GetScreenHeight())
		{
			health = 0;
		}
	}
}
void CShinningBrick::Render()
{
	if (IsDie)
	{
		return;
	}
	if (state != SBRICK_STATE_NOTHINGLEFT)
	{
		int ani = 0;

		/*if (state == BRICK_STATE_NOTHINGLEFT)
		{
			ani = 1;
		}*/
		if (state == SBRICK_STATE_TURN_COIN) {
			ani = 2;
		}

		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();
	}
	else
	{
		//DebugOut(L"line ani\n");
		animation_set->at(1)->Render(x1, y1);
		animation_set->at(1)->Render(x2, y2);
		animation_set->at(1)->Render(x3, y3);
		animation_set->at(1)->Render(x4, y4);
		
	}
}

void CShinningBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	//if (state != SBRICK_STATE_NOTHINGLEFT) {
		l = x;
		t = y;
		r = x + width;
		b = y + height;
	//}
	
}
void CShinningBrick::SetState(int state)
{
	switch (state)
	{
	case BRICK_STATE_NORMAL:

		this->state = BRICK_STATE_NORMAL;
		break;
	case SBRICK_STATE_NOTHINGLEFT:
		x1 = x2 = x3 = x4 = this->x+8;
		y1 = y2 = y3 = y4 = this->y;
		this->state = SBRICK_STATE_NOTHINGLEFT;
		//this->SubHealth(1);
		//SubHealth(1);
		break;
	case SBRICK_STATE_DISAPPEAR:
		this->state = SBRICK_STATE_DISAPPEAR;
		break;
	case SBRICK_STATE_TURN_COIN:
		this->state = SBRICK_STATE_TURN_COIN;
		break;
	}
}

