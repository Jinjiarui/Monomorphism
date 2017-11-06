/*================================================================
Filename: TestApp_ActorAttackingWithSword.h
Date: 2017.11.5
Created by AirGuanZ
================================================================*/
#ifndef __TEST_APP_ACTOR_ATTACKING_WITH_SWORD_H__
#define __TEST_APP_ACTOR_ATTACKING_WITH_SWORD_H__

#include <glm\glm.hpp>
#include <OWE.h>

#include "Actor.h"
#include "Sword.h"

namespace Test
{
    using namespace std;
    using namespace glm;
    using namespace OWE;

    class TestApp_ActorAttackingWithSword : public KeyboardListener
    {
        Actor actor_;

        ScreenScale scale_;
        Clock clock_;

        bool done_;
        bool space_;
        bool mouseLeft_;

    public:
        void KeyDown(KEY_CODE kc)
        {
            if(kc == KEY_CODE::KEY_ESCAPE)
                done_ = true;
        }

        void Run(void)
        {
            RenderContext::Desc desc;
            desc.bordered = true;
            desc.resizable = false;
            desc.fullscreen = false;
            desc.winWidth = 800;
            desc.winHeight = 600;
            desc.title = "Monomorphism";
            desc.glVersion = "4.3";

            //��ʼ��RenderContext
            RenderContext::InitInstance(desc);
            RenderContext &rc = RenderContext::GetInstance();
            InputManager &im = InputManager::GetInstance();
            im.AddKeyboardListener(this);

            //��ʼ����ɫ��Դ�Ͳ���
            actor_.Initialize();

            actor_.SetRunningVel(0.012f);
            actor_.SetFloatingAccVel(0.00006f);
            actor_.SetShiftingVel(0.02f);
            actor_.SetJumpingVel(0.025f);

            actor_.SetMaxFloatingVel(0.012f);
            actor_.SetFloatingFricAccVel(0.00003f);

            actor_.GetPosition() = vec2(3.0f, 2.0f);
            actor_.GetTexSize() = vec2(0.02f, 0.02f);

            rc.SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            scale_.Reinit(40.0f, 40.0f);
            done_ = false;
            space_ = false;
            mouseLeft_ = false;
            clock_.Restart();

            actor_.SetWeapon(new Sword());

            while(!done_)
            {
                clock_.Tick();

                actor_.ClearAccVel();
                actor_.Update(clock_.ElapsedTime());

                //���actor����
                actor_.GetUserInput().Reset();
                actor_.GetEnvirInput().Reset();

                //�����������ٶ�
                actor_.GetAccVelocity().y += -0.00008f;
                actor_.UpdateVelocity(clock_.ElapsedTime());

                //�ƶ�����ײ���
                actor_.GetPosition() += actor_.GetVelocity() *
                    static_cast<float>(clock_.ElapsedTime());
                if(actor_.GetPosition().y < 1.0f)
                {
                    actor_.GetPosition().y = 1.0f;
                    actor_.GetVelocity().y = 0.0f;
                    actor_.GetEnvirInput().colDown = true;
                }

                //��ȡ�û�����
                if(im.IsKeyPressed(KEY_CODE::KEY_A))
                    actor_.GetUserInput().left = true;
                if(im.IsKeyPressed(KEY_CODE::KEY_D))
                    actor_.GetUserInput().right = true;
                if(im.IsKeyPressed(KEY_CODE::KEY_LSHIFT))
                    actor_.GetUserInput().shift = true;
                if(im.IsKeyPressed(KEY_CODE::KEY_SPACE))
                {
                    if(!space_)
                    {
                        actor_.GetUserInput().jump = true;
                        space_ = true;
                    }
                }
                else space_ = false;
                if(im.IsMouseButtonPressed(MOUSE_BUTTON::BUTTON_LEFT))
                {
                    if(!mouseLeft_)
                    {
                        actor_.GetUserInput().attack = true;
                        mouseLeft_ = true;
                    }
                }
                else mouseLeft_ = false;

                //scale_.SetCentrePosition(actor_.GetPosition() + vec2(0.0f, 3.0f));

                rc.ClearColorAndDepth();
                actor_.Draw(scale_);
                rc.DoEvents();
                rc.Present();
            }
        }
    };
}

#endif //__TEST_APP_ACTOR_ATTACKING_WITH_SWORD_H__
