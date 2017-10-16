/*================================================================
Filename: ScreenScale.h
Date: 2017.10.15
Created by AirGuanZ
================================================================*/

//���ڳߴ��OpenGL��Ļ�����Ǹ��ѵģ�ֱ���������е��κ�һ����̫��Ӳ
//Scaleϵͳ���ڽ���һ���µ�����ϵͳ��������ϵ����

#ifndef __OWE_SCREEN_SCALE_H__
#define __OWE_SCREEN_SCALE_H__

#include <cassert>
#include <glm\glm.hpp>

#include "Common.h"
#include "RenderContext.h"

__OWE_BEGIN_NAMESPACE__(OWE)

//Screen����ϵ�����½�Ϊ(0, 0)���̶���user�ṩ��xpp��yppָ��
//GL����ϵ��OpenGL��Ļ����ϵ�����½�Ϊ(-1, -1)�����Ͻ�Ϊ(1, 1)
//Client����ϵ����������ϵ�����Ͻ�Ϊ(0, 0)��y�����������£��̶�Ϊ����

class ScreenScale
{
public:
    ScreenScale(void);
    ScreenScale(float xpp, float ypp);
    ~ScreenScale(void);

    //���ڴ�С�ı�������³�ʼ����ϵͳ

    void Reinit(float xpp, float ypp);

    float GetXPerPixel(void) const;
    float GetYPerPixel(void) const;

    float ScreenWidth(void) const;
    float ScreenHeight(void) const;

    float ScreenToGLX(float x) const;
    float ScreenToClientX(float x) const;

    float GLToScreenX(float x) const;
    float ClientToScreenX(float x) const;

    float ScreenToGLY(float y) const;
    float ScreenToClientY(float y) const;

    float GLToScreenY(float y) const;
    float ClientToScreenY(float y) const;

    glm::vec2 ScreenToGL(const glm::vec2 &screenCoord) const;
    glm::vec2 ScreenToClient(const glm::vec2 &screenCoord) const;

    glm::vec2 GLToScreen(const glm::vec2 &GLCoord) const;
    glm::vec2 ClientToScreen(const glm::vec2 &clientCoord) const;

    glm::mat3 ProjMatrix(void) const;

private:
    float xpp_;
    float ypp_;

    int clientWidth_;
    int clientHeight_;
    float screenWidth_;
    float screenHeight_;
};

__OWE_END_NAMESPACE__(OWE)

#include "ScreenScale.inl"

#endif //__OWE_SCREEN_SCALE_H__
