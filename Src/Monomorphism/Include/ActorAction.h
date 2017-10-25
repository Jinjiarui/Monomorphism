/*================================================================
Filename: ActorAction.h
Date: 2017.10.24
Created by AirGuanZ
================================================================*/
#ifndef __ACTOR_ACTION_H__
#define __ACTOR_ACTION_H__

#include <string>
#include <vector>

#include <glm\glm.hpp>
#include <OWE.h>

//һ��������һϵ��������岿���ķ���任
//kpSeq[i]ָ���任transSeq[i]�Ľ���ʱ��
//Ҫ��transSeq.size() = kpSeq.size()
struct ActorActionData
{
    using TransSeq = std::vector<glm::mat3>;
    using KeypntsSeq = std::vector<float>;

    TransSeq transSeq;
    KeypntsSeq kpSeq;

    /* �ļ���ʽ
        (float float^9 \n)+
       ÿ�е�һ��floatֵΪkpֵ�����9��floatֵ��������任������������ʾ
    */
    bool LoadFromFile(const std::string &filename);
};

//����Ȩ�޺÷� >_< �ɴ�struct��
struct ActorAction : public ActorActionData
{
    void Tick(float deltaTime);
    void Restart(void);
    bool End(void) const;

    float time = 0.0f;
    int idx;
};

#endif //__ACTOR_ACTION_H__
