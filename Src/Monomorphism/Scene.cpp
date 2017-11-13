﻿/*================================================================
Filename: Scene.cpp
Date: 2017.11.7
Created by AirGuanZ
================================================================*/
#include <set>

#include <glm\glm.hpp>
#include <OWE.h>

#include "Include\Scene.h"
#include "Include\Sword.h"

using namespace std;
using namespace glm;
using namespace OWE;

//==================================== 角色参数 ====================================

constexpr float ACTOR_RUNNING_VEL           = 0.012f;       //移动速度
constexpr float ACTOR_FLOATING_ACC_VEL      = 0.00006f;     //悬空时自给水平加速度
constexpr float ACTOR_SHIFTING_VEL          = 0.02f;        //闪避速度
constexpr float ACTOR_JUMPING_VEL           = 0.025f;       //跳跃初速度
constexpr float ACTOR_MAX_FLOATING_VEL      = 0.012f;       //悬空时最大水平速度
constexpr float ACTOR_FLOATING_FRIC_ACC_VEL = 0.00003f;     //悬空时空气阻力带来的水平加速度
constexpr float ACTOR_GRAVITY_ACC           = -0.00008f;    //重力加速度

constexpr KEY_CODE ACTOR_INPUT_KEY_MOVE_LEFT     = KEY_CODE::KEY_A;             //向左移动
constexpr KEY_CODE ACTOR_INPUT_KEY_MOVE_RIGHT    = KEY_CODE::KEY_D;             //向右移动
constexpr KEY_CODE ACTOR_INPUT_KEY_JUMP          = KEY_CODE::KEY_SPACE;         //跳跃
constexpr KEY_CODE ACTOR_INPUT_KEY_SHIFT         = KEY_CODE::KEY_LSHIFT;        //闪避
constexpr MOUSE_BUTTON ACTOR_INPUT_BUTTON_ATTACK = MOUSE_BUTTON::BUTTON_LEFT;   //攻击

//==================================================================================

Scene::Scene(void)
    : rc_(RenderContext::GetInstance()),
      im_(InputManager::GetInstance())
{
    
}

void Scene::AddBlockArea(BlockArea *area)
{
    assert(area);
    blockAreas_.insert(area);
    blockColMgr_.AddObject(area);
}

void Scene::AddCreature(Creature *creature)
{
    assert(creature);
    creatures_.insert(creature);
    creatureColMgr_.AddObject(creature);
}

void Scene::AddDamageArea(DamageArea *area)
{
    assert(area);
    damageAreas_.insert(area);
    damageColMgr_.AddObject(area);
}

void Scene::AddItem(Item *item)
{
    assert(item);
    items_.insert(item);
    itemColMgr_.AddObject(item);
}

void Scene::Initialize(void)
{
    leftBound_ = std::numeric_limits<float>::lowest();
    rightBound_ = std::numeric_limits<float>::max();

    actor_.Initialize();
    scale_.Reinit(35.0f, 35.0f);
    
    //初始化角色参数
    actor_.SetRunningVel        (ACTOR_RUNNING_VEL);
    actor_.SetFloatingAccVel    (ACTOR_FLOATING_ACC_VEL);
    actor_.SetShiftingVel       (ACTOR_SHIFTING_VEL);
    actor_.SetJumpingVel        (ACTOR_JUMPING_VEL);

    actor_.SetMaxFloatingVel    (ACTOR_MAX_FLOATING_VEL);
    actor_.SetFloatingFricAccVel(ACTOR_FLOATING_FRIC_ACC_VEL);

    actor_.GetPosition() = vec2(1.0f, 5.0f);
    actor_.GetTexSize()  = vec2(0.02f, 0.02f);

    actor_.SetWeapon(new Sword());
}

void Scene::SetBound(float left, float right)
{
    leftBound_ = left;
    rightBound_ = right;
}

Scene::RunningResult Scene::Run(void)
{
    clock_.Restart();
    while(true)
    {
        clock_.Tick();
        
        _UpdateActor();
        _UpdateBlockAreas();
        _UpdateCreatures();
        _UpdateItems();

        _InteractWithItems();

        _InteractWithDamageAreas();
        _UpdateDamageArea();
        
        //场景渲染
        rc_.ClearColorAndDepth();
        
        scale_.SetCentrePosition(actor_.GetPosition());

        _DrawBlockAreas();
        _DrawCreatures();
        actor_.Draw(scale_);
        _DrawDamageArea();

        rc_.Present();
        
        //外部输入处理
        rc_.DoEvents();
        
        if(im_.IsKeyPressed(KEY_CODE::KEY_ESCAPE))
            return RunningResult::Closed;
        if(actor_.GetPosition().x < leftBound_)
            return RunningResult::OutOfLeftBound;
        if(actor_.GetPosition().y > rightBound_)
            return RunningResult::OutOfRightBound;
    }
}

void Scene::_UpdateActor(void)
{
    actor_.ClearAccVel();
    actor_.Update(clock_.ElapsedTime());
    
    //清空角色输入
    actor_.ResetInput();
    
    //给予重力加速度并更新速度
    actor_.GetAccVelocity().y += ACTOR_GRAVITY_ACC;
    actor_.UpdateVelocity(clock_.ElapsedTime());

    //某个新的角色位置是否会发生碰撞
    auto testNewPos = [&](const vec2 &newPos) -> bool
    {
        actor_.GetPosition() = newPos;
        for(auto &area : actor_.GetBoundingAreas())
        {
            if(!blockColMgr_.CollisionWithBoundingArea(area).empty())
                return false;
        }
        return true;
    };

    //向无碰撞的方向移动
    vec2 oldPos    = actor_.GetPosition();
    vec2 deltaPos  = static_cast<float>(clock_.ElapsedTime()) * actor_.GetVelocity();
    if(!testNewPos(oldPos + deltaPos)) //新位置不行，考虑恢复x或y坐标
    {
        if(testNewPos(oldPos + vec2(deltaPos.x, 0.0f))) //先试试恢复y坐标
        {
            actor_.GetVelocity().y = 0.0f;
            //看看是脑袋撞了还是脚撞了
            if(deltaPos.y > 0.0f)
                actor_.GetEnvirInput().colUp = true;
            else
            {
                actor_.GetEnvirInput().colDown = true;
                //搜索一个合适的恢复位置
                constexpr float deltaY = 1e-3f;
                float dy = 0.0f;
                while(!testNewPos(oldPos + deltaPos + vec2(0.0f, dy)))
                    dy += deltaY;
            }
        }
        else //只恢复y坐标不行
        {
            if(testNewPos(oldPos + vec2(0.0f, deltaPos.y))) //再试试x坐标
            {
                actor_.GetVelocity().x = 0.0f;
                //看看是左边撞了还是右边撞了
                if(deltaPos.x > 0.0f)
                    actor_.GetEnvirInput().colRight = true;
                else
                    actor_.GetEnvirInput().colLeft = true;
            }
            else //看来只能直接恢复上一帧坐标了
            {
#ifdef _DEBUG
                assert(testNewPos(oldPos));
#else
                actor_.GetPosition() = oldPos;
#endif
            }
        }
    }

    //更新用户输入
    auto &userInput  = actor_.GetUserInput();
    userInput.left   = im_.IsKeyPressed(ACTOR_INPUT_KEY_MOVE_LEFT);
    userInput.right  = im_.IsKeyPressed(ACTOR_INPUT_KEY_MOVE_RIGHT);
    userInput.shift  = im_.IsKeyPressed(ACTOR_INPUT_KEY_SHIFT);
    userInput.jump   = keyJump_.Update(im_.IsKeyPressed(ACTOR_INPUT_KEY_JUMP));
    userInput.attack = keyAttack_.Update(im_.IsMouseButtonPressed(ACTOR_INPUT_BUTTON_ATTACK));
}

void Scene::_UpdateBlockAreas(void)
{
    //update
    for(BlockArea *pArea : blockAreas_)
        pArea->Update(clock_.ElapsedTime());

    //删去dead block
    std::set<BlockArea*> newBlockAreas;
    for(BlockArea *pArea : blockAreas_)
    {
        if(pArea->IsDead())
        {
            blockColMgr_.DelObject(pArea);
            delete pArea;
        }
        else
            newBlockAreas.insert(pArea);
    }
    blockAreas_ = std::move(newBlockAreas);
}

void Scene::_DrawBlockAreas(void)
{
    for(BlockArea *pArea : blockAreas_)
        pArea->Draw(scale_);
}

bool Scene::_CreatureTestCoi(const Creature *p)
{
    for(auto &area : p->GetBoundingAreas())
    {
        if(!blockColMgr_.CollisionWithBoundingArea(area).empty())
            return false;
    }
    return true;
}

void Scene::_UpdateCreatures(void)
{
    //update
    double timex = clock_.ElapsedTime();
    for(Creature *pCreature : creatures_)
        pCreature->Update(actor_.GetPosition(), timex);
    //向速度移动
    for(Creature *pCreature : creatures_)
    {
        glm::vec2 oldPosition = pCreature->GetPosition();
        glm::vec2 deltaPos = static_cast<float>(timex) * pCreature->GetVelocity();
        pCreature->SetPosition(oldPosition + deltaPos);

        if(!_CreatureTestCoi(pCreature))//新位置产生了碰撞
        {
            //尝试恢复y轴坐标
            pCreature->SetPosition(oldPosition + glm::vec2(deltaPos.x, 0.0f));
            if(_CreatureTestCoi(pCreature))//恢复成功了
            {
                pCreature->SetVelocity(glm::vec2(pCreature->GetVelocity().x, 0.0));//把y的速度变为0
                                                                                   //搜索一个合适的恢复位置(下落防止抖动)
                if(deltaPos.y < 0)
                {
                    constexpr float deltaY = 1e-2f;
                    float dy = 0.0f;
                    //testNewPos(oldPos + deltaPos + vec2(0.0f, dy)
                    do
                    {
                        pCreature->SetPosition(oldPosition + deltaPos + vec2(0.0f, dy));
                        dy += deltaY;
                    } while(!_CreatureTestCoi(pCreature));
                }
            }
            else
            {
                //需要恢复x坐标
                pCreature->SetPosition(oldPosition + glm::vec2(0.0f, deltaPos.y));
                if(_CreatureTestCoi(pCreature))
                {
                    pCreature->SetVelocity(glm::vec2(0.0f, pCreature->GetVelocity().y));
                }
                else
                {
                    pCreature->SetPosition(oldPosition);
                    pCreature->SetVelocity(glm::vec2(0.0f, 0.0f));
                    //直接恢复上一帧的状态
                }
            }
        }
    }

    //删去dead creature
    std::set<Creature*> newCreatures;
    for(Creature *pCreature : creatures_)
    {
        if(pCreature->IsDead())
        {
            //物品几率掉落

            /*int s = rand();
            s = glm::abs(s) % 100;
            if(s < 50)//add item;
            {
                std::string itempath = "";
                //Item = new  create new item;
            }*/
            creatureColMgr_.DelObject(pCreature);
            delete pCreature;
        }
        else
            newCreatures.insert(pCreature);
    }
    creatures_ = std::move(newCreatures);
}

void Scene::_DrawCreatures(void)
{
    for(Creature *pC : creatures_)
        pC->Draw(scale_);
}

void Scene::_InteractWithDamageAreas(void)
{
    //人的伤害判定
    {
        set<DamageArea*> as;
        for(const BoundingArea &area : actor_.GetBoundingAreas())
        {
            auto damageA = damageColMgr_.CollisionWithBoundingArea(area);
            copy(begin(damageA), end(damageA), inserter(as, begin(as)));
        }
        for(DamageArea *a : as)
            a->Damage(&actor_);
    }

    //其他生物的伤害判定
    for(Creature *c : creatures_)
    {
        set<DamageArea*> as;
        for(const BoundingArea &area : c->GetBoundingAreas())
        {
            auto damageA = damageColMgr_.CollisionWithBoundingArea(area);
            copy(begin(damageA), end(damageA), inserter(as, begin(as)));
        }
        for(DamageArea *a : as)
            a->Damage(c);
    }
}

void Scene::_UpdateDamageArea(void)
{
    //update
    for(DamageArea *pArea : damageAreas_)
        pArea->Update(clock_.ElapsedTime());

    //删去dead block
    std::set<DamageArea*> newDamageAreas;
    for(DamageArea *pArea : damageAreas_)
    {
        if(pArea->IsDead())
        {
            damageColMgr_.DelObject(pArea);
            delete pArea;
        }
        else
            newDamageAreas.insert(pArea);
    }
    damageAreas_ = std::move(newDamageAreas);
}

void Scene::_DrawDamageArea(void)
{
    for(DamageArea *area : damageAreas_)
        area->Draw(scale_);
}

void Scene::_UpdateItems(void)
{
    //Update
    for(Item *item : items_)
        item->Update(clock_.ElapsedTime());

    //删除死掉的item
    std::set<Item*> newItems;
    for(Item *item : items_)
    {
        if(item->IsDead())
        {
            itemColMgr_.DelObject(item);
            delete item;
        }
        else
            newItems.insert(item);
    }
    items_ = std::move(newItems);
}

void Scene::_DrawItems(void)
{
    for(Item *item : items_)
        item->Draw(scale_);
}

void Scene::_InteractWithItems(void)
{
    std::set<Item*> inItems;
    for(auto &area : actor_.GetBoundingAreas())
    {
        auto it = itemColMgr_.CollisionWithBoundingArea(area);
        for(auto *p : it)
            inItems.insert(p);
    }

    for(auto *p : inItems)
        p->InteractWithActor(&actor_);
}
