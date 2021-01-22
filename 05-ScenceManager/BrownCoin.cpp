#include "BrownCoin.h"

void CBrownCoin::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (IsDie == false) {
		left = x;
		right = left + COIN_WIDTH;
		top = y;
		bottom = y + COIN_WIDTH;
	}
	

}

void CBrownCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (health != 1)
		IsDie = true;
	if (IsDie) {
		return;
	}
}

void CBrownCoin::Render()
{
	int ani = 0;
	if (health == 1)
	{
		ani = COIN_ANI_APPEAR;
	}
	else return;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
