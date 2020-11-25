//
// Created by 葵ユメ on 2018/08/25.
//


#include "Engine.h"

EXTERN_C

//--------------------------------------------------------
JNIEXPORT void JNICALL
Java_com_aoiyume_Game_Engine_SetTouchInputInfo(
    JNIEnv *env, jobject jobj,
	int nEvent, float fTouchX, float fTouchY
)
{
	Engine::GetEngine()->SetTouchInputInfo(nEvent, fTouchX, fTouchY);
}
	
END_EXTERN_C