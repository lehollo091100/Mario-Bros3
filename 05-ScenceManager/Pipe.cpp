#include "Pipe.h"

void CPipe::Render()
{

	animation_set->at(Ani)->Render(x, y);
	RenderBoundingBox();
}

void CPipe::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}