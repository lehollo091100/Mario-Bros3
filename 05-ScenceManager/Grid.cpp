#include "Grid.h"

void Grid::push_backGrid(LPGAMEOBJECT obj, int left, int top, int right, int bottom, bool Check)
{

	for (int i = left; i <= right; i++)
		for (int j = top; j <= bottom; j++) {
			if (obj->IsMovingObj)
			{

				mObjGrid[i][j].push_back(obj);
			}
			else
			{
				ObjGrid[i][j].push_back(obj);
			}
		}

}

void Grid::GetListObj(vector<CGameObject*>& LstObj, int camwidth, int camheight, float camX, float camY)
{
	
	float l, t, r, b;
	int top = (int)(camY / 120);
	int bottom = (int)((camY + camheight) / 120);
	int left = (int)((camX ) / 165);
	int right = (int)((camX + camwidth) / 165);
	/*if (top == backup_top && bottom == backup_bot && left == backup_left && right == backup_right)
		return;*/
	/*for (int i = 0; i < LstObj.size(); i++)
	{
		LstObj[i]->GetBoundingBox(l, t, r, b);
		l = (int)(l/165);
		t = (int)(t / 120);
		r = (int)(r / 165);
		b = (int)(b / 120);
		push_backGrid(LstObj[i], (int)l, (int)t, (int)r, (int)b);
	}*/
	/*backup_top = top;
	backup_bot = bottom;
	backup_left = left;
	backup_right = right;*/
	LstObj.clear();
	//LstEnemy.clear();
	for (int i = left; i <= right; i++) {
		
		for (int j = top; j <= bottom; j++)
		{
			DebugOut(L"I,J:%d %d\n", i, j);
			DebugOut(L"size obj:%d\n", ObjGrid[i][j].size());

			for (int k = 0; k < mObjGrid[i][j].size(); k++)
			{
				if (mObjGrid[i][j].at(k) != NULL && mObjGrid[i][j].at(k)->GetHealth() > 0)
				{
					if (!mObjGrid[i][j].at(k)->IsInGrid) {
						LstObj.push_back(mObjGrid[i][j].at(k));
						mObjGrid[i][j].at(k)->IsInGrid = true;
					}
				}
			}
			for (int k = 0; k < ObjGrid[i][j].size(); k++)
			{
				if (ObjGrid[i][j].at(k) != NULL && ObjGrid[i][j].at(k)->GetHealth() > 0)
				{
					if (!ObjGrid[i][j].at(k)->IsInGrid) {
						LstObj.push_back(ObjGrid[i][j].at(k));
						ObjGrid[i][j].at(k)->IsInGrid = true;
					}
				}
			}
		}
	}
	
}

void Grid::ClearGrid(int camwidth, int camheight, float camX, float camY)
{
	int top = (int)(camY / 120);
	int bottom = (int)((camY + camheight) / 120);
	int left = (int)((camX ) / 165);
	int right = (int)((camX + camwidth) / 165);
	for (int i = left; i <= right; i++)
		for (int j = top; j <= bottom; j++) {
			
			mObjGrid[i][j].clear();
		}
	//LstObj.clear();
}