#include "ShinningExtraBrick.h"
#include"Utils.h"
#include"CoinInBrick.h"

void ShinningExtraBrick::SetState(int state)
{
	switch (state)
	{
	case EXTRABRICK_STATE_NORMAL:
	{
		this->state = EXTRABRICK_STATE_NORMAL;
		vy = 0;
		break;
	}
	case EXTRABRICK_STATE_COLLISION:
	{
		this->state = EXTRABRICK_STATE_COLLISION;
		vy = -BRICK_SPEED_Y;
		break;
	}
	case EXTRABRICK_STATE_NOTHINGLEFT:
	{
		vy = 0;
		this->state = EXTRABRICK_STATE_NOTHINGLEFT;
		break;
	}
	default:
		break;
	}
}

void ShinningExtraBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (vy == 0)
		startY = this->y;
	CGameObject::Update(dt, coObjects);
	y += dy;
	if (this->y < startY - EXTRABIRCK_MAX_HEIGHT)
	{
		vy = BRICK_SPEED_Y;
	}
	if (startY-y<0)
	{
			if (this->number == 0)
			{
				this->SetState(EXTRABRICK_STATE_NOTHINGLEFT);

			}
			else {

		this->SetState(EXTRABRICK_STATE_NORMAL);
			}
		/*else {
		}*/
	}
	for(int i=0;i<listcoin.size();i++)
		listcoin[i]->Update(dt, coObjects);
	for (int i = 0; i < listleaf.size(); i++)
	{
		//listleaf[i]->GetBoundingBox(this->x, this->y, float &right, float &bottom);
		listleaf[i]->Update(dt, coObjects);
	}
}

void ShinningExtraBrick::Render()
{
	int ani = 0;
	if (state == EXTRABRICK_STATE_NOTHINGLEFT)
	{
		ani = 1;
	}

	animation_set->at(ani)->Render(x, y);
	for(int i=0;i<listcoin.size();i++)
	{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(12);
	listcoin[i]->SetAnimationSet(ani_set);
		listcoin[i]->Render();
	}
	for (int i = 0; i < listleaf.size(); i++)
	{
		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(15);
		listleaf[i]->SetAnimationSet(ani_set);
		listleaf[i]->Render();
	}
	RenderBoundingBox();
}

void ShinningExtraBrick::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = l + EXTRABRICK_BBOX_WIDTH;
	b = t + EXTRABRICK_BBOX_HEIGHT;
}
