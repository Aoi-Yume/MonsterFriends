//
// Created by 葵ユメ on 2018/08/25.
//


#include "Engine.h"
#include "../Net/TransferManager.h"
#include <SoundManager.h>

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

JNIEXPORT void JNICALL
Java_com_aoiyume_Game_Engine_AccecpNearEndPoint(JNIEnv *env, jclass clazz, jstring end_point,
												jstring name) {
	const char* pEndPoint = env->GetStringUTFChars(end_point, nullptr);
	const char* pName = env->GetStringUTFChars(name, nullptr);
	TransferManager::Get()->AddConnect(TransferManager::ConnectInfo(pEndPoint, pName, 0xFF));
	env->ReleaseStringUTFChars(end_point, pEndPoint);
	env->ReleaseStringUTFChars(name, pName);
}

JNIEXPORT void JNICALL
Java_com_aoiyume_Game_Engine_ReceiveNearbyData(JNIEnv *env, jclass clazz, jstring end_point,
											   jbyteArray data) {
	jsize size = env->GetArrayLength(data);
	jbyte* pData = env->GetByteArrayElements(data, nullptr);
	const char* pEndPoint = env->GetStringUTFChars(end_point, nullptr);
	TransferManager::Get()->AddReceiveData(pEndPoint, pData, size);
	env->ReleaseStringUTFChars(end_point, pEndPoint);
	env->ReleaseByteArrayElements(data, pData, 0);
}

JNIEXPORT void JNICALL
Java_com_aoiyume_Game_SoundManager_LoadCompleteCallBack(JNIEnv *env, jobject thiz, jint handle, jint status) {
	auto pSoundManager = Engine::GetEngine()->GetSoundManager();
	pSoundManager->LoadCompleteCallBack(handle, status);
}

END_EXTERN_C