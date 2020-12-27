//
// Created by 葵ユメ on 2018/08/25.
//


#include "Engine.h"

static Engine* s_AoYumeEngine = nullptr;

//-----------------------------------------
//-----------------------------------------
Engine::Engine()
: m_nRef(0)
, m_touchInputInfo()
, m_AssetManagerObjRef(nullptr)
, m_pAssetMaanger(nullptr)
{
}

//-----------------------------------------
//-----------------------------------------
Engine::~Engine()
{
}

//-----------------------------------------
//-----------------------------------------
void Engine::Create()
{
	if( !s_AoYumeEngine ){
		s_AoYumeEngine = new Engine();
	}
	s_AoYumeEngine->AddRef();
}

//-----------------------------------------
//-----------------------------------------
void Engine::Destroy()
{
	s_AoYumeEngine->SubRef();
	if(s_AoYumeEngine->IsEmptyRef()){
		delete s_AoYumeEngine;
		s_AoYumeEngine = nullptr;
	}
}

//-----------------------------------------
//-----------------------------------------
void Engine::AddRef()
{
	m_nRef++;
}

//-----------------------------------------
//-----------------------------------------
void Engine::SubRef()
{
	m_nRef--;
}

//-----------------------------------------
//-----------------------------------------
bool Engine::IsEmptyRef() const
{
	return m_nRef == 0;
}

//-----------------------------------------
//-----------------------------------------
void Engine::SetScreenInfo(int nWidth, int nHeight, float fAspect)
{
	m_screenInfo.m_nScreenX = nWidth;
	m_screenInfo.m_nScreenY = nHeight;
	m_screenInfo.m_fAspect = fAspect;
}

//-----------------------------------------
//-----------------------------------------
const ScreenInfo& Engine::GetScreenInfo() const
{
	return m_screenInfo;
}

//-----------------------------------------
//-----------------------------------------
void Engine::SetTouchInputInfo(int nEvent, float fTouchX, float fTouchY)
{
	m_touchInputInfo.m_nPreTouchEvent = m_touchInputInfo.m_nTouchEvent;
	m_touchInputInfo.m_nTouchEvent = nEvent;
	m_touchInputInfo.m_fTouchX = fTouchX;
	m_touchInputInfo.m_fTouchY = fTouchY;
	m_touchInputInfo.m_nTouchCnt = 0;
}

//-----------------------------------------
//-----------------------------------------
const TouchInputInfo& Engine::GetTouchInputInfo() const
{
	return m_touchInputInfo;
}

//-----------------------------------------
//-----------------------------------------
void Engine::CheckTouchUpdate()
{
	m_touchInputInfo.m_nTouchCnt++;
	if(m_touchInputInfo.m_nTouchCnt >= 5){
		// TouchEventがない状態で一定フレーム経過した
		m_touchInputInfo.m_nTouchEvent = -1;
	}
}

//-----------------------------------------
//-----------------------------------------
void Engine::ResetTouchEvent()
{
	m_touchInputInfo.m_nPreTouchEvent = m_touchInputInfo.m_nTouchEvent;
	m_touchInputInfo.m_nTouchEvent = -1;
	m_touchInputInfo.m_nTouchCnt = 0;
}

//-----------------------------------------
//-----------------------------------------
void Engine::SetCameraComponent(void* pComponent)
{
	m_pCameraComponent = pComponent;
}

//-----------------------------------------
//-----------------------------------------
void* Engine::GetCameraComponent() const{
	return m_pCameraComponent;
}

//-----------------------------------------
//-----------------------------------------
void Engine::SetAssetManager(jobject assetMaangerObj) {
	m_AssetManagerObjRef = GetEnv()->NewGlobalRef(assetMaangerObj);
	m_pAssetMaanger = AAssetManager_fromJava(GetEnv(), m_AssetManagerObjRef);
}

//-----------------------------------------
//-----------------------------------------
void Engine::DeleteAssetManager() {
	GetEnv()->DeleteGlobalRef(m_AssetManagerObjRef);
	m_AssetManagerObjRef = nullptr;
}

//-----------------------------------------
//-----------------------------------------
void Engine::LoadAsset(const char *pFileName, void **pBuffer, long *pSize, bool bAllocate) {
	AAsset* pAsset = AAssetManager_open(m_pAssetMaanger, pFileName, AASSET_MODE_BUFFER);

	DEBUG_LOG_A("////// Begin LoadAsset[%s] //////", pFileName);
	if(!pAsset) {
		DEBUG_LOG_A("Not Found Asset[%s]", pFileName);
		assert(pAsset);
	}

	const long nSize = AAsset_getLength(pAsset);
	if(pSize){ *pSize = nSize; }

	if(pBuffer) {
		if (!*pBuffer && bAllocate) {
			*pBuffer = new char[nSize];
			DEBUG_LOG_A("Allocate Data[%ld]", nSize);
		}
		if (*pBuffer) {
			int nReadSize = AAsset_read(pAsset, *pBuffer, (size_t) nSize);
			DEBUG_LOG_A("Read[%d]", nReadSize);
		}
	}
	AAsset_close(pAsset);

	DEBUG_LOG_A("////// Complete LoadAsset[%s] //////", pFileName);
}

//-----------------------------------------
//-----------------------------------------
void Engine::ShowSoftwareKeyboard()
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "ShowSoftwareKeyboard");
	GetEnv()->CallStaticVoidMethod(classID, methodID_1);
}

//-----------------------------------------
//-----------------------------------------
bool Engine::IsShowSofrwareKeyboard() const
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "IsShowSoftwareKeyboard");
	return GetEnv()->CallStaticBooleanMethod(classID, methodID_1);
}

//-----------------------------------------
//-----------------------------------------
void Engine::GetInputText(char* pText, int nSize) const
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "GetInputText");
	jstring obj = (jstring)GetEnv()->CallStaticObjectMethod(classID, methodID_1);
	const char* pInputText = GetEnv()->GetStringUTFChars(obj, nullptr);
	std::snprintf(pText, nSize, "%s", pInputText);
	pText[nSize - 1] = '\0';
	GetEnv()->ReleaseStringUTFChars(obj, pInputText);
}

//-----------------------------------------
//-----------------------------------------
void Engine::StartNearbyAdvertising(const char* pConnectName)
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "StartNearbyAdvertising");
	jstring connectName = GetEnv()->NewStringUTF(pConnectName);
	GetEnv()->CallStaticVoidMethod(classID, methodID_1, connectName);
}

//-----------------------------------------
//-----------------------------------------
void Engine::StopNearbyAdvertising()
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "StopNearbyAdvertising");
	GetEnv()->CallStaticVoidMethod(classID, methodID_1);
}

//-----------------------------------------
//-----------------------------------------
void Engine::StartNearbyDiscovery(const char* pConnectName)
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "StartNearbyDiscovery");
	jstring connectName = GetEnv()->NewStringUTF(pConnectName);
	GetEnv()->CallStaticVoidMethod(classID, methodID_1, connectName);
}

//-----------------------------------------
//-----------------------------------------
void Engine::StopNearbyDiscovery()
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "StopNearbyDiscovery");
	GetEnv()->CallStaticVoidMethod(classID, methodID_1);
}

//-----------------------------------------
//-----------------------------------------
void Engine::SendData(const char* pId, jbyte* pData, int nSize)
{
	jbyteArray byteData = GetEnv()->NewByteArray(nSize);
	GetEnv()->SetByteArrayRegion(byteData, 0, nSize, pData);

	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "SendData");
	jstring Id = GetEnv()->NewStringUTF(pId);
	GetEnv()->CallStaticVoidMethod(classID, methodID_1, Id, byteData);
}

//-----------------------------------------
//-----------------------------------------
Engine* Engine::GetEngine()
{
	return s_AoYumeEngine;
}