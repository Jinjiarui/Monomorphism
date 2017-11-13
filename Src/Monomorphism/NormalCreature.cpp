#include"Include\NormalCreature.h"
#include "Include\Creturedamage.h"
#include<OWE.h>

NormalCreature::NormalCreature(int hp, glm::vec2 lp, glm::vec2 rp , std::string filepath[3], float xSpeed_, int _damage)
{
	lp_ = lp;
	rp_ = rp;
	hp_ = 0;
	damage_ = _damage;
	status = NormalCreature::Status::STANDING;
	relation_ = CreatureRelation::HOSTILE; //default
	standingtime = 0;
	//loading Texture;
	OWE::_Texture2DAux::Desc tmp;
	OWE::_Texture2DAux::_LoadTexture2DFromFile(filepath[1], tmp, texture_);
	OWE::_Texture2DAux::_LoadTexture2DFromFile(filepath[2], tmp, texattacked);
	OWE::_Texture2DAux::_LoadTexture2DFromFile(filepath[3], tmp, texdead);
	xspeed = xSpeed_;
}

NormalCreature::~NormalCreature(void)
{
	
	//improve
}

void NormalCreature::Update(glm::vec2 playerPoi, double time)
{
	//Improve
	//move towards 
	if (status == NormalCreature::Status::STANDING)
		_updateSTANDING(playerPoi, time);
	else if (status == NormalCreature::Status::FLOATING)
		_updateFLOATING(playerPoi, time);
	else if (status == NormalCreature::Status::DEAD)
		{_updateDEAD(time);return;}
	//gravity;
	//attack;
	
	if (attack1CoolDown_ < 0) 
	{
		Attack1();
	}
	attack1CoolDown_ -= time;
	if (delaytime >= 0) delaytime -= time;
	if (status == NormalCreature::Status::STANDING)standingtime += time;

	velocity_ -= glm::vec2(0, time * gravity);
	//do gravity;
	if (hp_ <= 0) 
	{ 
		status = NormalCreature::Status::DEAD; 
	}
}
// ---- update---------
void NormalCreature::_updateSTANDING(glm::vec2 playerPoi, double time)
{
	float xSpeed = xspeed;
	if (delaytime > 0) xSpeed = attackedspeedy;
	if (relation_ == CreatureRelation::HOSTILE )
	{
		glm::vec2 temp(0, 0);
		if (playerPoi.x > (lp_.x))
		{
			velocity_.x = xSpeed;
		}
		else
		{
			velocity_.x = -xSpeed;
		}

		//ת��Ϊ��Ծ״̬�ж�
		if (playerPoi.y < (lp_.y) && standingtime > standtim)
		{
			velocity_.y = xSpeed;
			standingtime = 0;
			status = NormalCreature::Status::FLOATING;
		}
	}
}
glm::vec2 NormalCreature::GetPosition(void) const
{
	return lp_;
}

glm::vec2 NormalCreature::GetVelocity(void) const
{
	return velocity_;
}

void NormalCreature::_updateFLOATING(glm::vec2 playerPoi, double time)
{
	float xSpeed = xspeed;
	if (delaytime > 0) xSpeed = attackedspeedy;
	if (relation_ == CreatureRelation::HOSTILE)
	{
		glm::vec2 temp(0, 0);
		if (playerPoi.x > (lp_.x))
		{
			velocity_.x = xSpeed;
		}
		else
		{
			velocity_.x = -xSpeed;
		}
	}//change status in main loop
}


void NormalCreature::_updateDEAD(double time)
{
	//play dead anime        
	deadlasttime -= time;
	velocity_ = glm::vec2(0, 0);
	if (deadlasttime < 0)
	{
		;//do nothing
	}
	//~NormalCreature();
}

// update ends

void NormalCreature::SetPosition(const glm::vec2 &poi)
{
	glm::vec2 lp1 = lp_;
	glm::vec2 rp1 = rp_;
	lp_ = poi;
	rp_ = poi + (rp1 - lp1);
}
void NormalCreature::Draw(const OWE::ScreenScale &screenscale)
{
	OWE::Texture2DView nowText;
	if (status == NormalCreature::Status::DEAD)nowText  = texdead.GetTextureView();
	else if (delaytime > 0) nowText = texattacked.GetTextureView();
	else nowText = texture_.GetTextureView();
	if (velocity_.x > 0)//heading left
	{
		OWE::ImmediateRenderer::DrawTexturedBox(lp_, rp_, glm::vec2(0, 0), glm::vec2(1, 1),
			nowText, screenscale);
	}
	else
	{
		OWE::ImmediateRenderer::DrawTexturedBox(lp_, rp_, glm::vec2(1, 0), glm::vec2(0, 1),
			nowText, screenscale);
	}
}



void NormalCreature::SetRelation(CreatureRelation c)
{
	relation_ = c;
}

void NormalCreature::SetHp(int hp)
{
	hp_ = hp;
}

std::vector<OWE::BoundingArea> NormalCreature::GetBoundingAreas(void) const
{
	OWE::BoundingArea::AABB area(lp_.x, lp_.y, rp_.x, rp_.y);
	if (status != NormalCreature::Status::DEAD)
		return { OWE::BoundingArea(area) };
	else
		return {};
}

CreatureRelation NormalCreature::RelationwithActor(void) const
{
	return relation_;
}

void NormalCreature::Attack1()
{
	const double attack1Col = 2500; //2500ms default
	//improve : create damage area
	OWE::BoundingArea::AABB area(lp_.x, lp_.y, rp_.x, rp_.y);
	DamageArea * demage = new CreaturedamageA(area, damage_, 1000);
	//add to sense
	//
	attack1CoolDown_ = attack1Col;
}

bool NormalCreature::IsDead(void)
{
	if (deadlasttime < 0) return true;
	else return false;
}

