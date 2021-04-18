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
#define DEBUG_ERROR(mes)		__android_log_print(ANDROID_LOG_FATAL, "MonsterFriends", mes);
#define DEBUG_ERROR_A(mes, ...)		__android_log_print(ANDROID_LOG_FATAL, "MonsterFriends", mes, __VA_ARGS__);

#define ASSERT_MSG_A(expression, mes, ...) if(!(expression)) { DEBUG_ERROR("Assert:"); DEBUG_ERROR_A(mes, __VA_ARGS__); assert(expression); }
#define ASSERT_MSG(expression, mes) if(!(expression)) { DEBUG_ERROR("Assert:"); DEBUG_ERROR(mes); assert(expression); }
#define ASSERT(expression) ASSERT_MSG(expression, #expression);

#endif