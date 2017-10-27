/*================================================================
Filename: Actor.h
Date: 2017.10.21
Created by AirGuanZ
================================================================*/

/* actor��weapon�����ô��ƣ�
    actor�б���Ķ�����Ҳ�и���weapon����Ķ���
    ��actor�Ķ�������weapon����actor����չ�ǳ�������
    ����Ӧ����actor�ṩ�����ֿɹ�ѡ�����weapon�Ķ�����weapon��֪actorӦ�����ĸ�
    ��weapon����̬��λ�ñ任�����ṩ����任���任������actor����weapon������
*/

#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>

#include <OWE.h>
#include "Weapon.h"

constexpr int ACTOR_WEAPON_CNT = 2;

class Actor : public OWE::Utility::Uncopyable
{
public:
    enum class State
    {
        Unknown,
        Standing,
        Walking,
        Floating,
        Attacking,
        Shifting,
        BeingAttacked
    };

    enum class Direction
    {
        Left, Right
    };

    Actor(void);
    ~Actor(void);

    void Initialize(void);
    void Reset(void);

    void SetMaxVelocity(float maxVel);
    float GetMaxVelocity(void) const;

    void SetMinVelocity(float minVel);
    float GetMinVelocity(void) const;

    void SetPosition(const glm::vec2 &pos);
    glm::vec2 GetPosition(void) const;

    void SetSize(const glm::vec2 &size);
    glm::vec2 GetSize(void) const;

    void SetVelocity(const glm::vec2 &vel);
    glm::vec2 GetVelocity(void) const;

    void SetAcceleratedVelocity(const glm::vec2 &pos);
    glm::vec2 GetAcceleratedVelocity(void) const;

    void SetState(State state);
    State GetState(void) const;

    void UpdateMoving(double deltaTime);
    void UpdateJumping(double deltaTime);

    void Draw(const OWE::ScreenScale &scale);

private:
    static bool _CanWalk(State state);
    static bool _CanJump(State state);

    float maxVelocity_;
    float minVelocity_;

    glm::vec2 position_;
    glm::vec2 size_;

    glm::vec2 velocity_;
    glm::vec2 acceleratedVelocity_;

    State state_;
    Direction dir_;

    //������ʱ��
    double aniTime_;
    int aniIdx_;

    //ÿ��������ΪtexSeq��kpSeq������
    //texSeq����һ������֡���У�����λ�ڻ��������²�
    //kpSeq[i]������i֡�Ľ���ʱ��

    //վ������
    std::vector<OWE::Texture2D> standingTexSeq_;
    std::vector<double> standingKpSeq_;

    //���߶���
    std::vector<OWE::Texture2D> walkingTexSeq_;
    std::vector<double> walkingKpSeq_;

    //��Ծ����
    std::vector<OWE::Texture2D> jumpingTexSeq_;
    std::vector<double> jumpingKpSeq_;

    static OWE::KEY_CODE keyLeft_;
    static OWE::KEY_CODE keyRight_;
    static OWE::KEY_CODE keyJump_;

    //�����ٶ�
    static float walkingSpeed_;
    static float jumpingSpeed_;
};

#endif //__ACTOR_H__
