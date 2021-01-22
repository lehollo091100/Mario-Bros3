#include "Font.h"
#include"Textures.h"
#include"GameObject.h"
#include"Utils.h"
Font::Font(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Font::Render1(float x, float y, string str)
{
	if (animation_set == NULL)
	{
		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(54);
		SetAnimationSet(ani_set);
	}
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(54);
	SetAnimationSet(ani_set);
	for (int i = 0; i < str.size(); i++)
	{
		int num;
		if (str[i] >= '0' && str[i] <= '9')
		{
			num = (int)(str[i]) - 48;//-48+27
		}
		else
			if (str[i] >= 'a' && str[i] <= 'z')
				num = (int)(str[i]) - 96;//-97+1
		animation_set->at(num)->Render(x+i*8, y);
		//if (num != 38)
			//sprite->DrawBoard(x + (i) * 15, y);
	}
}

Font::~Font()
{
}
