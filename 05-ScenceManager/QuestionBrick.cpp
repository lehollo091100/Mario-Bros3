#include "QuestionBrick.h"
CQuestionBrick::CQuestionBrick(int hitem)
{
	type = GType::QUESTIONBRICK;
	width = QBRICK_BBOX_WIDTH;
	height = QBRICK_BBOX_HEIGHT;
	SetHealth(1);
	startY = 0;
	HiddenItem = hitem;
	state = 1;
}
void CQuestionBrick::Render()
{
	int ani = 0;
	if (state == BRICK_STATE_NOTHINGLEFT)
	{
		ani = 1;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
		if (vy == 0)
			startY = y;
		if (startY - y >= MAX_HIGH)
		{
			vy = BRICK_SPEED_Y;
		}
		if (startY - y < 0)
		{
			
			SetState(BRICK_STATE_NOTHINGLEFT);
			DebugOut(L"line 34 hiddenitem%d\n",HiddenItem);
			switch (HiddenItem)
			{
			case GType::GOOMBA:
			{
				CGoomba *goomba = new CGoomba();
				float qx, qy;
				//objects[i]->GetPosition(qx, qy);
				goomba->SetPosition(x, y - 17);
				CAnimationSets * animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set = animation_sets->Get(3);
				goomba->SetAnimationSet(ani_set);
				coObjects->push_back(goomba);
				DebugOut(L"line 48\n");
			}
			default:
				break;
			}
		}
		CGameObject::Update(dt);
		y += dy;
	
}

void CQuestionBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
void CQuestionBrick::SetState(int state)
{
	switch (state)
	{
	case BRICK_STATE_COLISSION:
		vy = -BRICK_SPEED_Y;
		break;
	case BRICK_STATE_NOTHINGLEFT:
		this->state = state;
		vy = 0;
		if (y > startY)
			y = startY;
		//SubHealth(1);

		break;
	}
}
