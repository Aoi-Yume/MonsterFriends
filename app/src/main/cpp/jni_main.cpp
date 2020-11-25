//
// Created by 葵ユメ on 2018/08/20.
//

#include "Engine.h"

extern "C"
{

jint JNI_OnLoad(JavaVM *vm, void *reserved) {

	JNIEnv *env;
	if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
		return JNI_ERR;
	}
	DEBUG_LOG("Call JNI_OnLoad");

	SetVM(vm);

	Engine::Create();
	ObjectLoader::Create();
	ObjectLoader::GetObjectLoader()->SetDefaultLoad();

	jclass classId = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_UTILITY);
	jmethodID methodId =  ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_UTILITY, "GetAssetManager");
	Engine::GetEngine()->SetAssetManager(GetEnv()->CallStaticObjectMethod(classId, methodId));

	return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	DEBUG_LOG("Call JNI_OnUnLoad");
	Engine::GetEngine()->DeleteAssetManager();
	ObjectLoader::Destroy();
	Engine::Destroy();
}
	
}