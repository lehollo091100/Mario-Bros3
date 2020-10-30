#include "Map.h"
#include <fstream>
void Map::ReadMap()
{
	ifstream ifs(MapLink, ios::in);
	ifs >> MapRow >> MapCol;
	for(int i=0;i<MapRow;i++)
		for (int j = 0; j < MapCol; j++)
		{
			ifs >>map[i][j];
		}
	int noBj;//so loai Obj
	ifs >> noBj;
	for (int i = 0; i < noBj; i++)
	{
		int Type;//day la loai Ojb (luu trong file txt)
		ifs >> Type;
		int nofType;//so luong cua loai Obj
		ifs >>nofType;
		for (int j = 0; j < nofType; j++)
		{
			int idObj, xObj, yObj, anisetid,  width, height;
			int objHiden, left, right, top, bot;
			ifs >> idObj >> xObj >> yObj >> width >> height >> objHiden >> left >> right >> top >> bot;
		}
	}
	ifs.close();

}

void Map::Drawmap()
{
	RECT r;
	int FrameW = TexW / TexCol;
	int FrameH = TexH / TexRow;
	//DebugOut(L"%d %d \n", FrameW, FrameH);
	for (int i = 0; i < MapRow; i++)
	{
		for (int j = 0; j < MapCol/2; j++)
		{
			int IdFrame = map[i][j];
			int h = i * FrameH;
			r.left = (IdFrame - 1) % TexCol * FrameW;
			r.top = (IdFrame - 1) / TexCol * FrameH;
			r.right = r.left + FrameW;
			r.bottom = r.top + FrameH;
			//CGame::GetInstance()->Draw(j * FrameW , i * FrameH , tex,r.left, r.top, r.right, r.bottom);
		}
		//DebugOut(L"\n");
	}
}
void Map::SetMap(int Id)
{
	mapId = 1;
	string TexLink;
	ifstream ifs("MapInfo.txt", ios::in);
	ifs >> nMap;
	DebugOut(L"nMap%d\n", nMap);
	if (Id > nMap)
		return;
	for (int i = 0; i < mapId; i++)
	{
		ifs >> MapLink;
		
		//ifs >> MapLink;
		//ifs >> MapRow >> MapColumn;
		ifs >> TexLink;
		ifs >> TexRow >> TexCol;
		DebugOut(L"%d%d%s\n", TexCol, TexRow, TexLink);
		
	}
	//Lay texture tu linktex
	LPCSTR FName = TexLink.c_str();
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFileA(FName, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed\n");
		return;
	}
	TexH = info.Height;
	TexW = info.Width;
	//DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", info.Height,info.Width);
	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	result = D3DXCreateTextureFromFileExA(
		d3ddv,								// Pointer to Direct3D device object
		FName,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 90),
		&info,
		NULL,
		&tex);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}
	DebugOut(L"[INFO] Maptile loaded Ok\n");
	ifs.close();
}
