#include "HUD.h"
#include<string>

CHUD::CHUD()
{
	//animation_set = CAnimationSets::GetInstance()->Get(9);
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(53);
	SetAnimationSet(ani_set);
	time = 300;
	count = 0;
}

void CHUD::Update(DWORD dt)
{
	if (count >= 1000)
	{
		time -= 1;
		count = 0;
	}
	else {
		count += dt;
	}
	if (Point == 0)
	{
		p = "0000000";
	}
	if (Point < 1000&&Point>0)
	{
		p = "0000" + to_string(Point);
	}
	else if (Point >= 1000)
	{
		p = "000" + to_string(Point);
	}
	else if (Point >= 10000)
	{
		p = "00" + to_string(Point);
	}
}

void CHUD::Render()
{
	if (animation_set == NULL)
	{
		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(53);
		SetAnimationSet(ani_set);
	}
	int ani = HUD_ANI;
	string c = to_string(Coin);
	string t = to_string(time);
	animation_set->at(ani)->Render(CGame::GetInstance()->GetCamX(), CGame::GetInstance()->GetCamY()+169);
	stack->Render1(CGame::GetInstance()->GetCamX() + 70, CGame::GetInstance()->GetCamY() +180, Stack);
	point->Render1(CGame::GetInstance()->GetCamX() +75, CGame::GetInstance()->GetCamY() +188,(p));
	coin->Render1(CGame::GetInstance()->GetCamX() + 175, CGame::GetInstance()->GetCamY() + 180, (c));
	Time->Render1(CGame::GetInstance()->GetCamX() + 175, CGame::GetInstance()->GetCamY() + 190, (t));
}
CHUD *CHUD::__instance = NULL;
CHUD * CHUD::GetInstance()
{
	if (__instance == NULL) __instance = new CHUD();
	return __instance;
}