#include "BrokenBrick.h"

BrokenBrick::BrokenBrick(float X, float Y)
{
	health = 1;
	type = GType::BROKENBRICK;
	//LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	CSprites::GetInstance()->Add(40026, 484, 120, 492, 128, CTextures::GetInstance()->Get(20));
	sprite=sprite2=sprite3=sprite4 = new CSprite(40026,484,120,492,128, CTextures::GetInstance()->Get(20));
	x = x2 = x3 = x4;
	y = y2 = y3 = y4;
	vx = 0.15f;
	vx2 = -0.15f;
	vx3 = 0.07;
	vx4 = -0.07f;
	vy = -0.25f;
	vy2 = -0.25f;
	vy3 = -0.22f;
	vy4 = -0.22f;
}

void BrokenBrick::Render()
{
	if (IsDie)
		return;
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(13);
	SetAnimationSet(ani_set);
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
	/*sprite->Draw(x, y, 255, 1);
	sprite2->Draw(x2, y2, 255, 1);
	sprite3->Draw(x3, y3, 255, 1);
	sprite4->Draw(x4, y4, 255, 1);*/
}
void BrokenBrick::Update(DWORD dt)
{
	if (this->health == 0)
		IsDie = true;
	if (IsDie)
		return;
	vy += 0.002f * dt;
	vy2 += 0.002f * dt;
	vy3 += 0.002f * dt;
	vy4 += 0.002f * dt;
	x += dt * vx;
	x2 += dt * vx2;
	x3 += dt * vx3;
	x4 += dt * vx4;
	y += dt * vy;
	y2 += dt * vy2;
	y3 += dt * vy3;
	y4 += dt * vy4;
	if (this->x > 500)
		this->health = 0;
}
