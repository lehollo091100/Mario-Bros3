#include "Portal.h"


CPortal::CPortal(float l, float t, float r, float b, int scene_id,int nextx,int nexty)
{
	type = GType::PORTAL;
	this->scene_id = scene_id;
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
	this->nextX = nextx;
	this->nextY = nexty;
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"portal.h");
}
