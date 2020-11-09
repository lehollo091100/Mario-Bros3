#include "Fire.h"

Fire::Fire(int nx)
{
	this->nx = nx;
	vx = nx * 0.19f;
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(4);
	SetAnimationSet(ani_set);
	health = 0;
}

void Fire::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + 10;
	bottom = top + 10;
}

void Fire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health <= 0)
		return;
	float cam_x = CGame::GetInstance()->GetCamX();
	float cam_w = CGame::GetInstance()->GetScreenWidth();
	//out cam
	if (x > cam_x + cam_w || x < cam_x)
		health = 0;
	CGameObject::Update(dt);
	//DebugOut(L"update");
	// Simple fall down
	vy += FIRE_GRAVITY * dt;

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

		if (nx != 0) {
			//destroy
			state = 2;
			x += dx;
			//health--;
		}
		if (ny != 0) {
			//bounding
			vy = ny *0.2f;
		}
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//subhealth
			if (e->obj->GetType() !=GType::BRICK ) {
				e->obj->SubHealth(1);
			}
			
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Fire::Render()
{
	if (health <= 0)
		return;
	int ani = FIRE_ANI_SHOOT;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}
