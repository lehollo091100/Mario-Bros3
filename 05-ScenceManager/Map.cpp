#include "Map.h"
#include <fstream>
#include"Utils.h"
Map *Map::_instance = NULL;
Map *Map::GetInstance()
{
	if (_instance == NULL) _instance = new Map();
	return _instance;
}
void Map::ReadMap()
{
	ifstream ifs(MapLink, ios::in);
	ifs >> MapRow >> MapCol;
	for(int i=0;i<MapRow;i++)
		for (int j = 0; j < MapCol; j++)
		{
			ifs >>map[i][j];
		}
	ifs.close();

}

void Map::Drawmap()
{
	RECT r;
	int FrameW = TexW / TexCol;
	int FrameH = TexH / TexRow;
	//DebugOut(L"%d %d \n", FrameW, FrameH);
	//DebugOut(L"Row%dcol%d\n",MapRow,MapCol);
	for (int i = 0; i < MapRow; i++)
	{
		for (int j = 0; j < MapCol; j++)
		{
			int IdFrame = map[i][j];
			int h = i * FrameH;
			r.left = (IdFrame - 1) % TexCol * FrameW;
			r.top = (IdFrame - 1) / TexCol * FrameH;
			r.right = r.left + FrameW;
			r.bottom = r.top + FrameH;
			CGame::GetInstance()->Draw(j * FrameW , i * FrameH , tex,r.left, r.top, r.right, r.bottom);
		}
		
	}
}
void Map::SetMap(int Id)
{
	mapId = Id;
	string TexLink;
	ifstream ifs("MapInfo.txt", ios::in);
	ifs >> nMap;
	//DebugOut(L"nMap%d\n", nMap);
	if (mapId > nMap)
		return;
	for (int i = 0; i < mapId; i++)
	{
		ifs >> MapLink;
		ifs >> TexLink;
		ifs >> TexRow >> TexCol;
	}
	//Lay texture tu linktex
	LPCSTR FName = TexLink.c_str();
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFileA(FName, &info);
	DebugOut(L"Texlink:%c\n", TexLink.c_str());
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

void Map::Clear()
{
	for (int i = 0; i < MapRow; i++)
	{
		for (int j = 0; j < MapCol; j++)
		{
			map[i][j] = 0;
		}
	}
	TexCol = TexRow = 0;
}


