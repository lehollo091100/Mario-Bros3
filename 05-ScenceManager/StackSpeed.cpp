#include "StackSpeed.h"
#include"Textures.h"
#include"GameObject.h"
#include"Utils.h"
StackSpeed::StackSpeed()
{
	
}

void StackSpeed::Render1(float x, float y, int s)
{
	if (animation_set == NULL)
	{
		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(55);
		SetAnimationSet(ani_set);
	}
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(55);
	SetAnimationSet(ani_set);
	for (int i = 0; i < s; i++)
	{
		animation_set->at(0)->Render(x + i * 11, y);
	}
}

StackSpeed::~StackSpeed()
{
}

