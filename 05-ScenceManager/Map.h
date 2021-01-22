
#pragma once
#include "Game.h"
#include "Sprites.h"
#include<fstream>
#include "Utils.h"
#include <string>
//typedef Map *LPMAP;
class Map
{
	LPDIRECT3DTEXTURE9 tex;
	int TexRow, TexCol, MapRow, MapCol;
	UINT TexW, TexH;
	int mapId;
	int map[100][200];
	char* FileName;
	string MapLink;
	int nMap;
	unordered_map<int, LPDIRECT3DTEXTURE9> maps;
	static Map *_instance;
	
public:
	Map() {};
	~Map() {};
	void ReadMap();
	void Drawmap();
	void SetMap(int Id);
	void Clear();
	static Map *GetInstance();
};
