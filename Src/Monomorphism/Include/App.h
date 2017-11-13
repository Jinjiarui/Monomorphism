/*================================================================
Filename: App.h
Date: 2017.11.13
Created by AirGuanZ
================================================================*/
#ifndef __APP_H__
#define __APP_H__

#include <OWE.h>

#include "World.h"

class App
{
public:
    void Run(void)
    {
        //��ʼ��RenderContext
        OWE::RenderContext::Desc desc;
        desc.bordered = true;
        desc.resizable = false;
        desc.fullscreen = false;
        desc.winWidth = 800;
        desc.winHeight = 600;
        desc.title = "Monomorphism";
        desc.glVersion = "4.3";

        OWE::RenderContext::InitInstance(desc);
        OWE::RenderContext &rc = OWE::RenderContext::GetInstance();
        OWE::InputManager &im  = OWE::InputManager::GetInstance();

        //��������
        World::InitInstance();
        World::GetInstance().InitializeScene(128, 0);
        World::GetInstance().Run();
    }
};

#endif //__APP_H__
