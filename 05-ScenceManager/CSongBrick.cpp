#include "CSongBrick.h"
#define SONGBRICK_WIDTH	16
CSongBrick::CSongBrick(CMario *m) {
	type= GType::SONGBRICK;
	health = 1;
	width = SONGBRICK_WIDTH;
	IsDie = false;
	vy = 0;
	state = SONGBRICK_STATE_NORMAL;
	vx = 0;
	ny = 1;
	startY = 0;
	mario = m;
}
void CSongBrick::Render() {
	int ani = 0;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void CSongBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	if (vy == 0)
	{
		startY = y;
		//DebugOut(L"songbricky: %f\n", y);
	}
	if (state==SONGBRICK_STATE_HITUP)
	{
		if ( startY -y >= RANGE)
		{
			vy = SONGBRICK_SPEED_UP;
		}
		if (y>=startY)
		{
			SetState(SONGBRICK_STATE_NORMAL);
		}
	}
	if (state == SONGBRICK_STATE_JUMPEDON)
	{
		if (y-startY >= RANGE*2)
		{
			vy = -SONGBRICK_SPEED_UP;
		}
		if (y <= startY)
		{
			/*mario->vy  -= SONGBRICK_HITUP_SPEED ;
			mario->IsJumping = true;
			mario->IsOnGround = false;*/
			SetState(SONGBRICK_STATE_NORMAL);
		}
	}
	//}
	
	

}
void CSongBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
		case SONGBRICK_STATE_NORMAL:
		{

			vy = 0;
			if (y != startY)
			{
				y = startY;
			}
			break; 
		}
		case SONGBRICK_STATE_HITUP:
		{
			vy = -SONGBRICK_SPEED_UP;
			break;
		}
		case SONGBRICK_STATE_JUMPEDON:
		{
			vy =SONGBRICK_SPEED_UP;
			break;
		}
	}
}
void CSongBrick::GetBoundingBox(float &l, float &t, float &r, float &b) {
	l = x;
	t = y;
	r = x + width;
	b = y + width;
}