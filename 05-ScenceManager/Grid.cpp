#include "Grid.h"

void Grid::push_backGrid(LPGAMEOBJECT obj, int left, int top, int right, int bottom, bool Check)
{

	for (int i = left; i <= right; i++)
		for (int j = top; j <= bottom; j++) {
				//DebugOut(L"type:%d\n",obj->type);
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

void Grid::GetListObj(vector<CGameObject*>& LstObj, vector<CGameObject*>& mLstObj, vector<CGameObject*>& staticLstObj, int camwidth, int camheight, float camX, float camY)
{
	
	float l, t, r, b;
	int top = (int)(camY / 120);
	int bottom = (int)((camY + camheight) / 120);
	int left = (int)((camX ) / 165);
	int right = (int)((camX + camwidth) / 165);
	LstObj.clear();
	staticLstObj.clear();
	mLstObj.clear();
	for (int i = left; i <= right; i++) {
		
		for (int j = top; j <= bottom; j++)
		{
			//DebugOut(L"I,J:%d %d\n", i, j);
			//DebugOut(L"size obj:%d\n", ObjGrid[i][j].size());

			for (int k = 0; k < mObjGrid[i][j].size(); k++)
			{
				if (mObjGrid[i][j].at(k) != NULL && mObjGrid[i][j].at(k)->GetHealth() > 0)
				{
					if (!mObjGrid[i][j].at(k)->IsInGrid) {

						LstObj.push_back(mObjGrid[i][j].at(k));
						mLstObj.push_back(mObjGrid[i][j].at(k));
						mObjGrid[i][j].at(k)->IsInGrid = true;

					}
				}
			}

			for (int k = 0; k < ObjGrid[i][j].size(); k++)
			{
				//DebugOut(L"debug325:%d \n", ObjGrid[i][j].at(k)->type);
				if (ObjGrid[i][j].at(k) != NULL && ObjGrid[i][j].at(k)->GetHealth() > 0)
				{
					if (!ObjGrid[i][j].at(k)->IsInGrid) {
						LstObj.push_back(ObjGrid[i][j].at(k));
						staticLstObj.push_back(ObjGrid[i][j].at(k));
						ObjGrid[i][j].at(k)->IsInGrid = true;
					}
				}
			}
			//ObjGrid[i][j].clear();
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
			ObjGrid[i][j].clear();

		}
	//LstObj.clear();
}