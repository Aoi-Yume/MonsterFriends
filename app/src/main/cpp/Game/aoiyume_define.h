//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_DEFINE_H
#define AOIYUME_DEFINE_H

#include <android/log.h>

#define CLASS_NAME_SYSTEM			"com/aoiyume/Game/GameMainFragment"
#define CLASS_NAME_UTILITY			"com/aoiyume/Game/Utility"

#define DEBUG_LOG(mes)			__android_log_print(ANDROID_LOG_DEBUG, "MonsterFriends", mes);
#define DEBUG_LOG_A(mes, ...)	__android_log_print(ANDROID_LOG_DEBUG, "MonsterFriends", mes, __VA_ARGS__);

#endif