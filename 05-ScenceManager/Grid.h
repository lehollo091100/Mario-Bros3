#pragma once
#include<fstream>
#include"GameObject.h"
#include"Mario.h"
#include"Game.h"
class Grid
{
	ofstream ofs;
	CMario* mario;
	//vector<LPGAMEOBJECT> LstObj;
	unordered_map< int, unordered_map<int, vector<LPGAMEOBJECT>> > mObjGrid;
	int backup_top, backup_bot, backup_left, backup_right;
	unordered_map< int, unordered_map<int, vector<LPGAMEOBJECT>> > ObjGrid;
public:
	void push_backGrid(LPGAMEOBJECT obj,int left,int top,int right,int bottom ,bool Check = false);
	Grid()
	{
	};
	~Grid() {}
	void GetListObj(vector<CGameObject*>& LstObj, vector<CGameObject*>& mLstObj, vector<CGameObject*>& staticLstObj, int camwidth, int camheight, float camX, float camY);
	void ClearGrid(int camwidth, int camheight, float camX, float camY);
	void RemoveGrid() {
		mObjGrid.clear();
		ObjGrid.clear();
	};
};
