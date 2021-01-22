#pragma once
#include "GameObject.h"
#include"Game.h"
#include "Font.h"
#include"StackSpeed.h"
#define HUD_ANI	0
class CHUD : public CGameObject
{
	static CHUD * __instance;
public:
	int Point;
	int Stack;
	int Coin;
	Font* point = new Font(CGame::GetInstance()->GetCamX()+30, CGame::GetInstance()->GetCamY() + 175);
	Font* coin = new Font(CGame::GetInstance()->GetCamX(), CGame::GetInstance()->GetCamY() + 169);
	Font* Time = new Font(CGame::GetInstance()->GetCamX(), CGame::GetInstance()->GetCamY() + 160);
	int time;
	bool IsInScene1;
	DWORD count;
	StackSpeed* stack = new StackSpeed();
	string p;
	//string p = (string)(Point);
	CHUD();
	void Update(DWORD dt);
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {
		return;
	};
	static CHUD * GetInstance();
	void SetPoint(int p){
		this->Point = p;
	};
	void SetStack(int s)
	{
		this->Stack = s;
	};
	void SetCoin(int c)
	{
		this->Coin = c;
	}


};

