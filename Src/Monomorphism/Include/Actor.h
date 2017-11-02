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

namespace _ActorAux
{
    enum class ActorInternalState
    {
        Unknown,
        Standing,
        Running,
        Jumping,
        Shifting,
        AttackingWithSword,
    };

    enum class ActorDirection
    {
        Left,
        Right
    };

    struct ActorUserInput
    {
        ActorUserInput(void)
        {
            Reset();
        }

        void Reset(void)
        {
            left   = false;
            right  = false;
            jump   = false;
            shift  = false;
            attack = false;
        }

        bool left;
        bool right;
        bool jump;
        bool shift;
        bool attack;
    };

    struct ActorEnvirInput
    {
        ActorEnvirInput(void)
        {
            Reset();
        }

        void Reset(void)
        {
            colLeft  = false;
            colRight = false;
            colUp    = false;
            colDown  = false;
        }

        bool colLeft;
        bool colRight;
        bool colUp;
        bool colDown;
    };
}

/* ״̬�������̣�
    ���ü��ٶ�
    actor_.Update();
    ���������ٶ�
    actor_.UpdateVelocity();
    ���û���������û�����
    ����λ�ã���ײ��⣬���ٶȣ��ָ���ײ��λ�ã��Ļ�������
    ��¼�û�����
*/
class Actor
{
public:
    using Action = _ActorAux::ActorAction;
    using State = _ActorAux::ActorInternalState;
    using Direction = _ActorAux::ActorDirection;
    using UserInput = _ActorAux::ActorUserInput;
    using EnvirInput = _ActorAux::ActorEnvirInput;

    using TransSeq = std::vector<glm::mat3>;

    Actor(void);

    void Initialize(void);
    void ClearAccVel(void);

    UserInput &GetUserInput(void);
    EnvirInput &GetEnvirInput(void);

    void Update(double time);
    void UpdateVelocity(double time);

    void Draw(const OWE::ScreenScale &scale);

    glm::vec2 &GetPosition(void);
    glm::vec2 &GetTexSize(void);
    glm::vec2 &GetVelocity(void);
    glm::vec2 &GetAccVelocity(void);

    void SetRunningVel(float Vel);        //�ƶ�ʱ�Ը���ˮƽ���ٶ�
    void SetFloatingAccVel(float accVel); //�ڿ���ʱ�Ը���ˮƽ���ٶ�
    void SetJumpingVel(float vel);        //��Ծ��ֱ��������ĳ��ٶ�
    void SetShiftingVel(float vel);       //�����ٶ�

    void SetMaxFloatingVel(float vel);
    void SetFloatingFricAccVel(float accVel);

    void SetWeapon(Weapon *weapon);

private:
    void _UpdateStanding(double time);
    void _UpdateRunning(double time);
    void _UpdateJumping(double time);
    void _UpdateShifting(double time);
    void _UpdateAttackingWithSword(double time);

    void _DrawNormalAction(const OWE::ScreenScale &scale);
    void _DrawStanding(const OWE::ScreenScale &scale);
    void _DrawRunning(const OWE::ScreenScale &scale);
    void _DrawJumping(const OWE::ScreenScale &scale);
    void _DrawShifting(const OWE::ScreenScale &scale);
    void _DrawAttackingWithSword(const OWE::ScreenScale &scale);

private:
    glm::vec2 pos_;
    glm::vec2 texSize_;
    glm::vec2 vel_;
    glm::vec2 accVel_;

    glm::vec2 airFricAccVel_; //�������ٶ�

    /*
        �����ƶ�ֱ�Ӹ��ٶ�
        �������������ƶ��ɼ��ٶȸ���
    */

    float runningVel_;          //ˮƽ�ƶ��ٶ�
    float floatingAccVel_;      //�����ƶ����ٶ�

    float maxFloatingVel_;      //������ˮƽ�ƶ��ٶ�
    float floatingFricAccVel_;  //�����������ٶ�
    
    float jumpingVel_;          //��Ծ��ֱ��������ĳ��ٶ�
    float shiftingVel_;         //�����ٶ�

    State state_;
    Direction dir_;
    
    UserInput user_;
    EnvirInput envir_;

    Action act_;
    
    //���ֶ�����Դ

    Action::TexSeq actTexStanding_;
    Action::KpSeq  actKpStanding_;

    Action::TexSeq actTexRunning_;
    Action::KpSeq  actKpRunning_;

    Action::TexSeq actTexJumping_;
    Action::KpSeq  actKpJumping_;

    Action::TexSeq actTexShifting_;
    Action::KpSeq  actKpShifting_;

    Action::TexSeq actTexAttackingWithSword_;
    Action::KpSeq  actKpAttackingWithSword_;

    //Я��������

    Weapon *weapon_;

    //�����ķ���任

    TransSeq weaponTransAttackingWithSword_;

    //������Ϊ�Ƿ�������Ծʱ

    bool attackWhenFloating_;
};

#endif //__ACTOR_H__
