/*================================================================
Filename: ResourceNames.h
Date: 2017.10.26
Created by AirGuanZ
================================================================*/

/*
    ��Դ·��ע��
*/

#ifndef __RESOURCE_NAMES_H__
#define __RESOURCE_NAMES_H__

#define MM_FILENAME(X) (_MM_FILENAME(X))
#define _MM_FILENAME(X) "Bin\\"##X

#define ACTOR_ANIMATION_STANDING_CONFIGURE MM_FILENAME("Actor\\AnimationStanding.conf")
#define ACTOR_ANIMATION_WALKING_CONFIGURE  MM_FILENAME("Actor\\AnimationWalking.conf")
#define ACTOR_ANIMATION_JUMPING_CONFIGURE  MM_FILENAME("Actor\\AnimationJumping.conf")

#endif //__RESOURCE_NAMES_H__
