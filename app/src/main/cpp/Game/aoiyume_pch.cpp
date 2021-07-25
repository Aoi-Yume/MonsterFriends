//
// Created by 葵ユメ on 2018/08/20.
//


#include "aoiyume_pch.h"

#include <map>
#include <string>

using namespace std;

static ObjectLoader* s_ObjectLoader = nullptr;

EXTERN_C
END_EXTERN_C


class ObjectLoaderInner
{
    friend class ObjectLoader;
public:
    ObjectLoaderInner(){}
    ~ObjectLoaderInner(){}

public:
    struct ObjectInfo{
        jclass  				m_ClassInfo;
        map<string, jmethodID > m_Method;
        map<string, jfieldID>	m_Filed;
    };
private:
    map<string, ObjectInfo>		m_ObjectInfo;
};
//
ObjectLoader::ObjectLoader()
: m_pInner(new ObjectLoaderInner())
{
}

//
ObjectLoader::~ObjectLoader()
{
    delete m_pInner;
}

//	
void ObjectLoader::Create()
{
	if(s_ObjectLoader){ return; }
	
	s_ObjectLoader = new ObjectLoader();
}

//
void ObjectLoader::Destroy()
{
	delete s_ObjectLoader;
}

//
ObjectLoader* ObjectLoader::GetObjectLoader()
{
    return s_ObjectLoader;
}

//	
void ObjectLoader::SetClassID(const char* className)
{
    JNIEnv* env = GetEnv();
	auto it = m_pInner->m_ObjectInfo.find(className);
	if(it == m_pInner->m_ObjectInfo.end()){
        // TODO DeleteGlobalRef
        m_pInner->m_ObjectInfo[className].m_ClassInfo = (jclass)env->NewGlobalRef(env->FindClass(className));
        CheckEnvThrow();
	}
}

//	
jclass ObjectLoader::GetClassID(const char* className)
{
	auto it = m_pInner->m_ObjectInfo.find(className);
	ASSERT(it != m_pInner->m_ObjectInfo.end());
	return (*it).second.m_ClassInfo;
}

void ObjectLoader::SetFieldID(const char *className, const char *fieldName, const char* sig)
{
    JNIEnv* env = GetEnv();
    jclass classID = GetClassID(className);

    auto it = m_pInner->m_ObjectInfo[className].m_Filed.find(fieldName);
    if(it == m_pInner->m_ObjectInfo[className].m_Filed.end()){
        m_pInner->m_ObjectInfo[className].m_Filed[fieldName] = env->GetFieldID(classID, fieldName, sig);
        CheckEnvThrow();
    }
}

//
jfieldID ObjectLoader::GetFieledID(const char* className, const char* fieldName)
{
	jclass classID = GetClassID(className);

    auto it = m_pInner->m_ObjectInfo[className].m_Filed.find(fieldName);
	ASSERT(it != m_pInner->m_ObjectInfo[className].m_Filed.end());

    return (*it).second;
}

void ObjectLoader::SetEnumID(const char *className, const char *enumName, const char* sig)
{
    JNIEnv* env = GetEnv();
    jclass  classID = GetClassID(className);

    auto it = m_pInner->m_ObjectInfo[className].m_Filed.find(enumName);
    if(it == m_pInner->m_ObjectInfo[className].m_Filed.end()){
        m_pInner->m_ObjectInfo[className].m_Filed[enumName] = env->GetStaticFieldID(classID, enumName, sig);
        CheckEnvThrow();
    }
}

jfieldID ObjectLoader::GetEnumID(const char *className, const char *enumName)
{
    jclass classID = GetClassID(className);

    auto it = m_pInner->m_ObjectInfo[className].m_Filed.find(enumName);
	ASSERT(it != m_pInner->m_ObjectInfo[className].m_Filed.end());
    return (*it).second;
}

jobject  ObjectLoader::GetEnumObj(const char *className, const char *enumName)
{
    JNIEnv* env = GetEnv();
    jclass  classID = GetClassID(className);
    jfieldID enumID = GetEnumID(className, enumName);
    jobject obj = env->GetStaticObjectField(classID, enumID);
    CheckEnvThrow();
    return obj;
}

void ObjectLoader::SetMethodID(const char *className, const char *methodName, const char *sig, bool isStatic)
{
    JNIEnv* env = GetEnv();
    jclass classID  = GetClassID(className);

    auto it = m_pInner->m_ObjectInfo[className].m_Method.find(methodName);
    if(it == m_pInner->m_ObjectInfo[className].m_Method.end()){
		jmethodID methodID = nullptr;
		if(isStatic){
			methodID= env->GetStaticMethodID(classID, methodName, sig);
		}
		else{
			methodID = env->GetMethodID(classID, methodName, sig);
		}
		m_pInner->m_ObjectInfo[className].m_Method[methodName] = methodID;
        CheckEnvThrow();
    }
}

jmethodID ObjectLoader::GetMethodID(const char *className, const char *methodName)
{
    jclass classID = GetClassID(className);

    auto it = m_pInner->m_ObjectInfo[className].m_Method.find(methodName);
	ASSERT(it != m_pInner->m_ObjectInfo[className].m_Method.end());

    return (*it).second;
}

void ObjectLoader::SetDefaultLoad() {
    DEBUG_LOG("/////////////////////////////////////////");
    DEBUG_LOG("Begin Load Default Object");
	{
		DEBUG_LOG("\tLoad System");
		s_ObjectLoader->SetClassID(CLASS_NAME_SYSTEM);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM, "ShowSoftwareKeyboard", "()V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM, "IsShowSoftwareKeyboard", "()Z", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM, "GetInputText", "()Ljava/lang/String;", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM, "StartNearbyAdvertising", "(Ljava/lang/String;)V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM,"StopNearbyAdvertising", "()V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM, "StartNearbyDiscovery", "(Ljava/lang/String;)V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM,"StopNearbyDiscovery", "()V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM,"DisconnectNearbyAllEndPoint", "()V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM,"SendData", "(Ljava/lang/String;[B)V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM, "LoadAds", "()V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM, "ShowAds", "()V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SYSTEM, "IsShowAds", "()Z", true);
	}
	// Class SoundManager
	{
		DEBUG_LOG("\tLoad SoundManager");
		s_ObjectLoader->SetClassID(CLASS_NAME_SOUND);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SOUND, "Initialize", "()V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SOUND, "Finalize", "()V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SOUND, "LoadSE", "(Ljava/lang/String;)I", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SOUND, "UnLoadSE", "(I)V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SOUND, "IsLoadSE", "(I)Z", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SOUND, "PlaySE", "(IFFZ)V", true);
		s_ObjectLoader->SetMethodID(CLASS_NAME_SOUND, "StopSE", "(I)V", true);
	}
    // Class Utility
    {
        DEBUG_LOG("\tLoad Utility");
        s_ObjectLoader->SetClassID(CLASS_NAME_UTILITY);
		s_ObjectLoader->SetMethodID(CLASS_NAME_UTILITY, "GetAssetManager", "()Ljava/lang/Object;", true);
        s_ObjectLoader->SetMethodID(CLASS_NAME_UTILITY, "LoadImage", "(Ljava/lang/String;)Ljava/lang/Object;", true);
        s_ObjectLoader->SetMethodID(CLASS_NAME_UTILITY, "GetImageSize", "(Ljava/lang/String;)[I", true);
        s_ObjectLoader->SetMethodID(CLASS_NAME_UTILITY, "GetTextImageSize", "(Ljava/lang/String;I)[F", true);
        s_ObjectLoader->SetMethodID(CLASS_NAME_UTILITY, "CreateFontImage", "(Ljava/lang/String;I)Ljava/lang/Object;", true);
    }

    DEBUG_LOG("End Load Default Object");
    DEBUG_LOG("/////////////////////////////////////////");
}


static JavaVM* s_vm = nullptr;
void SetVM(JavaVM* pVM)
{
    s_vm = pVM;
}


JNIEnv* GetEnv()
{
    JNIEnv* env = nullptr;
    s_vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    return env;
}

JNIEnv* GetThreadEnv()
{
	JNIEnv* env = nullptr;
    s_vm->AttachCurrentThread(&env, NULL);
    return env;
}

void DetachThreadEnv()
{
	s_vm->DetachCurrentThread();
}

void CheckEnvThrow()
{
    if(GetEnv()->ExceptionCheck() == JNI_TRUE){
        GetEnv()->ExceptionDescribe();
        GetEnv()->ExceptionClear();
    }
}