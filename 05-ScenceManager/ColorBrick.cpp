#include "ColorBrick.h"

void ColorBrick::Render()
{
	RenderBoundingBox();
}

void ColorBrick::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
