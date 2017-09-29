#include <iostream>

#include "Include\ASCIIFile.h"
#include "Include\GLHeaders.h"
#include "Include\InputManager.h"
#include "Include\RenderContext.h"
#include "Include\Shader.h"

#include "Include\UniformVariable.h"

using namespace std;
using namespace OWE;

class App : public WindowListener
{
public:
    int run(void)
    {
        RenderContext::Desc desc;
        desc.bordered = true;
        desc.resizable = false;
        desc.winWidth = 800;
        desc.winHeight = 600;
        desc.title = "Monomorphism";

        //��ʼ��RenderContext
        RenderContext::InitInstance(desc);
        RenderContext &rc = RenderContext::GetInstance();
        InputManager &im = InputManager::GetInstance();

        rc.AddWindowListener(this);

        //����shader�ļ�
        Shader shader;
        string vtx, frag, err;
        Utility::ASCIIFile::Read("test.vs", vtx);
        Utility::ASCIIFile::Read("test.fs", frag);
        if(shader.Initialize(err, VSSrc(vtx), FSSrc(frag)) != Shader::InitState::Success)
        {
            cout << err << endl;
            return -1;
        }

        //��ѭ��
        while(!closed_)
        {
            glClearColor((abs(glm::sin(t_ += 0.08f)) + 1.0f) / 2.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.Bind();
            //Draw something
            shader.Unbind();

            rc.DoEvents();
            rc.Present();

            if(im.IsKeyPressed(KEY_CODE::KEY_ESCAPE))
                closed_ = true;
        }

        shader.Destroy();
        return 0;
    }

protected:
    bool closed_ = false;
    float t_ = 0.0;

    void WindowClosed(void)
    {
        closed_ = true;
    }
};

int main(void)
{
    App app;
    return app.run();
}