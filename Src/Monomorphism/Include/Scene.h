/*================================================================
Filename: Scene.h
Date: 2017.11.7
Created by AirGuanZ
================================================================*/
#ifndef __SCENE_H__
#define __SCENE_H__

#include <limits>
#include <set>

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

    enum RunningResult
    {
        Closed,
        OutOfLeftBound,
        OutOfRightBound
    };
    
    Scene(void);
    
    void AddBlockArea(BlockArea *area);
    void AddCreature(Creature *creature);
    void AddItem(Item *item);

    void Initialize(float leftBound  = std::numeric_limits<float>::lowest(),
                    float rightBound = std::numeric_limits<float>::max());
    RunningResult Run(void);

private:
    void _UpdateActor(void);

    void _UpdateBlockAreas(void);
    void _DrawBlockAreas(void);

    void _UpdateCreatures(void);
    void _DrawCreatures(void);

    void _UpdateItems(void);
    void _DrawItems(void);

    void _InteractWithItems(void);

private:
    OWE::RenderContext &rc_;
    OWE::InputManager &im_;

    //�������ұ߽�
    float leftBound_;
    float rightBound_;

    //ʱ�ӡ���Ļ����ϵ
    OWE::Clock clock_;
    OWE::ScreenScale scale_;

    //��ɫ
    Actor actor_;

    //��ɫ�������
    KeyDown keyJump_;
    KeyDown keyAttack_;

    //����Ԫ�ؼ���
    std::set<BlockArea*>  blockAreas_;
    std::set<Creature*>   creatures_;
    std::set<DamageArea*> damageAreas_;
    std::set<Item*>       items_;

    //��ײ�����
    CollisionManager<BlockArea>  blockColMgr_;
    CollisionManager<Creature>   creatureColMgr_;
    CollisionManager<DamageArea> damageColMgr_;
    CollisionManager<Item>       itemColMgr_;
};

#endif //__SCENE_H__
