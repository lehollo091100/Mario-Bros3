#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "ColorBrick.h"
#include "QuestionBrick.h"
#include "Item.h"
#include "Pipe.h"
#include"FirePirahaPlant.h"
#include"GreenPlant.h"
#include"FireGreenPlant.h"
#include"KoopasBrown.h"
#include"FlyKoopas.h"
#include"BrownCoin.h"
#include"ShinningBrick.h"
#include"Leaf.h"
#include"Node.h"
#include"RedGoomba.h"
#include"CoinInBrick.h"
#include "points.h"
#include"EndGameObject.h"
#include"HUD.h"
#include "PBrick.h"
#include"PItem.h"
#include"UpDownWood.h"
#include"ShinningExtraBrick.h"
#include"Turtle.h"
#include"FlyUpDownKoopas.h"
#include"Grid.h"
#include"BrokenBrick.h"
#define BBOX_16	16
#define NUM_20	20
#define HUD_ANIMATION_SETS_ID	53
#define FIRE_ANIMATION_SETS_ID	4
class CPlayScene : public CScene
{
protected:
	CMario *player=CMario::GetInstance();		
	// A play scene has to have player, right? 
	Map *map;
	CHUD *hud = CHUD::GetInstance();
	Grid *grid;
	vector<LPGAMEOBJECT> objects;
	int mapid;
	

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line,int l, int t, int r, int b);

public:

	float nextX, nextY;
	int levelbackup;
	bool Iscreated;
public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario * GetPlayer() { return player; }
	int GetMapID() {
		return mapid;
	}
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) ;
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

