#pragma once
#include <vector>
#include <istream>
#define MARIO_WALKING_SPEED		0.15f 
#define MARIO_AX				0.04f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.0015f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f


#define MARIO_WALKING_DECELERATION						0.0002f
#define MARIO_WALKING_ACCELERATION						0.0006f
#define MARIO_WALKING_MAX_SPEED							0.15f

#define MARIO_RUNNING_DECELERATION						0.00525f
#define MARIO_RUNNING_ACCELERATION						0.00006f
#define MARIO_RUNNING_MAX_SPEED							0.25f
#define FRICTION										0.0016875f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMPING			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_RUN_RIGHT       500
#define MARIO_STATE_RUN_LEFT        600
#define MARIO_STATE_WALKING			201
#define MARIO_STATE_ATTACK_TAIL		800
#define MARIO_STATE_SITTING				900
#define MARIO_STATE_FLY				1000
#define MARIO_STATE_TURN			700
#define MARIO_STATE_STOP_RUNNING			701
#define MARIO_STATE_SPEEDDOWNRIGHT		1100
#define MARIO_STATE_SPEEDDOWNLEFT		1200

//---------------IDLE--------------------//
#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3
#define MARIOTAIL_ANI_IDLE_RIGHT	12
#define MARIOTAIL_ANI_IDLE_LEFT		13

//--------------WALKING--------------------//
#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7
#define MARIOTAIL_ANI_WALK_RIGHT	14
#define MARIOTAIL_ANI_WALK_LEFT		15


//----------------JUMPING------------------//
#define MARIO_ANI_SMALL_JUMP_RIGHT		8
#define MARIO_ANI_SMALL_JUMP_LEFT		9
#define MARIO_ANI_BIG_JUMP_RIGHT		10
#define MARIO_ANI_BIG_JUMP_LEFT		11
#define MARIOTAIL_ANI_JUMP_RIGHT	16
#define MARIOTAIL_ANI_JUMP_LEFT		17



//--------------aniturn--------------------//
#define MARIO_ANI_BIG_WALKRIGHT_TURNLEFT	18
#define MARIO_ANI_BIG_WALKLEFT_TURNRIGHT	19
#define MARIO_ANI_SMALL_WALKRIGHT_TURNLEFT	20
#define MARIO_ANI_SMALL_WALKLEFT_TURNRIGHT	21
#define MARIOTAIL_ANI_WALKRIGHT_TURNLEFT	26
#define MARIOTAIL_ANI_WALKLEFT_TURNRIGHT	27

//------------------SITTING---------------//
#define MARIO_ANI_SIT_RIGHT				22
#define MARIO_ANI_SIT_LEFT				23
#define MARIOTAIL_ANI_SIT_RIGHT			24
#define MARIOTAIL_ANI_SIT_LEFT			25

//-----------------Flying----------------//
#define MARIOTAIL_ANI_FLYRIGHT			28
#define MARIOTAIL_ANI_FLYLEFT			29


#define MARIOTAIL_ANI_ATTACK_RIGHT		30
#define MARIOTAIL_ANI_ATTACK_LEFT		31

#define MARIO_ANI_DIE				32

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL	3

#define MARIO_BIG_BBOX_WIDtH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000
using namespace std;
class CMarioGeneral
{
	static CMarioGeneral* __instance;
	int level;
public:
	CMarioGeneral();
	vector<int> listAni_mario_small;
	vector<int> listAni_mario_big;
	vector<int> listAni_mario_tail;
	void LoadListAni();
	int GetAni_Mario(int index);
	void SetLevel(int lv);
	static CMarioGeneral* GetInstance();
};

