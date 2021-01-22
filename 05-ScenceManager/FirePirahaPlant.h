#include "GameObject.h"
#include "Utils.h"
#include "Mario.h"
#include "FireBullet.h"
#define BULLETVY_NEAR	0.03f
#define BULLETVY_FAR	0.01f

#define FIREPIRAHAPLANT_ANI_UPRIGHT		0
#define FIREPIRAHAPLANT_ANI_UPLEFT		1
#define FIREPIRAHAPLANT_ANI_DOWNRIGHT		2
#define FIREPIRAHAPLANT_ANI_DOWNLEFT	3
#define FIREPIRAHAPLANT_ANI_UPIDLERIGHT	4
#define FIREPIRAHAPLANT_ANI_UPIDLELEFT	5
#define FIREPIRAHAPLANT_ANI_DOWNIDLERIGHT	6
#define FIREPIRAHAPLANT_ANI_DOWNIDLELEFT	7

#define FIREPIRAHAPLANT_WIDTH	18
#define FIREPIRAHAPLANT_HEIGHT	31
#define FIREPIRAHAPLANT_HIDDENSTATE 100
#define FIREPIRAHAPLANT_ATTACKSTATE	200
#define FIREPIRAHAPLANT_APPEARSTATE	300
#define FIREPIRAHAPLANT_STOPTIME	3000
#define	FIREPIRANHAPLANT_MAXY_ATK	200
#define	FIREPIRANHAPLANT_MAXX_ATK	150
#define	FIREPIRANHAPLANT_MINX_ATK	16
#define	FIREPIRANHAPLANT_SPEED_Y	0.03

#define MAXHEIGHT_APPEAR	35
#define MAXHEIGHT_HIDDEN	16
class FirePirahaPlant : public CGameObject
{
public:
	bool IsHidden,IsAttacking,IsAppearing,IsInZone,IsMoving;
	DWORD AttackTime, MovingTime;
	DWORD StopTime;
	int count;
	float startY;
	float marioX, marioY;
	CMario* m;
	int numberBullet;
	bool canAttack;
	float bulletvx, bulletvy;
	int bulletnx;
	vector<LPFireBullet> Lstbullet;
	bool IsNextTo;
	int statebackup;
public:
	FirePirahaPlant(CMario *mario)
	{
		FireBullet* bullet = new FireBullet();
		Lstbullet.push_back(bullet);
		type=GType::FIREPIRAHAPLANT;
		IsHidden = true;
		IsAttacking = false;
		IsMoving = false;
		ny = 1;
		m = mario;
		SetHealth(1);
		IsNextTo = false;
		//vy = -FIREPIRANHAPLANT_SPEED_Y;
	};
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render(); 
	virtual void SetState(int state);
	void CalAttackZone();
	void CalAttackPos();
	
};
