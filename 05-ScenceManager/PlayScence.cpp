#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	map = new Map();
	mapid = id;
	map->SetMap(id);
	map->ReadMap();
	key_handler = new CPlayScenceKeyHandler(this);
	grid = new Grid();
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines
	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
	//DebugOut(L"%d\n", ID);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());

	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line,int l,int t,int r,int b )
{
	vector<string> tokens = split(line);
	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	//DebugOut(L"object_tye:%d",object_type);
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	switch (object_type)
	{
	case GType::PORTAL:
	{
		int x1, y1;
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		x1 = atoi(tokens[7].c_str());
		y1 = atoi(tokens[8].c_str());
		//DebugOut(L"nextX%f\n:", x1);
		obj = new CPortal(x, y, r, b, scene_id, x1, y1);
		break;
	}
	case OBJECT_TYPE_MARIO: {
		player->SetPosition(x, y);
		CGame::GetInstance()->SetCamPos(player->x, player->y);
		if (player->NextX != 0 && player->NextY != 0)
		{
			player->SetPosition(player->NextX, player->NextY);
			CGame::GetInstance()->SetCamPos(player->NextX, player->NextY);
			player->NextX = 0;
			player->NextY = 0;
		}
		DebugOut(L"[INFO] Player object created!\n");

		break;
	}
	case GType::GOOMBA: {obj = new CGoomba(); break; }
	case OBJECT_TYPE_BRICK: {
		int W, H;
		W = atoi(tokens[4].c_str());
		H = atoi(tokens[5].c_str());
		obj = new CBrick(W, H);
		break;
	}
	case GType::KOOPAS: obj = new CKoopas(player);/* DebugOut(L"koopas\n");*/ break;
	case GType::KOOPASBROWN: obj = new CKoopasBrown(player); break;

	case GType::FLYKOOPAS: obj = new CFlyKoopas(player); break;
	case GType::COLORBRICK: {
		float W, H;
		W = atoi(tokens[4].c_str());
		H = atoi(tokens[5].c_str());
		obj = new ColorBrick(W, H, x);
		break;
	}
	case GType::QUESTIONBRICK: {
		int QBrickType = atoi(tokens[6].c_str());
		obj = new CQuestionBrick(QBrickType);
		break;
	}

	case GType::FIREPIRAHAPLANT:
	{
		obj = new FirePirahaPlant(player);
		break;
	}
	case GType::FIREGREENPLANT:
	{
		obj = new FireGreenPlant(player);
		break;
	}
	case GType::GREENPLANT:
	{
		obj = new GreenPlant(player);
		break;
	}
	case GType::PIPE:
	{
		int W, H;
		W = atoi(tokens[4].c_str());
		H = atoi(tokens[5].c_str());
		int ani = atoi(tokens[6].c_str());
		int pipeswitch = atoi(tokens[7].c_str());
		obj = new CPipe(W, H, ani, pipeswitch);
		break;
	}
	case GType::BROWNCOIN:
	{
		obj = new CBrownCoin();
		break;
	}
	case GType::BROWNLEAF:
	{
		obj = new CLeaf();
		break;
	}
	case GType::SHINNINGBRICK:
	{
		//int hitem = atoi(tokens[6].c_str());
		obj = new CShinningBrick();
		break;
	}
	case GType::CPBRICK:
	{
		obj = new CPBrick();
		break;
	}
	case GType::SONGBRICK:
	{
		obj = new CSongBrick(player);
		break;
	}
	case GType::EXTRASONGBRICK: {
		obj = new Extrasongbrick();
		break;
	}
	case GType::COUNTERBRICK: 
	{
		int item= atoi(tokens[6].c_str());
		obj = new CounterBrick(item);
		break;
	}
	case GType::NODE:
	{
		int W, H, up, down, left, right, scene_id;
		W = atoi(tokens[4].c_str());
		H = atoi(tokens[5].c_str());
		up = atoi(tokens[6].c_str());
		down = atoi(tokens[7].c_str());
		left = atoi(tokens[8].c_str());
		right = atoi(tokens[9].c_str());
		scene_id = atoi(tokens[10].c_str());
		obj = new CNode(W, H, up, down, left, right, scene_id);
		break;
	}
	case GType::REDGOOMBA:
	{
		obj = new RedGoomba();
		break;
	}
	case GType::ENDGAMEITEM:
	{
		obj = new EndGameObject();
		break;
	}
	case GType::UPDOWNWOOD:
	{
		int W = atoi(tokens[4].c_str());
		int H = atoi(tokens[5].c_str());
		obj = new CUpDownWood(W, H);
		break;
	}
	case GType::SHINNINGEXTRABRICK:
	{
		int h = atoi(tokens[6].c_str());
		int t = atoi(tokens[7].c_str());
		obj = new ShinningExtraBrick(h,t);
		break;
	}
	case GType::TURTLE:
	{
		obj = new Turtle(player);
		break;
	}
	case GType::FLYUPDOWNKOOPAS:
	{
		obj = new FlyUpDownKoopas();
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	if (obj != NULL)
		obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	//-----------------ANIMTION SET FIREE-----------------
	LPANIMATION_SET ani_set2 = animation_sets->Get(FIRE_ANIMATION_SETS_ID);
	for (int i = 0; i < 2; i++)
		player->LstWeapon[i]->SetAnimationSet(ani_set2);
	player->SetAnimationSet(ani_set);
	LPANIMATION_SET ani_set1 = animation_sets->Get(HUD_ANIMATION_SETS_ID);
	hud->SetAnimationSet(ani_set1);
	if (obj != NULL)
	{
		obj->SetAnimationSet(ani_set);
	}
	if (obj != NULL) {
		grid->push_backGrid(obj, l, t, r, b);
		//DebugOut(L"debug325:%d %d %d %d %d\n", obj->type, (int)l, (int)t, (int)r, (int)b);
		
		//objects.push_back(obj);
	}
}

void CPlayScene::Load()
{
	string filegrid;
	if (mapid == 5)
	{
		if(player->x<300)
			cam1 = 0;
	}
	filegrid = "C:/Users/Admin/Documents/Mario Bros 3/Mario-Bros3/LoadGrid/gridinfo"+to_string(mapid)+".txt";
	ifstream ifs(filegrid, ios::in);
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	string filepath;
	//filepath = "inputObjects" + to_string(mapid)+".txt";
	/*Loader *load = new Loader();

	load->LoadGrid(filepath);*/
	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: {
			int l, t, r, b, type;
			ifs >>type>> l >> t >> r >> b;
			/*DebugOut(L"type:%d %d %d %d %d\n",type, l,t,r,b);*/
			_ParseSection_OBJECTS(line,l,t,r,b); 
			break; 
		}
		}
	}
	ifs.close();
	f.close();
	Map::GetInstance()->SetMap(player->NextScene);
	Map::GetInstance()->ReadMap();
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	//player->NextScene = NULL;
}

void CPlayScene::Update(DWORD dt)
{
	//DebugOut(L"mapid:%d\n", mapid);
	if (mapid == 5) {
		grid->GetListObj(objects, mObjects, staticObjects, 290, 224, CGame::GetInstance()->GetCamX(), CGame::GetInstance()->GetCamY());
		grid->ClearGrid(290, 224, CGame::GetInstance()->GetCamX(), CGame::GetInstance()->GetCamY());
	}
	else
	{
		grid->GetListObj(objects,mObjects,staticObjects, 330, 224, player->x - 165, CGame::GetInstance()->GetCamY());
		grid->ClearGrid(330, 224, player->x - 165, CGame::GetInstance()->GetCamY());
	}
	//DebugOut(L"%d %d %d \n", objects.size(), mObjects.size(), staticObjects.size());
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> object2;
	vector<LPGAMEOBJECT> coObjects;
	object2.clear();
	coObjects.clear();
	for (size_t i = 0; i < objects.size(); i++)
	{

		if (objects[i]->IsDie != true)
		{
			object2.push_back(objects[i]);
			if (objects[i]->GetType() == GType::TURTLE)
			{
			}
				//DebugOut(L"type:%d\n", objects[i]->type);
			if (objects[i]->GetType() == GType::QUESTIONBRICK && (objects[i]->GetState() == BRICK_STATE_NOTHINGLEFT))
			{

				if (objects[i]->HiddenItem == TYPE_MUSHROOM) {
					if (objects[i]->HiddenItem != -1) {
						CItem *item = new CItem();
						objects[i]->HiddenItem = -1;
						float qx, qy;
						objects[i]->GetPosition(qx, qy);
						item->SetPosition(qx, qy - BBOX_16);
						CAnimationSets * animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(7);
						item->SetAnimationSet(ani_set);
						object2.push_back(item);
						mObjects.push_back(item);
					}
				}
				if (objects[i]->HiddenItem == TYPE_COIN)
				{
					player->Coin += 1;
					player->Point += 100;
					if (objects[i]->HiddenItem != -1) {
						CCoinInBrick *item = new CCoinInBrick();
						objects[i]->HiddenItem = -1;
						float qx, qy;
						objects[i]->GetPosition(qx, qy);
						item->SetPosition(qx, qy - BBOX_16);
						CAnimationSets * animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(12);
						item->SetAnimationSet(ani_set);
						object2.push_back(item);
						mObjects.push_back(item);
					}
				}
				if (objects[i]->HiddenItem == TYPE_LEAF)
				{
					if (objects[i]->HiddenItem != -1) {
						CLeaf *item = new CLeaf();
						objects[i]->HiddenItem = -1;
						float qx, qy;
						objects[i]->GetPosition(qx, qy);
						item->SetPosition(qx, qy - BBOX_16);
						CAnimationSets * animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(15);
						item->SetAnimationSet(ani_set);
						object2.push_back(item);
						mObjects.push_back(item);
					}
				}

			}
			if ((objects[i]->GetType() == GType::COUNTERBRICK)&& (objects[i]->GetState()==COUNTERBRICK_STATE_CHANGE) )
			{
				if (objects[i]->HiddenItem != -1)
				{
					if (objects[i]->HiddenItem == 1)
					{
						CLeaf *item = new CLeaf();
						objects[i]->HiddenItem = -1;
						float qx, qy;
						objects[i]->GetPosition(qx, qy);
						item->SetPosition(qx, qy - BBOX_16);
						CAnimationSets * animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(15);
						item->SetAnimationSet(ani_set);
						objects.push_back(item);
					}
				}
			}
			if (objects[i]->GetType() == GType::SHINNINGBRICK && (objects[i]->GetState() == SBRICK_STATE_NOTHINGLEFT))
			{
				
			}
			if (objects[i]->GetType() == GType::CPBRICK && (objects[i]->GetState() == CPBRICK_STATE_NOTHINGLEFT))
			{
				float qx, qy;
				objects[i]->GetPosition(qx, qy);
				objects[i]->SetState(CPBRICK_STATE_COLLISION);
				CPItem *PItem = new CPItem();
				PItem->SetPosition(qx, qy - NUM_20);
				object2.push_back(PItem);
				mObjects.push_back(PItem);
			}
			if (objects[i]->GetType() == GType::PITEM && (objects[i]->GetState() == PITEM_STATE_COLLISION))
			{
				for (size_t i = 0; i < objects.size(); i++) {
					if (objects[i]->GetType() == GType::SHINNINGBRICK&&objects[i]->GetState() == BRICK_STATE_NORMAL)
					{
						objects[i]->SetState(SBRICK_STATE_TURN_COIN);
					}
				}
			}
			if (objects[i]->GetType() == GType::FLYKOOPAS && (objects[i]->GetState() == FLYKOOPAS_STATE_DISAPPEAR))
			{
				float a, b;
				objects[i]->GetPosition(a, b);
				objects[i]->SubHealth(1);
				CKoopas *koo = new CKoopas(player);
				koo->SetPosition(a, b);
				CAnimationSets * animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set = animation_sets->Get(6);
				koo->SetAnimationSet(ani_set);
				//animation_set->at(ani)->Render(x, y);
				objects.push_back(koo);
			}
			if (objects[i]->GetType()==GType::FLYUPDOWNKOOPAS&&(objects[i]->GetState()==DISAPPEAR_STATE))
			{
				float a, b;
				objects[i]->GetPosition(a, b);
				objects[i]->SubHealth(1);
				CKoopas *koo = new CKoopas(player);
				koo->SetPosition(a, b);
				CAnimationSets * animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set = animation_sets->Get(6);
				koo->SetAnimationSet(ani_set);
				//animation_set->at(ani)->Render(x, y);
				objects.push_back(koo);
			}
			if (objects[i]->GetType() == GType::SHINNINGEXTRABRICK&&objects[i]->GetState()==EXTRABRICK_STATE_COLLISION)
			{
				
				ShinningExtraBrick* brick = dynamic_cast<ShinningExtraBrick*>(objects[i]);
				//brick->SetState(EXTRABRICK_STATE_NORMAL);
				//brick->number -= 1;
				float qx, qy;
				brick->GetPosition(qx, qy);
				if (brick->kind == 1&&brick->number>0)
				{
					if (player->z == 0) {
						player->z += 1;
						brick->number -= 1;
						CCoinInBrick *coin = new CCoinInBrick();
						coin->SetPosition(qx, qy - BBOX_16 * 2);
						CAnimationSets * animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(12);
						coin->SetAnimationSet(ani_set);
						object2.push_back(coin);
						mObjects.push_back(coin);
					}
				}
				else if (brick->kind == 2&&brick->number>0)
				{
					//a += 1;
					if (player->z == 0) {
						player->z += 1;
						brick->number -= 1;
						CLeaf *leaf = new CLeaf();
						leaf->SetPosition(qx, qy - BBOX_16 * 2);
						CAnimationSets * animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(15);
						leaf->SetAnimationSet(ani_set);
						object2.push_back(leaf);
						mObjects.push_back(leaf);
					}
				}
				else if (brick->kind == 3 && brick->number > 0)
				{
					if (player->z == 0) {
						player->z += 1;
						brick->number -= 1;
						CItem *item = new CItem();
						item->SetPosition(qx, qy - BBOX_16*2);
						CAnimationSets * animation_sets = CAnimationSets::GetInstance();
						LPANIMATION_SET ani_set = animation_sets->Get(7);
						item->SetAnimationSet(ani_set);
						object2.push_back(item);
						mObjects.push_back(item);
					}
				}
				
			}

		}
	}
	//for (size_t i = 0; i < objects.size(); i++)
	//{
	//	if (objects[i]->IsDie != true) {
	//		coObjects.push_back(objects[i]);
	//			DebugOut(L"object:%d\n", objects[i]->GetType());
	//		
	//	}

	//}
	for (size_t i = 0; i < mObjects.size(); i++)
	{
		if (mObjects[i]->IsDie != true)
		{
			//DebugOut(L"%d\n", mObjects[i]->type);
			mObjects[i]->IsInGrid = false;
			mObjects[i]->Update(dt, &staticObjects);
			if (mObjects[i]->IsMovingObj) {
				float l, t, r, b;
				mObjects[i]->GetBoundingBox(l, t, r, b);
				l = (int)(l / 165);
				t = (int)(t / 120);
				r = (int)(r / 165);
				b = (int)(b / 120);
				grid->push_backGrid(mObjects[i], (int)l, (int)t, (int)r, (int)b);
			}
		}
	}
	for (size_t i = 0; i < staticObjects.size(); i++)
	{
		if (staticObjects[i]->IsDie != true)
		{
			staticObjects[i]->IsInGrid = false;
			staticObjects[i]->Update(dt);
			float l, t, r, b;
			staticObjects[i]->GetBoundingBox(l, t, r, b);
			l = (int)(l / 165);
			t = (int)(t / 120);
			r = (int)(r / 165);
			b = (int)(b / 120);
			grid->push_backGrid(staticObjects[i], (int)l, (int)t, (int)r, (int)b);

		}
	}
	/*for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->IsDie != true)
		{
			objects[i]->IsInGrid = false;
			objects[i]->Update(dt, &coObjects);
			if (objects[i]->IsMovingObj) {
				float l, t, r, b;
				objects[i]->GetBoundingBox(l, t, r, b);
				l = (int)(l / 165);
				t = (int)(t / 120);
				r = (int)(r / 165);
				b = (int)(b / 120);
				grid->push_backGrid(objects[i], (int)l, (int)t, (int)r, (int)b);
			}
		}
	}*/

	player->Update(dt, &object2);
	hud->Update(dt);
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	
	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / (2.25);
	//if(player->y)
	hud->SetPoint(player->Point);
	hud->SetStack(player->stack);
	hud->SetCoin(player->Coin);
	//CGame::GetInstance()->SetCamPos(cx, cy);
	if (mapid == 3)
	{
		CGame::GetInstance()->SetCamPos(-60, 0);
		player->IsInMap3 = true;
		player->IsInMap4 = false;
	}
	else if (mapid == 4)
	{
		CGame::GetInstance()->SetCamPos(3, 35);
		player->IsInMap4 = true;
		player->IsInMap3 = false;
	}
	else
	{
		player->IsInMap3 = false;
		player->IsInMap4 = false;
		if (mapid == 6)
		{
			if (player->y > 650)
			{
				CGame::GetInstance()->SetCamPos(cx, 730);
			}
			else
			{
				CGame::GetInstance()->SetCamPos(cx, cy);
			}
		}
		else {

			if (player->y > 250)
			{
				CGame::GetInstance()->SetCamPos(cx, 250);
			}
			else {
				CGame::GetInstance()->SetCamPos(cx, cy);
			}
		}
	}
	if (mapid == 2)
		CGame::GetInstance()->SetCamPos(cx, cy);
	if (mapid == 1)
	{
		hud->IsInScene1 = true;
	}
	if (player->IsPortal == true)
	{
		if(player->NextScene!=2)
			hud->time = 300;
		CGame::GetInstance()->SwitchScene(player->NextScene);
	}

}

void CPlayScene::Render()
{
	map->Drawmap();
	player->Render();
	//player->RenderBoundingBox();
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
		objects[i]->RenderBoundingBox();
	}
	if (mapid != 3 && mapid != 4)
		hud->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	if (player != NULL) {
		player->IsPortal = false;
	}
	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}

	objects.clear();
	grid->RemoveGrid();
	//int levelbackup;
	//player = NULL;
	Map::GetInstance()->Clear();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{

	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE||  mario->IsEndGame == true)
		return;
	//DebugOut(L"int: %d\n", mario->IsJumping);
	int id = ((CPlayScene*)scence)->GetMapID();
	if (id != 3 && id != 4) {
		switch (KeyCode)
		{
		case DIK_R:
			mario->Reset();
			break;
		case DIK_1:
			mario->SetLevel(MARIO_LEVEL_SMALL);
			break;
		case DIK_2:
			mario->SetLevel(MARIO_LEVEL_BIG);
			break;
		case DIK_3:
		{
			mario->SetLevel(MARIO_LEVEL_FIRE);
			break;
		}
		case DIK_4:
		{
			mario->SetLevel(MARIO_LEVEL_TAIL);
			break;
		}
		case DIK_5://SET POSITION(2259,90)
		{
			mario->SetPosition(2259, 80);
			break;
		}
		case DIK_6:
		{
			mario->SetPosition(2210, 370);
			break;
		}
		case DIK_7:
		{
			mario->SetPosition(625, 300);
			break;
		}
		case DIK_A:
		{
			if (mario->level == MARIO_LEVEL_TAIL)
			{
				mario->SetState(MARIO_STATE_ATTACK_TAIL);
				mario->LastimeAttack = GetTickCount64();
				break;
			}
			else if (mario->level == MARIO_LEVEL_FIRE)
			{
				mario->SetState(MARIO_STATE_FIRE);
				break;
			}
			break;
		}
		case DIK_S:
		{
			mario->SetState(MARIO_STATE_UP);

			break;
		}
		}
	}
	if (id == 3 || id == 4)
	{
		switch (KeyCode)
		{
		case DIK_S:
		{
			mario->IsPortal = true;
			mario->SetState(MARIO_STATE_IDLE);
			break;
		}
		}
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	int id = ((CPlayScene*)scence)->GetMapID();
	//DebugOut(L"id:%d\n", id);
	if (id != 3 && id != 4) {
		// disable control key when Mario die
		if (mario->GetState() == MARIO_STATE_DIE)
			return;
		if (game->IsKeyDown(DIK_DOWN))
		{
			if (mario->CanPiping == true)
			{
				mario->SetState(MARIO_STATE_IN_PIPE);
			}
			else if(mario->CanPiping==false)
			{
			if ((!mario->IsWalking) || !mario->IsRunning)
			{

				mario->SetState(MARIO_STATE_SIT);
			}
			}

		}
		if (game->IsKeyDown(DIK_UP))
		{
			if (mario->CanPiping == true) {
				if (mario->IsJumping)
				{
					//mario->ny = -1;
					mario->SetState(MARIO_STATE_IN_PIPE);
				}
			}
		}
		if (game->IsKeyDown(DIK_RIGHT)) {
			
			mario->nx = 1;
			mario->SetState(MARIO_STATE_WALKING_RIGHT);

		}
		if (game->IsKeyDown(DIK_LEFT))
		{
			mario->nx = -1;
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
		if (game->IsKeyDown(DIK_A))
		{
			//mario->IsHolding = true;
			if (mario->IsWalking)
			{
				mario->SetState(MARIO_STATE_RUNNING);
				mario->IsHolding = true;
			}
		}
		if (game->IsKeyDown(DIK_Z))
		{
		}
	}
	if (id == 3 || id == 4)
	{
		if (game->IsKeyDown(DIK_DOWN))
		{
			if (mario->down == true)
			{
				mario->SetState(MARIO_STATE_MOVE_DOWN);
			}
			else
			{
				return;
			}
		}
		if (game->IsKeyDown(DIK_UP))
		{
			if (mario->up == true)
			{
				mario->SetState(MARIO_STATE_MOVE_UP);
			}
			else
			{
				return;
			}
		}
		if (game->IsKeyDown(DIK_RIGHT))
		{
			if (mario->right == true)
			{
				mario->SetState(MARIO_STATE_MOVE_RIGHT);
			}
			else
			{
				return;
			}
		}
		if (game->IsKeyDown(DIK_LEFT))
		{
			if (mario->left == true)
			{
				mario->SetState(MARIO_STATE_MOVE_LEFT);
			}
			else
			{
				return;
			}
		}

	}
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	//DebugOut(L"int: %d\n", mario->IsJumping);
	switch (KeyCode)
	{
	case DIK_RIGHT:
		mario->IsWalkingR = false;
		//mario->IsWalking = false;
		break;
	case DIK_LEFT:
		mario->IsWalkingL = false;
		break;
	case DIK_A:
		mario->IsRunning = false;
		mario->IsHolding = false;
		break;
	case DIK_S:
		float mvx, mvy;
		//mario->IspressingS = false;
		mario->GetSpeed(mvx, mvy);
		if (mvy < 0)
		{
			if (!mario->IsOnSongbrick)
			{
				mario->SetSpeed(mvx, 0);
			}
		}
		mario->IsFallSlow = false;
		if (mario->IsFlying) {
			if (mario->level == 4) {
				mario->IsFalling = true;
				if (mario->IsFlyup)
					mario->IsFallfly = true;
			}
			else if (mario->level == 2 || mario->level == 3)
			{
				mario->IsFalling = true;
				if (mario->IsFlying);
				//mario->IsFlying = false;
				{
					mario->IsFallfly = true;
					mario->IsFalling = false;
				}
			}
		}
		break;
	case DIK_DOWN:
		if (mario->IsSitting) {
			//DebugOut(L"LINE 435 ISSITTING=FALSE");
			mario->IsSitting = false;
			float mx, my;
			mario->GetPosition(mx, my);
			mario->y -= 15;
			mario->SetState(MARIO_STATE_IDLE);
			break;
		}
		else
		{
			break;
		}
		//break;
	case DIK_Z:
		if (mario->IsHolding)
		{
			mario->IsHolding = false;
			break;
		}
	}

}

