#include "GameObject.h"
#include "Utils.h"

#define FIREPIRAHAPLANT_ANI_UPRIGHT		0
#define FIREPIRAHAPLANT_ANI_UPLEFT		1
#define FIREPIRAHAPLANT_ANI_DOWNRIGHT		2
#define FIREPIRAHAPLANT_ANI_DOWNLEFT	3

#define FIREPIRAHAPLANT_WIDTH	18
#define FIREPIRAHAPLANT_HEIGHT	35
#define FIREPIRAHAPLANT_HIDDENSTATE 100
#define FIREPIRAHAPLANT_ATTACKSTATE	200
#define FIREPIRAHAPLANT_APPEARSTATE	300

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
public:
	FirePirahaPlant()
	{
		type:GType::FIREPIRAHAPLANT;
		IsHidden = true;
		IsAttacking = false;
		IsMoving = false;
		ny = 1;
		
		//vy = -FIREPIRANHAPLANT_SPEED_Y;
	};
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render(); 
	virtual void SetState(int state);
	void CalAttackZone() {
		if (abs(x - EnemyX) <= FIREPIRANHAPLANT_MAXX_ATK && abs(y - EnemyY) <= FIREPIRANHAPLANT_MAXY_ATK)
		{
			if (!IsAttacking)
				count = 1;
			IsInZone = true;
		}
		else
		{
			IsInZone = false;
		}
	}

};
