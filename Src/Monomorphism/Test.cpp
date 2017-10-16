#include <iostream>

#include <OWE.h>

using namespace std;
using namespace OWE;
using Utility::ASCIIFile;

class App : public WindowListener
{
public:
    int run(void)
    {
        RenderContext::Desc desc;
        desc.bordered   = true;
        desc.resizable  = false;
        desc.fullscreen = false;
        desc.winWidth   = 1440;
        desc.winHeight  = 768;
        desc.title      = "Monomorphism";
        desc.glVersion  = "4.3";

        //��ʼ��RenderContext
        RenderContext::InitInstance(desc);
        RenderContext &rc = RenderContext::GetInstance();
        InputManager &im = InputManager::GetInstance();

        rc.AddWindowListener(this);

        //����shader�ļ�
        Shader shader;
        string vtx, frag, err;
        ASCIIFile::Read("test.vs", vtx);
        ASCIIFile::Read("test.fs", frag);
        if(shader.Initialize(err, VSSrc(vtx), FSSrc(frag)) != Shader::InitState::Success)
        {
            cout << err << endl;
            return -1;
        }

        //׼��vertex buffer
        VertexBuffer<glm::vec4> vec4Buf;
        const glm::vec4 vec4BufData[] =
        {
            { -0.5f, -0.5f, 0.0f, 1.0f },
            { 0.0f, 0.5f, 0.0f, 1.0f },
            { 0.5f, -0.5f, 0.0f, 1.0f }
        };
        vec4Buf.Initialize(3, vec4BufData);

        VertexBuffer<glm::vec2> uvBuf;
        const glm::vec2 uvBufData[] =
        {
            { 0.0, 0.0 },
            { 0.5, 1.0 },
            { 1.0, 0.0 }
        };
        uvBuf.Initialize(3, uvBufData);

        Texture2D tex;
        if(!LoadTexture2DFromFile("testTex.png", Texture2D::Desc(), tex))
        {
            cout << "Failed to load testTex.png" << endl;
            return -1;
        }

        //׼��uniform variable
        UniformVariableManager uniformMgr = shader.CreateUniformMgr();
        UniformVariable<GLint> texSam = uniformMgr.GetUniform<GLint>("tex");

        //׼����������
        AttribVariableManager attribMgr = shader.CreateAttribMgr();
        AttribVariable<glm::vec4> pos = attribMgr.GetAttrib<glm::vec4>("position");
        AttribVariable<glm::vec2> uv = attribMgr.GetAttrib<glm::vec2>("uv_in");
        pos.SetBuffer(vec4Buf);
        uv.SetBuffer(uvBuf);

        FrameBuffer fb;
        fb.Initialize(100, 100, 1);
        fb.AddTex(0, Texture2D::Desc());
        fb.AddDepth();
        if(!fb.IsAvailable())
        {
            cout << "Failed to initialize framebuffer" << endl;
            return -1;
        }

        //��ѭ��
        while(!closed_)
        {
            glClearColor((abs(glm::sin(t_ += 0.08f)) + 1.0f) / 2.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //��Ⱦ������
            fb.Begin();
            {
                glClearColor((abs(glm::sin(t_)) + 1.0f) / 2.0f, 1.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                tex.Bind(0);

                shader.Bind();
                attribMgr.Bind();

                uniformMgr.Apply();
                glDrawArrays(GL_TRIANGLES, 0, 3);

                attribMgr.Unbind();
                shader.Unbind();
            }
            fb.End();

            //��Ⱦ������
            fb.GetTex(0).Bind(0);

            shader.Bind();
            attribMgr.Bind();

            uniformMgr.Apply();
            glDrawArrays(GL_TRIANGLES, 0, 3);

            attribMgr.Unbind();
            shader.Unbind();

            rc.DoEvents();
            rc.Present();

            if(im.IsKeyPressed(KEY_CODE::KEY_ESCAPE))
                closed_ = true;
        }

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
