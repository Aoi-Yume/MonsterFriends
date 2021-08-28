//
// Created by 葵ユメ on 2018/09/22.
//


#ifndef AOIYUME_BASE_DEFINE_H
#define AOIYUME_BASE_DEFINE_H

#include "SoundDefine.h"

enum ComponentKind {
    eComponentKind_Transform,
	eComponentKind_Collision,
    eComponentKind_Camera,
    eComponentKind_Layout,
    eComponentKind_Animation,
    eComponentKind_Max
};

enum GameMessage {
    eGameMessage_Setup,
    eGameMessage_Sync,
    eGameMessage_Update,
	eGameMessage_HitCollision,
    eGameMessage_PostUpdate,
    eGameMessage_ChangeCamera,
    eGameMessage_Draw,
    eGameMessage_Pause,
    eGameMessage_Destroy,
    eGameMessage_Max
};

enum GameMessageResult {
	eGameMessageResult_Break,
	eGameMessageResult_Continue,
	eGameMessageResult_Max
};

enum PermissionResult {
	eNone,
	eSuccess,
	eFailed,
	eDetailExplain,
};


#endif