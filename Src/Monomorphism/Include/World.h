/*================================================================
Filename: World.h
Date: 2017.11.12
Created by AirGuanZ
================================================================*/
#ifndef __WORLD_H__
#define __WORLD_H__

#include <OWE.h>

#include "Scene.h"
#include "SceneGenerator.h"
#include "TextureManager.h"

class World : public OWE::Utility::Singleton<World>
{
public:
    using StageNumber = int;

    TextureManager &GetTextureManager(void);

    void InitializeScene(SceneGenerator::SeedType worldSeed, StageNumber stage);

    void Run(void);

    Scene &GetCurrentScene(void);

private:
    World(void);

    void _InitializeResources(void);

    void _Save(void);

private:
    friend class SingletonType;

    SceneGenerator::SeedType worldSeed_;

    //�ؿ����Ϊ�Ǹ�������ż������浵�㣬��������ս���ؿ�
    StageNumber stage_;
    
    float leftBound_;
    float rightBound_;
    Scene *scene_;

    TextureManager texMgr_;
};

#endif //__WORLD_H__
