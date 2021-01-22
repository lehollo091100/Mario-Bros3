#include "Node.h"
#include"Utils.h"

void CNode::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt,coObjects);
}

void CNode::Render()
{
	//int ani = 0;
	//animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}