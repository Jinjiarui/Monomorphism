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
    ~Scene(void);
    
    void AddBlockArea(BlockArea *area);
    void AddCreature(Creature *creature);
    void AddDamageArea(DamageArea *area);
    void AddItem(Item *item);

    void Initialize(void);
    void SetBound(float left, float right);

    void SetBackgroundColor(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f);
    void SetTiledBackground(OWE::TiledTexture *bkgd);

    RunningResult Run(void);

    Actor &GetActor(void);

private:
    void _InitializeShader(void);

    void _UpdateActor(void);

    void _UpdateBlockAreas(void);
    void _DrawBlockAreas(void);
    void _DrawBlockAreasLight(void);

    bool _CreatureTestCoi(const Creature *);
    void _UpdateCreatures(void);
    void _DrawCreatures(void);

    void _InteractWithDamageAreas(void);
    void _UpdateDamageArea(void);
    void _DrawDamageArea(void);

    void _UpdateItems(void);
    void _DrawItems(void);

    void _InteractWithItems(void);

    void _DrawActorState(void);

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

    //�ϳ������
    OWE::FrameBuffer fbBasic_; //������ɫ
    OWE::FrameBuffer fbLight_; //��Դ����

    OWE::Shader chainShader_;

    OWE::Shader::UniformMgrPtr chainUniforms_;
    OWE::Shader::AttribMgrPtr  chainAttribs_;

    OWE::UniformVariable<OWE::Texture2DView> chainBasicTex_;
    OWE::UniformVariable<OWE::Texture2DView> chainLightTex_;

    OWE::VertexBuffer<glm::vec2> chainVtxPosBuf_;
    OWE::VertexBuffer<glm::vec2> chainVtxUVBuf_;

    //����ɫ
    glm::vec4 bkgdColor_;

    //����tiled texture
    OWE::TiledTexture *tiledBackground_;
};

#endif //__SCENE_H__
