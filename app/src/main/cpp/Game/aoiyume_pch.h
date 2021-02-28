//
// Created by 葵ユメ on 2018/08/20.
//


#ifndef AOIYUME_PCH_H
#define AOIYUME_PCH_H

#define EXTERN_C extern "C" {
#define END_EXTERN_C };

#define EXTERN_CXX extern "C++" {
#define END_EXTERN_CXX };

//---------- extern "c" ------------
EXTERN_C

#include <jni.h>
#include <android/log.h>
#include <memory.h>
#include <assert.h>
#include "aoiyume_define.h"

class ObjectLoaderInner;

class ObjectLoader
{
public:
	ObjectLoader();
	~ObjectLoader();

	static void Create();
	static void Destroy();
	static ObjectLoader* GetObjectLoader();

	void SetClassID(const char* className);
	jclass GetClassID(const char* className);
	void SetFieldID(const char* className, const char* fieldName, const char* sig);
	jfieldID GetFieledID(const char* className, const char* fieldName);

	void SetEnumID(const char* className, const char* enumName, const char* sig);
	jfieldID GetEnumID(const char* className, const char* enumName);
	jobject  GetEnumObj(const char* className, const char* enumName);

	void SetMethodID(const char* className, const char* methodName, const char* sig, bool isStatic = false);
	jmethodID GetMethodID(const char* className, const char* methodName);

	void SetDefaultLoad();
private:
    ObjectLoaderInner*  m_pInner;
};

void SetVM(JavaVM* pVM);
JNIEnv* GetEnv();
JNIEnv* GetThreadEnv();
void DetachThreadEnv();
void CheckEnvThrow();

END_EXTERN_C
//---------- end extern "c" ------------

//---------- extern "c++" ------------
EXTERN_CXX

#include <string>

using SYSTEM_CLOCK = std::chrono::system_clock;
using DURATION = std::chrono::milliseconds;
using TIME_POINT = std::chrono::time_point<SYSTEM_CLOCK>;

END_EXTERN_CXX
//---------- end extern "c++" ------------

#endif