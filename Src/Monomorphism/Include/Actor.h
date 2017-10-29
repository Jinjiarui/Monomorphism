/*================================================================
Filename: Actor.h
Date: 2017.10.21
Created by AirGuanZ
================================================================*/
#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>

#include <OWE.h>

#include "ActorAction.h"
#include "Weapon.h"

/* Actor��Ҫ
    ���� ���ٶ�-�ٶ�-λ�� ����·������Щ�����Ǹ����ٶȣ���Щ�����Ǹ��ٶ�
    ������״̬����
        Unknown                 ��������������ڵ��ϣ�
        Standing                ��ͨ��վ��
        Running                 ˮƽ�ƶ�
        Shifting                ����
        Jumping                 ��Ծ
        BeingAttacked           ����
        Floating                �����÷��� Orz
        Lying                   ����
        ����״̬                ���ע���״̬��Ҫ���ת�ƺ���
    ���ܵ�������룺
        ���Ҽ�����Ծ������������
        �����˶���
            �����ٶ�
            �������ٶ�
        �Ը��˶���
            �Ը��ٶ�
            �Ը����ٶ�

    ���ռ�������

    ����preupdate
        Floating��ؽ���Lying
        Jumping��ؽ���Unknown
        Standing��Running�������ս���Jumping
        Shifting��Lying��BeingAttackedʱ���������Unknown
        ���ע��״̬�Լ�����
    
    ����state transfer
        Unknown[�����ǿ�������������]            -> Floating
        Unknown[�������]                        -> BeingAttacked
        Unknown[��Ծ��]                            -> Jumping������Ծ��ֱ�ٶȣ��̳�ˮƽ�ٶ�
        Unknown[������]                            -> ��������ע���ת�ƺ���
        Unknown[�ƶ���]                            -> Running�����ƶ����ٶ�
        Unknown[����]                              -> Standing

        Standing[�����ǿ�������������]           -> Floating
        Standing[�������]                       -> BeingAttacked
        Standing[��Ծ��]                           -> Jumping������Ծ��ֱ�ٶȣ��̳�ˮƽ�ٶ�
        Standing[������]                           -> ��������ע���ת�ƺ���
        Standing[�ƶ���]                           -> Running�����ƶ����ٶ�
        Standing[������]                           -> Shifting

        Running[�����ǿ�������������]            -> Floating
        Running[�������]                        -> BeingAttacked
        Running[��Ծ��]                            -> Jumping������Ծ��ֱ�ٶȣ��̳�ˮƽ�ٶ�
        Running[������]                            -> ��������ע���״̬
        Running[�ƶ���]                            -> Running�����ƶ����ٶ�
        Running[����]                              -> Standing
        Running[������]                            -> Shifting

        Jumping[�����ǿ�����/�������]         -> Floating
        Jumping[������]                            -> ��������ע���ת�ƺ���
        Jumping[�ƶ���]                            -> Jumping, ��ˮƽ���ٶ�

        BeingAttacked[�����ǿ�������������]      -> Floating
        BeingAttacked[�������]                  -> BeingAttacked�����ü�ʱ��

        Floating[�����ǿ�����/�������]        -> Floating

        Lying[�����ǿ�������������]              -> Floating
        Lying[�������]                          -> Lying�����ü�ʱ��

    ������ƶ��ͳ�����ײ��⡢�˺�����һ��Ѷ���
*/

class Actor;

namespace _ActorAux
{
    class ActorExpandingState
    {
    public:
        virtual ~ActorExpandingState(void) { }
        virtual void Preupdate(Actor &actor) = 0;
        virtual void Transfer(Actor &actor) = 0;
    };

    enum class ActorState
    {
        Unknown,
        Standing,
        Running,
        Shifting,
        Jumping,
        BeingAttacked,
        Floating,
        Lying,
        Other
    };

    //ÿ֡Actor����ܵ����������
    struct ActorEnvironment
    {
        ActorEnvironment(void)
        {
            Reset();
        }

        void Reset(void)
        {
            collisionLeft  = false;
            collisionRight = false;
            collisionUp    = false;
            collisionDown  = false;

            outVelocity     = glm::vec2(0.0f);
            outAcceVelocity = glm::vec2(0.0f);
        }

        //���������ĸ�������û��ײǽ
        bool collisionLeft;
        bool collisionRight;
        bool collisionUp;
        bool collisionDown;

        //�����ٶ�/���ٶ�
        glm::vec2 outVelocity;
        glm::vec2 outAcceVelocity;

    };

    //ÿ֡Actor���յ����û�����
    struct ActorUserInput
    {
        ActorUserInput(void)
        {
            Reset();
        }

        void Reset(void)
        {
            movingLeft  = false;
            movingRight = false;
            jumping     = false;
            shifting    = false;
        }

        bool movingLeft;
        bool movingRight;
        bool jumping;
        bool shifting;
    };
}

class Actor : public OWE::Utility::Uncopyable
{
public:
    using Action         = _ActorAux::ActorAction;          //��������
    using ExpandingState = _ActorAux::ActorExpandingState;  //��չ״̬
    using InternalState  = _ActorAux::ActorState;           //�ڽ�״̬
    using Environment    = _ActorAux::ActorEnvironment;     //��������
    using Input          = _ActorAux::ActorUserInput;       //�û�����

    using ActionTexRsc = std::vector<OWE::Texture2D>;       //������������

    Actor(void);

    //��������
    const Input &GetInput(void) const;
    Input &GetInput(void);

    //��������
    const Environment &GetEnvironment(void) const;
    Environment &GetEnvironment(void);

    //״̬����
    void Update(void);

private:
    void _Preupdate(void);

private:
    //����
    Input input_;
    Environment envir_;
    //��ǰ״̬
    ExpandingState *expandingState_;
    InternalState internalState_;
    //�������
    Action action_;

    Action::TexSeq  actionTexStanding_;
    Action::TexSeq  actionTexRunning_;
    Action::TexSeq  actionTexShifting_;
    Action::TexSeq  actionTexJumping_;
    Action::TexSeq  actionTexBeingAttacked_;
    Action::TexSeq  actionTexFloating_;
    Action::TexSeq  actionTexLying_;

    ActionTexRsc actionTexRscStanding_;
    ActionTexRsc actionTexRscRunning_;
    ActionTexRsc actionTexRscShifting_;
    ActionTexRsc actionTexRscJumping_;
    ActionTexRsc actionTexRscBeingAttacked_;
    ActionTexRsc actionTexRscFloating_;
    ActionTexRsc actionTexRscLying_;
};

#endif //__ACTOR_H__
