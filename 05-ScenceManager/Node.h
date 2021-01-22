#pragma once
#include"GameObject.h"
class CNode : public CGameObject
{
public:
	int up, down, left, right;
	int scene_id;
	float width,height;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {
		left = x;
		top = y;
		right = left + width;
		bottom = top + height;
	};
	CNode(int w,int h,int up,int down,int left,int right,int scene_id) {
		type = GType::NODE;
		width = w;
		height = h;
		this->up = up;
		this->right = right;
		this->left = left;
		this->down = down;
		this->scene_id = scene_id;
		SetHealth(1);

	};
	int GetSceneIDOfNode()
	{
		return this->scene_id;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	bool IsUp()
	{
		if (up == 1)
			return true;
		else return false;
	}
	bool IsDown()
	{
		if (down == 1)
			return true;
		else return false;
	}
	bool IsLeft()
	{
		if (left == 1)
			return true;
		else return false;
	}
	bool IsRight()
	{
		if (right == 1)
			return true;
		else return false;
	}
};

