/*================================================================
Filename: TestApp3.h
Date: 2017.10.19
Created by AirGuanZ
================================================================*/

/*
    ����TiledTexture�Ĳ���
*/

#ifndef __TEST_APP_3_H__
#define __TEST_APP_3_H__

#include <glm\glm.hpp>
#include <OWE.h>

namespace Test
{
    using namespace std;
    using namespace glm;
    using namespace OWE;

    class TestApp3 : public KeyboardListener
    {
    public:
        void Run(void)
        {
            RenderContext::Desc desc;
            desc.bordered = true;
            desc.resizable = false;
            desc.fullscreen = false;
            desc.winWidth = 1440;
            desc.winHeight = 768;
            desc.title = "Monomorphism";
            desc.glVersion = "4.3";

            //��ʼ��RenderContext
            RenderContext::InitInstance(desc);
            RenderContext &rc = RenderContext::GetInstance();
            InputManager &im = InputManager::GetInstance();
            im.AddKeyboardListener(this);

            //��Ϊ���ԣ�������һ����������Ϊtileʹ��
            if(!LoadTexture2DFromFile("TestApp3/tex.png", Texture2D::Desc(), tex_))
            {
                cout << "Failed to load background texture from file" << endl;
                return;
            }

            //���ø�tile������Ϣ
            constexpr int BKGD_WIDTH = 10;
            constexpr int BKGD_HEIGHT = 10;
            bkgd_.Initialize(BKGD_WIDTH, BKGD_HEIGHT, 20.0f, 20.0f);
            for(int y = 0; y != BKGD_HEIGHT; ++y)
            {
                for(int x = 0; x != BKGD_WIDTH; ++x)
                {
                    bkgd_.SetTile(x, y,
                        vec2(1.0f / BKGD_WIDTH * x,       1.0f / BKGD_HEIGHT * y),
                        vec2(1.0f / BKGD_WIDTH * (x + 1), 1.0f / BKGD_HEIGHT * (y + 1)),
                        tex_);
                }
            }

            //ʹ�ô���alpha test�Ļ���ģʽ
            //����alpha����С��0.5f������
            bkgd_.SetRenderMode(TiledTexture::RenderMode::AlphaTest);
            bkgd_.SetAlphaThreshold(0.5f);
            bkgd_.SetPosition(vec2(-20.0f, -20.0f));

            scale_.Reinit(10.0f, 10.0f);
            pos_ = vec2(0.0f, 0.0f);

            done_ = false;
            clock_.Restart();
            glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
            rc.SetClearColor(0.0f, 1.0f, 1.0f, 1.0f);
            while(!done_)
            {
                clock_.Tick();
                rc.ClearColorAndDepth();

                constexpr float SPEED = 50.0f / 1000.0f;
                if(InputManager::GetInstance().IsKeyPressed(KEY_CODE::KEY_W))
                    pos_ += vec2(0.0f, SPEED * clock_.ElapsedTime());
                if(InputManager::GetInstance().IsKeyPressed(KEY_CODE::KEY_A))
                    pos_ += vec2(-SPEED * clock_.ElapsedTime(), 0.0f);
                if(InputManager::GetInstance().IsKeyPressed(KEY_CODE::KEY_S))
                    pos_ += vec2(0.0f, -SPEED * clock_.ElapsedTime());
                if(InputManager::GetInstance().IsKeyPressed(KEY_CODE::KEY_D))
                    pos_ += vec2(SPEED * clock_.ElapsedTime(), 0.0f);

                bkgd_.Draw(pos_ - 0.5f * vec2(scale_.ScreenWidth(), scale_.ScreenHeight()), scale_);

                rc.DoEvents();
                rc.Present();
            }
        }

        void KeyDown(KEY_CODE kc)
        {
            if(kc == KEY_CODE::KEY_ESCAPE)
                done_ = true;
        }

    private:
        TiledTexture bkgd_;
        Texture2D tex_;

        Clock clock_;
        ScreenScale scale_;

        vec2 pos_;

        bool done_;
    };
}

#endif //__TEST_APP_3_H__
