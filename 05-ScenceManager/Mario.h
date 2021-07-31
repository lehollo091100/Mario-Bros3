#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "MarioGeneral.h"
#include "Fire.h"
#include "Tail.h"
#include "Boomerang.h"
#define MARIO_WALKING_SPEED		0.04f 
#define MARIO_AX				0.04f
#define MARIO_JUMP_SPEED_Y		0.37f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.001f
#define MARIO_GRAVITY_INCREASE	0.00011f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define MARIO_ACCELERATE		0.004f
#define MARIO_DECELERATE		0.006f
#define MARIO_WALKING_MAXSPEED	0.1f
#define MARIO_RUNNING_MAXSPEED	0.2f
#define GRAVITY_INCREASE		0.0004f;
#define GRAVITY_DECREASE_UPFLYING	0.019f
#define GRAVITY_FALLSLOW		0.0005f;
#define GRAVITY_FALLSLOW1		0.0006f;
#define SPEED_RUNNING_INCREASE	0.002f
#define ZEROPOINTFOUR	0.4f
#define ZEROZEROZEROFIVE	0.005f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_RUNNING			500
#define MARIO_STATE_RUN_MAXSPEED	600
#define MARIO_STATE_TURN			700
#define MARIO_STATE_ATTACK_TAIL		800
#define MARIO_STATE_SIT				901
#define MARIO_STATE_FLY				1000
#define MARIO_STATE_SPEEDDOWNRIGHT		1100
#define MARIO_STATE_SPEEDDOWNLEFT		1200
#define MARIO_STATE_FALLING				1300
#define MARIO_STATE_FALLING_SLOW		1301
#define MARIO_STATE_FALLINFLY			1302
#define MARIO_STATE_UP					1400
#define MARIO_STATE_FIRE			1201
#define MARIO_STATE_HOLDTURTLE		1202
#define MARIO_STATE_HOLDTUTLE_WALK	1203
#define MARIO_STATE_KICKTURTLE		1204
#define MARIO_STATE_MOVE_RIGHT		1205
#define MARIO_STATE_MOVE_LEFT		1206
#define MARIO_STATE_MOVE_DOWN		1207
#define MARIO_STATE_MOVE_UP			1208
#define MARIO_STATE_IN_PIPE			1209
#define MARIO_STATE_OUT_PIPE		1210

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7

#define MARIO_ANI_SMALL_JUMP_RIGHT		8
#define MARIO_ANI_SMALL_JUMP_LEFT		9
#define MARIO_ANI_BIG_JUMP_RIGHT		10
#define MARIO_ANI_BIG_JUMP_LEFT		11//
#define MARIO_ANI_SIT_RIGHT				22
#define MARIO_ANI_SIT_LEFT				23

#define MARIOTAIL_ANI_IDLE_RIGHT	12
#define MARIOTAIL_ANI_IDLE_LEFT		13

#define MARIOTAIL_ANI_WALK_RIGHT	14
#define MARIOTAIL_ANI_WALK_LEFT		15

#define MARIOTAIL_ANI_JUMP_RIGHT	16
#define MARIOTAIL_ANI_JUMP_LEFT		17

#define MARIO_ANI_BIG_WALKRIGHT_TURNLEFT	18
#define MARIO_ANI_BIG_WALKLEFT_TURNRIGHT	19
#define MARIO_ANI_SMALL_WALKRIGHT_TURNLEFT	20
#define MARIO_ANI_SMALL_WALKLEFT_TURNRIGHT	21


#define MARIOTAIL_ANI_SIT_RIGHT			24
#define MARIOTAIL_ANI_SIT_LEFT			25
#define MARIOTAIL_ANI_WALKRIGHT_TURNLEFT	26
#define MARIOTAIL_ANI_WALKLEFT_TURNRIGHT	27
#define MARIOTAIL_ANI_FLYRIGHT			28
#define MARIOTAIL_ANI_FLYLEFT			29
#define MARIOTAIL_ANI_ATTACKRIGHT		30
#define MARIOTAIL_ANI_ATTACKLEFT		31			
#define MARIO_ANI_RUN_MAXSPEEDRIGHT			32
#define MARIO_ANI_RUN_MAXSPEEDLEFT		33
#define MARIO_ANI_RUNRIGHT				34
#define MARIO_ANI_RUNLEFT				35
#define MARIOTAIL_ANI_RUN_MAXSPEEDRIGHT		36
#define MARIOTAIL_ANI_RUN_MAXSPEEDLEFT		37

////-----------MARIOWHITE---------------------//
#define MARIOWHITE_ANI_IDLE_RIGHT	38
#define MARIOWHITE_ANI_IDLE_LEFT	39
#define MARIOWHITE_ANI_WALK_RIGHT	40
#define MARIOWHITE_ANI_WALK_LEFT	41
#define MARIOWHITE_ANI_JUMP_RIGHT	42
#define MARIOWHITE_ANI_JUMP_LEFT		43
#define MARIOWHITE_ANI_RUN_RIGHT	44
#define MARIOWHITE_ANI_RUN_LEFT		45
#define MARIOWHITE_ANI_WALKLEFT_TURNRIGHT	46
#define MARIOWHITE_ANI_WALKRIGHT_TURNLEFT	47
#define MARIOWHITE_ANI_RUN_MAXSPEEDRIGHT		48
#define MARIOWHITE_ANI_RUN_MAXSPEEDLEFT		49
#define MARIOWHITE_ANI_SIT_RIGHT		50
#define MARIOWHITE_ANI_SIT_LEFT			51
#define MARIOWHITE_ANI_SHOOT_FIRE_RIGHT		52
#define MARIOWHITE_ANI_SHOOT_FIRE_LEFT		53

#define MARIOTAIL_ANI_FALLRIGHT		54
#define MARIOTAIL_ANI_FALLLEFT		55

#define MARIOBIG_ANI_FLYRIGHT			56
#define MARIOBIG_ANI_FLYLEFT			57
#define MARIOSMALL_ANI_FLYRIGHT			58
#define MARIOSMALL_ANI_FLYLEFT			59
#define MARIOWHITE_ANI_FLYRIGHT		60
#define MARIOWHITE_ANI_FLYLEFT		61
#define MARIOTAIL_ANI_RUNRIGHT		62
#define MARIOTAIL_ANI_RUNLEFT		63
#define MARIOTAIL_ANI_FALLSLOWRIGHT		64
#define MARIOTAIL_ANI_FALLSLOWLEFT		65
#define MARIOTAIL_ANI_FALLFLY_RIGHT		66
#define MARIOTAIL_ANI_FALLFLY_LEFT		67
#define MARIOSMALL_ANI_RUN_RIGHT		68
#define MARIOSMALL_ANI_RUN_LEFT			69
#define MARIOWHITE_ANI_FALLRIGHT		70
#define MARIOWHITE_ANI_FALLLEFT			71
#define MARIO_ANI_BIG_FALL_RIGHT			72
#define MARIO_ANI_BIG_FALL_LEFT				73

#define MARIO_ANI_BIG_HOLDTURTLE_IDLE_RIGHT	74
#define MARIO_ANI_BIG_HOLDTURTLE_IDLE_LEFT	75
#define MARIO_ANI_BIG_HOLDTURTLE_WALK_RIGHT	76
#define MARIO_ANI_BIG_HOLDTURTLE_WALK_LEFT	77

#define MARIO_ANI_BIG_KICK_RIGHT		78
#define MARIO_ANI_BIG_KICK_LEFT		79
#define MARIO_ANI_WHITE_KICK_RIGHT		80
#define MARIO_ANI_WHITE_KICK_LEFT		81
#define MARIO_START_SCENE_ANI		82
#define MARIO_ANI_DIE				83
#define MARIO_ANI_START_SCENE	84
#define MARIO_ANI_WORLDMAP_LEVELTAIL	85
#define MARIO_ANI_WORLDMAP_LEVELBIG		86
#define MARIOTAIL_ANI_GO_IN_PIPE		87
#define MARIO_FIRE_ANI_GO_IN_PIPE		88
#define MARIO_BIG_ANI_GO_IN_PIPE		89
#define MARIO_SMALL_ANI_GO_IN_PIPE		90

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL	4
#define MARIO_LEVEL_FIRE	3

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 28
#define MARIO_TAIL_BBOX_WIDTH_ATTACK	24
#define MARIO_TAIL_BBOX_WIDTH	14

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15
#define MARIOTAIL_BBOX_TAIL		5
#define MARIO_START_SCENE_BBOX_HEIGHT	5

#define MARIO_UNTOUCHABLE_TIME 5000
#define NUMBER_10	10
#define NUMBER_8	8
#define THIRTEEN	13
#define TIME_ATTACK_FIRST	100
#define TIMEATTACK_LAST	470

class CMario : public CGameObject
{
	static CMario * __instance;
public:
	int Point;
	int stack;
	int Coin;
	int level;
	int untouchable;
	DWORD untouchable_start;
	float start_x;			// initial position of Mario at scene
	float start_y;
	int ani;
	int time1;
	//vector<LPGAMEOBJECT> LstWeapon;
	vector<LPFire> LstWeapon;
	Tail *tail;
	//CBoomerang *boom=new CBoomerang();
	//Tail *tail;
	int z;// bien dung de han che so lan tao ra COIN trong exxtrashinningbrick
public:
	bool IsOnSongbrick=false;
	bool IsWalkingR, IsWalkingL, IsRunning, IsSitting, IsRollback, IsSlowDown, IsFalling, IsMaxspeed, IsAttacking,IsDie;
	bool IsJumping;
	bool IsWalking;
	bool IsFallSlow;
	bool IsFlying;
	bool IsFlyup;
	bool IsFallfly;
	bool IsOnGround;
	bool IsHolding,IsKicking;
	int nxbackup;
	float Gravity;
	DWORD RollbackDt;
	DWORD RollBackTime;
	DWORD rightdown, leftdown;
	DWORD LastimeAttack;
	DWORD startfly;
	int time;
	float vx_backup;
	int NextScene;
	//float NextX, NextY;
	bool IsPortal;
	CGameObject* koo=NULL ;
	bool IsInMap3,IsMoving,IsInMap4;
	bool up, down, right, left;
	float NextX, NextY;
	bool IsOnPortal;//nhấn nút xuống mới chuyển cảnh
	bool IsEndGame;
	float OutPipeY;
	bool IsInPipe, IsOuttingPipe;//chui cong( xuong+ len)
	bool CanPiping;//bien de lay gia tri tu PIPE;
public:
	CMario(float x , float y );
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) {
		if (level == 1) {
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
		}
		if (level == 4)
		{
			if (nx > 0) {
				if (l == 2 || l == 3 || l == 1)
				{
					x += MARIOTAIL_BBOX_TAIL;
				}

			}
		}
		if (l == 4 && level != 4)
		{
			x -= MARIOTAIL_BBOX_TAIL;
		}
		if (l >= 4)
			l = 4;
		else if (l <= 1)
			l = 1;
		level = l;
	}
	int GetLevel() { return level; };
	void LevelUp()
	{
		if (level == 1)
			y -=10;
		if(level<4)
			level++;
		
	}
	void SubLevel()
	{
		if (level > 1)
			level--;

	}
	void SetIsFalling(bool f)
	{
		IsFalling = f;
	}
	float Getvx() {
		return vx;
	}
	bool GetJump() { return IsJumping; }

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();


	static CMario * GetInstance();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

};