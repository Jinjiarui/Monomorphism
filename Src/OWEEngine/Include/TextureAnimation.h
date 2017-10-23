/*================================================================
Filename: Animation.h
Date: 2017.10.22
Created by AirGuanZ
================================================================*/
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>

#include "ImmediateRenderer.h"
#include "ScreenScale.h"
#include "Texture2D.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_AnimationAux)

using TextureSeq = std::vector<Texture2DView>;
using KeypntsSeq = std::vector<float>;

//һ�鶯��������һ��(����ʱ��)����
//��texSeq����n��ͼ�񣬵�һ��ͼ���time = 0
//ʱ�̿�ʼ���ţ�kpSeq����ǰn - 1���л�ͼ���ʱ��㣬
//time >= kpSeq[n - 1]ʱά�����һ��ͼ��
struct _AnimationData
{
    TextureSeq texSeq;
    KeypntsSeq kpSeq;
};

//һ��������һ�鶯�������Լ�����ָʾ��ǰ����ʱ���time����
//ʱ��ĸ�����Ҫ�ֶ�����_Animation::Tick����
class _Animation
{
public:
    using Data = _AnimationData;

    //�Բ�Ϊ�յ����ݣ�������0 <= kpSeq.size() = texSeq.size() - 1
    _Animation(const Data *data = nullptr);

    void SetTexData(const Data *data);
    bool IsAvailable(void) const;
    const Data *GetTexData(void) const;
    
    glm::vec2 GetLBPosition(void) const;
    void SetLBPosition(const glm::vec2 &LB);

    glm::vec2 GetRTPosition(void) const;
    void SetRTPosition(const glm::vec2 &RT);

    void Move(const glm::vec2 &deltaPos);

    void Restart(void);
    void Tick(double deltaTime);

    double GetTime(void) const;

    void Draw(
        const ScreenScale &scale,
        ImmediateRenderer::RenderMode mode = ImmediateRenderer::RenderMode::AlphaTest,
        const ImmediateRenderer::RenderDesc &desc = ImmediateRenderer::RenderDesc(0.5f));

private:
    glm::vec2 LB_;
    glm::vec2 RT_;

    int idx_;
    double time_;
    const Data *data_;
};

__OWE_END_NAMESPACE__(_AnimationAux)

using Animation = _AnimationAux::_Animation;

__OWE_END_NAMESPACE__(OWE)

#include "TextureAnimation.inl"

#endif //__ANIMATION_H__
