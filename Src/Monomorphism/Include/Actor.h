/*================================================================
Filename: Actor.h
Date: 2017.10.21
Created by AirGuanZ
================================================================*/
#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>

#include <OWE.h>
#include "Weapon.h"

/* Actor��Ҫ
    ���� ���ٶ�-�ٶ�-λ�� ����·������Щ�����Ǹ����ٶȣ���Щ�����Ǹ��ٶ�
    ������״̬����
        Unknown                 ��������������ڵ��ϣ�
        Standing                ��ͨ��վ��
        Running                 ˮƽ�ƶ�
        Jumping                 ��Ծ
        BeingAttacked           ����
        Floating                �����÷��� Orz
        Lying                   ����
        ����״̬                ���ע���״̬��Ҫ���ת�ƺ���
    ���ܵ�������룺
        ���Ҽ�����Ծ������������
        �����˶���
            �����ٶ�
            �������ٶ�
            ������ٶ�
            ��������ٶ�
        �Ը��˶���
            �Ը��ٶ�
            �Ը����ٶ�

    �����Զ�ת��
        Floating��ؽ���Lying
        Jumping��ؽ���Unknown
        BeingAttacked��������Unknown
        Standing��Running�������ս���Jumping
        ���ע��״̬�Լ�����
    
    ��������ת��
        Unknown[�����ǿ�������������]            -> Floating
        Unknown[�������]                        -> BeingAttacked
        Unknown[��Ծ��]                            -> Jumping������Ծ��ֱ�ٶȣ��̳�ˮƽ�ٶ�
        Unknown[������]                            -> ��������ע���ת�ƺ���
        Unknown[�ƶ���]                            -> Running�����ƶ����ٶ�

        Standing[�����ǿ�������������]           -> Floating
        Standing[�������]                       -> BeingAttacked
        Standing[��Ծ��]                           -> Jumping������Ծ��ֱ�ٶȣ��̳�ˮƽ�ٶ�
        Standing[������]                           -> ��������ע���ת�ƺ���
        Standing[�ƶ���]                           -> Running�����ƶ����ٶ�

        Running[�����ǿ�������������]            -> Floating
        Running[�������]                        -> BeingAttacked
        Running[��Ծ��]                            -> Jumping������Ծ��ֱ�ٶȣ��̳�ˮƽ�ٶ�
        Running[������]                            -> ��������ע���״̬
        Running[�ƶ���]                            -> Running�����ƶ����ٶ�
        Running[����]                              -> Standing

        Jumping[�����ǿ�����/�������]         -> Floating
        Jumping[������]                            -> ��������ע���ת�ƺ���
        Jumping[�ƶ���]                            -> Jumping, ��ˮƽ���ٶ�

        BeingAttacked[�����ǿ�������������]      -> Floating
        BeingAttacked[�������]                  -> BeingAttacked�����ü�ʱ��

        Floating[�����ǿ�����/�������]        -> Floating

        Lying[�����ǿ�������������]              -> Floating
        Lying[�������]                          -> Lying�����ü�ʱ��

    ������ƶ��ͳ�����ײ��⡢�˺�����һ��Ѷ���
*/

class Actor : public OWE::Utility::Uncopyable
{
public:
    
private:

};

#endif //__ACTOR_H__
