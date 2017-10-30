/*================================================================
Filename: Actor.cpp
Date: 2017.10.24
Created by AirGuanZ
================================================================*/
#include <limits>
#include <numeric>
#include <string>

#include <glm\glm.hpp>
#include <OWE.h>

#include "Include\Actor.h"
#include "Include\ConfigureFile.h"
#include "Include\ResourceNames.h"

using namespace std;
using namespace glm;
using namespace OWE;

namespace
{
    void _LoadActionRsc(const ConfigureFile &conf, const string &action,
                        Actor::Action::TexSeq &texSeq, Actor::Action::KpSeq &kpSeq)
    {
        texSeq.clear();
        kpSeq.clear();

        int cnt = stoi(conf(action, "Count"));
        texSeq.resize(cnt);
        kpSeq.resize(cnt);

        for(int i = 0; i != cnt; ++i)
        {
            string stri = to_string(i);
            string texFilename = conf(action, "Tex" + stri);
            if(!LoadTexture2DFromFile(texFilename, Texture2D::Desc(), texSeq[i]))
                throw FatalError("Failed to load texture from file: " + texFilename);
            kpSeq[i] = stod(conf(action, "Kp" + stri)) + (i ? kpSeq[i - 1] : 0.0);
        }
    }
}

Actor::Actor(void)
{
    state_ = State::Unknown;
    dir_ = Direction::Right;
}

void Actor::Initialize(void)
{
    //========= ��ʼ�����ڻ��Ƶ�������Դ =========

    ConfigureFile conf;
    if(!conf.Load(ACTOR_ACTION_RESOURCE))
        throw FatalError(string("Failed to load configure file: ") + ACTOR_ACTION_RESOURCE);

    _LoadActionRsc(conf, "Standing", actTexStanding_, actKpStanding_);
    _LoadActionRsc(conf, "Running",  actTexRunning_,  actKpRunning_);
    _LoadActionRsc(conf, "Jumping",  actTexJumping_,  actKpJumping_);
    _LoadActionRsc(conf, "Shifting", actTexShifting_, actKpShifting_);

    conf.Clear();
}

Actor::UserInput &Actor::GetUserInput(void)
{
    return user_;
}

Actor::EnvirInput &Actor::GetEnvirInput(void)
{
    return envir_;
}


/*
    ���ݵ�ǰ���������Ƿ�Ҫת�Ƶ���������
*/
void Actor::Update(double time)
{
    switch(state_)
    {
    case State::Unknown:
        _UpdateStanding(time);
        break;
    case State::Standing:
        _UpdateStanding(time);
        break;
    case State::Running:
        _UpdateRunning(time);
        break;
    case State::Jumping:
        _UpdateJumping(time);
        break;
    case State::Shifting:
        _UpdateShifting(time);
        break;
    }
}

void Actor::_UpdateStanding(double time)
{
    State oldState = state_;
    state_ = State::Standing;

    //���������ջ�����Ծ����������Ծ��̬
    if(!envir_.colDown || user_.jump)
    {
        _UpdateJumping(time);
        return;
    }

    //������shift������������̬
    if(user_.shift)
    {
        _UpdateShifting(time);
        return;
    }

    //�����ƶ�״̬
    if(user_.left || user_.right)
    {
        _UpdateRunning(time);
        return;
    }

    //�Ƿ����½���Standing״̬
    if(state_ != oldState)
    {
        act_.SetData(&actTexStanding_, &actKpStanding_);
        act_.EnableLoop(true);
        act_.Restart();
    }

    act_.Tick(time);
}

void Actor::_UpdateRunning(double time)
{
    State oldState = state_;
    state_ = State::Running;

    //���������ջ�����Ծ����������Ծ��̬
    if(!envir_.colDown || user_.jump)
    {
        _UpdateJumping(time);
        return;
    }

    //������shift������������̬
    if(user_.shift)
    {
        _UpdateShifting(time);
        return;
    }

    //��û���ƶ������ص�Standing
    if(!user_.left && !user_.right)
    {
        _UpdateStanding(time);
        return;
    }

    //��֡������ƶ�����
    Direction newDir = user_.left ? Direction::Left : Direction::Right;

    //�Ƿ������½���Running״̬
    if(state_ != oldState || newDir != dir_)
    {
        dir_ = newDir;
        act_.SetData(&actTexRunning_, &actKpRunning_);
        act_.EnableLoop(true);
        act_.Restart();
    }

    accVel_ += vec2((dir_ == Direction::Right ? runningAccVel_ : -runningAccVel_), 0.0f);
    act_.Tick(time);
}

void Actor::_UpdateJumping(double time)
{
    State oldState = state_;
    state_ = State::Jumping;

    //�������ŵأ�����վ����̬
    //����!jump��������Ϊ��������ʱҲ����colDown����
    if(envir_.colDown && !user_.jump)
    {
        _UpdateStanding(time);
        return;
    }

    //�Ƿ����½���Jumping״̬
    if(state_ != oldState)
    {
        act_.SetData(&actTexJumping_, &actKpJumping_);
        act_.EnableLoop(true);
        act_.Restart();

        //����������������ģ�������ֱ���ٶ�
        if(envir_.colDown && user_.jump)
            vel_.y = jumpingVel_;
    }

    //�Ƿ��ڿ��и����˼��ٶ�
    if(user_.left)
    {
        accVel_.x -= floatingAccVel_;
        dir_ = Direction::Left;
    }
    else if(user_.right)
    {
        accVel_.y += floatingAccVel_;
        dir_ = Direction::Right;
    }

    act_.Tick(time);
}

void Actor::_UpdateShifting(double time)
{
    State oldState = state_;
    state_ = State::Shifting;

    //ʱ��������Զ�ת��ΪStanding״̬
    if(act_.End())
    {
        _UpdateStanding(time);
        return;
    }

    //�Ƿ����½���Shifting״̬��
    if(state_ != oldState)
    {
        //�ս�Shiftingʱ���иķ���Ļ���
        if(user_.left)
            dir_ = Direction::Left;
        else if(user_.right)
            dir_ = Direction::Right;

        act_.SetData(&actTexShifting_, &actKpShifting_);
        act_.EnableLoop(false);
        act_.Restart();
    }

    //ֻҪ���ڵ��Ͼ͸������ٶ�
    if(envir_.colDown)
        vel_.x = (dir_ == Direction::Left ? -shiftingVel_ : shiftingVel_);

    act_.Tick(time);
}

