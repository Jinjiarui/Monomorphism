/*================================================================
Filename: Actor.cpp
Date: 2017.10.24
Created by AirGuanZ
================================================================*/
#include <limits>
#include <numeric>

#include <glm\glm.hpp>
#include "Include\Actor.h"
#include "Include\ConfigureFile.h"
#include "Include\ResourceNames.h"

using namespace std;
using namespace glm;
using namespace OWE;

namespace
{
    void _LoadActorStandingAnimation(vector<Texture2D> &texSeq, vector<float> &kpSeq)
    {
        texSeq.clear();
        kpSeq.clear();

        ConfigureFile config;
        if(!config.Load(ACTOR_ANIMATION_STANDING_CONFIGURE))
        {
            throw OWE::FatalError(string("Failed to load configure file: ") +
                                  ACTOR_ANIMATION_STANDING_CONFIGURE);
        }

        int cnt = stoi(config("Count"));
        texSeq.resize(cnt);
        kpSeq.resize(cnt);
        for(int i = 0; i != cnt; ++i)
        {
            string stri = to_string(i);
            string filename = config("Tex" + stri);
            if(!LoadTexture2DFromFile(filename, Texture2D::Desc(), texSeq[i]))
                throw FatalError("Failed to load texture from file: " + filename);
            kpSeq[i] = stof(config("KeyPoint") + stri);
        }
    }

    void _LoadActorWalkingAnimation(vector<Texture2D> &texSeq, vector<float> &kpSeq, vector<float> &speedSeq)
    {
        texSeq.clear();
        kpSeq.clear();
        speedSeq.clear();

        ConfigureFile config;
        if(!config.Load(ACTOR_ANIMATION_WALKING_CONFIGURE))
        {
            throw OWE::FatalError(string("Failed to load configure file: ") +
                                  ACTOR_ANIMATION_WALKING_CONFIGURE);
        }

        int cnt = stoi(config("Count"));
        texSeq.resize(cnt);
        kpSeq.resize(cnt);
        speedSeq.resize(cnt);
        for(int i = 0; i != cnt; ++i)
        {
            string stri = to_string(i);
            string filename = config("Tex" + stri);
            if(!LoadTexture2DFromFile(filename, Texture2D::Desc(), texSeq[i]))
                throw FatalError("Failed to load texture from file: " + filename);
            kpSeq[i] = stof(config("KeyPoint" + stri));
            speedSeq[i] = stof(config("Speed" + stri));
        }

        //�ٶȹ�һ��
        float speedFactor = 1.0f / accumulate(begin(speedSeq), end(speedSeq), numeric_limits<float>::min(),
            [](float acc, float f) { return std::max(acc, f); });
        for(float &f : speedSeq)
            f *= speedFactor;
    }

    void _LoadActorJumpingAnimation(vector<OWE::Texture2D> &texSeq, vector<float> &kpSeq)
    {
        texSeq.clear();
        kpSeq.clear();

        ConfigureFile config;
        if(!config.Load(ACTOR_ANIMATION_JUMPING_CONFIGURE))
        {
            throw FatalError(string("Failed to load configure file: ") +
                             ACTOR_ANIMATION_JUMPING_CONFIGURE);
        }

        int cnt = stoi(config("Count"));

        texSeq.resize(cnt);
        kpSeq.resize(cnt);
        for(int i = 0; i != cnt; ++i)
        {
            string stri = to_string(i);
            string filename = config("Tex" + stri);
            if(!LoadTexture2DFromFile(filename, Texture2D::Desc(), texSeq[i]))
                throw FatalError("Failed to load texture from file: " + filename);
            kpSeq[i] = stoi(config("KeyPoint" + stri));
        }
    }
}

OWE::KEY_CODE Actor::keyLeft_  = OWE::KEY_CODE::KEY_A;
OWE::KEY_CODE Actor::keyRight_ = OWE::KEY_CODE::KEY_D;
OWE::KEY_CODE Actor::keyJump_  = OWE::KEY_CODE::KEY_SPACE;

float Actor::walkingSpeed_ = 1.0f;
float Actor::jumpingSpeed_ = 1.0f;

Actor::Actor(void)
{
    Reset();
}

Actor::~Actor(void)
{

}

void Actor::Initialize(void)
{
    //���ֶ�����������Դ
    _LoadActorStandingAnimation(standingTexSeq_, standingKpSeq_);
    _LoadActorWalkingAnimation(walkingTexSeq_, walkingKpSeq_, walkingSpeedSeq_);
    _LoadActorJumpingAnimation(jumpingTexSeq_, jumpingKpSeq_);

    Reset();
}

void Actor::Reset(void)
{
    maxVelocity_ = 0.0f;
    minVelocity_ = 0.0f;

    position_ = vec2(0.0f, 0.0f);
    velocity_ = vec2(0.0f, 0.0f);
    acceleratedVelocity_ = vec2(0.0f, 0.0f);

    state_ = State::Standing;
    dir_ = Direction::Right;

    aniTime_ = 0.0f;
    aniIdx_ = 0;
}

void Actor::SetMaxVelocity(float maxVel)
{
    maxVelocity_ = maxVel;
}

float Actor::GetMaxVelocity(void) const
{
    return maxVelocity_;
}

void Actor::SetMinVelocity(float minVel)
{
    minVelocity_ = minVel;
}

float Actor::GetMinVelocity(void) const
{
    return minVelocity_;
}

void Actor::SetPosition(const vec2 &pos)
{
    position_ = pos;
}

vec2 Actor::GetPosition(void) const
{
    return position_;
}

void Actor::SetVelocity(const vec2 &vel)
{
    velocity_ = vel;
}

vec2 Actor::GetVelocity(void) const
{
    return velocity_;
}

void Actor::SetAcceleratedVelocity(const vec2 &accVel)
{
    acceleratedVelocity_ = accVel;
}

vec2 Actor::GetAcceleratedVelocity(void) const
{
    return acceleratedVelocity_;
}

Actor::State Actor::GetState(void) const
{
    return state_;
}

void Actor::UpdateMoving(double time)
{
    InputManager &im = InputManager::GetInstance();
    
    State newState = state_;
    Direction newDir = dir_;
    if(_CanWalk(state_))
    {
        if(im.IsKeyPressed(keyLeft_))
        {
            newState = State::Walking;
            newDir = Direction::Left;
        }
        else if(im.IsKeyPressed(keyRight_))
        {
            newState = State::Walking;
            newDir = Direction::Right;
        }
    }

    if(newState == State::Walking) //û����
    {
        if(state_ == State::Walking && newDir == dir_) //���������ƶ�����
        {
            aniTime_ += time;
            if(aniTime_ >= walkingKpSeq_.back())
            {
                aniTime_ = 0.0f;
                aniIdx_ = 0;
            }
            else while(aniTime_ >= walkingKpSeq_[aniIdx_])
                ++aniIdx_;
        }
        else //���¿�ʼ�����ƶ�����
        {
            aniTime_ = 0.0f;
            aniIdx_ = 0;
        }
    }

    state_ = newState;
    dir_ = newDir;
}

void Actor::UpdateJumping(double time)
{
    InputManager &im = InputManager::GetInstance();

    State newState = state_;
    Direction newDir = dir_;
    if(_CanJump(state_) && im.IsKeyPressed(keyJump_))
    {
        newState = State::Floating;
        velocity_.y += jumpingSpeed_;
        aniTime_ = 0.0f;
        aniIdx_ = 0;
    }

    if(newState == State::Floating)
    {
        if(im.IsKeyPressed(keyLeft_))
            newDir = Direction::Left;
        else if(im.IsKeyPressed(keyRight_))
            newDir = Direction::Right;

        if(state_ == State::Floating && newDir == dir_) //�������Ŷ���
        {
            aniTime_ += time;
            if(aniTime_ >= jumpingKpSeq_.back())
            {
                aniTime_ = 0.0f;
                aniIdx_ = 0;
            }
            else while(aniTime_ >= jumpingKpSeq_[aniIdx_])
                ++aniIdx_;
        }
        else //���²��Ŷ���
        {
            aniTime_ = 0.0f;
            aniIdx_ = 0;
        }
    }

    state_ = newState;
    dir_ = newDir;
}

void Actor::Draw(const ScreenScale &scale)
{
    vec2 LB = position_ - vec2(0.5f * size_.x, 0.0f);
    vec2 RT = position_ + vec2(0.5f * size_.x, size_.y);
    vec2 uvLB, uvRT;
    if(dir_ == Direction::Left)
    {
        uvLB = vec2(1.0f, 0.0f);
        uvRT = vec2(0.0f, 1.0f);
    }
    else
    {
        uvLB = vec2(0.0f, 0.0f);
        uvRT = vec2(1.0f, 1.0f);
    }

    switch(state_)
    {
    case State::Standing:
        ImmediateRenderer::DrawTexturedBox(
            LB, RT, uvLB, uvRT,
            standingTexSeq_[aniIdx_], scale,
            ImmediateRenderer::RenderMode::AlphaTest);
        break;
    case State::Walking:
        ImmediateRenderer::DrawTexturedBox(
            LB, RT, uvLB, uvRT,
            walkingTexSeq_[aniIdx_], scale,
            ImmediateRenderer::RenderMode::AlphaTest);
        break;
    case State::Floating:
        ImmediateRenderer::DrawTexturedBox(
            LB, RT, uvLB, uvRT,
            jumpingTexSeq_[aniIdx_], scale,
            ImmediateRenderer::RenderMode::AlphaTest);
        break;
    }
}

bool Actor::_CanWalk(State state)
{
    if(state == State::Standing ||
       state == State::Walking)
        return true;
    return false;
}

bool Actor::_CanJump(State state)
{
    if(state == State::Standing ||
       state == State::Walking)
        return true;
    return false;
}
