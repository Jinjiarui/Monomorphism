/*================================================================
Filename: Scene.h
Date: 2017.11.7
Created by AirGuanZ
================================================================*/
#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include <glm\glm.hpp>
#include <OWE.h>

#include "Actor.h"
#include "BlockArea.h"
#include "Creature.h"
#include "DamageArea.h"
#include "Item.h"
#include "KeyDown.h"

class Scene
{
public:
    template<typename T>
    using CollisionManager = OWE::SimpleCollisionManager<T>;
    
    Scene(void);

    void Initialize(void);
    void Run(void);

private:
    void _UpdateActor(void);

private:
    OWE::RenderContext &rc_;
    OWE::InputManager &im_;

    //ʱ�ӡ���Ļ����ϵ�ͽ�����־
    OWE::Clock clock_;
    OWE::ScreenScale scale_;
    bool mainLoopDone_;

    //��ɫ
    Actor actor_;

    //��ɫ�������
    KeyDown keyJump_;
    KeyDown keyAttack_;

    //����Ԫ�ؼ���
    std::vector<BlockArea*>  blockAreas_;
    std::vector<Creature*>   creatures_;
    std::vector<DamageArea*> damageAreas_;
    std::vector<Item*>       items_;

    //��ײ�����
    CollisionManager<BlockArea>  blockColMgr_;
    CollisionManager<Creature>   creatureColMgr_;
    CollisionManager<DamageArea> damageColMgr_;
    CollisionManager<Item>       itemColMgr_;
};

#endif //__SCENE_H__
