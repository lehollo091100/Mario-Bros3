#include "FireBullet.h"
#include "Mario.h"

FireBullet::FireBullet()
{
	//this->nx = nx;
	//vx = nx * 0.19f;
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(4);
	SetAnimationSet(ani_set);
	//SetHealth(1);
	health = 0;
	
}

void FireBullet::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + FIRE_BULLET_BBOX;
	bottom = top + FIRE_BULLET_BBOX;
}

void FireBullet::Update(DWORD dt, LPGAMEOBJECT mario)
{
	if (health <= 0)
		return;
	if (IsDie)
		return;
	float cam_x = CGame::GetInstance()->GetCamX();
	float cam_w = CGame::GetInstance()->GetScreenWidth();
	//out cam
	if (x > cam_x + cam_w || x < cam_x)
		health = 0;
	vector<LPGAMEOBJECT> coOb;
	coOb.clear();
	coOb.push_back(mario);
	CGameObject::Update(dt, &coOb);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(&coOb, coEvents);
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
		//x += dx;
		//y += dy;
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == GType::MARIO) 
			{
				health--;
				e->obj->SetState(MARIO_STATE_DIE);
				/*CMario *mario = dynamic_cast<CMario *>(e->obj);
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					mario->SetState(MARIO_STATE_DIE);
				else
				{
					mario->SetLevel(mario->level - 1);
				}*/
				
			}
			else{
				x += dx;
				y += dy;
			}
			
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void FireBullet::Render()
{
	if (health<=0)
		return;
	else
	{
		int ani = FIRE_ANI_SHOOT;

		animation_set->at(ani)->Render(x, y);

		RenderBoundingBox();
	}

}
