//
// Created by 葵ユメ on 2018/08/25.
//


#include "Engine.h"
#include <TransferManager.h>
#include <DelayInput.h>
#include <Random.h>

static Engine* s_AoYumeEngine = nullptr;

//-----------------------------------------
//-----------------------------------------
Engine::Engine()
: m_nRef(0)
, m_AssetManagerObjRef(nullptr)
, m_pAssetMaanger(nullptr)
, m_TimePoint()
, m_DeltaTime()
, m_DestroyCallBack()
{
}

//-----------------------------------------
//-----------------------------------------
Engine::~Engine()
{
	if(m_DestroyCallBack){
		m_DestroyCallBack();
	}
}

//-----------------------------------------
//-----------------------------------------
void Engine::SetDestrpyCallBack(const std::function<void ()> &callBack)
{
	m_DestroyCallBack = callBack;
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
void Engine::SetTouchInputInfo(int nEvent, float fTouchX, float fTouchY, int nPlayerId)
{
	nPlayerId = fixNetPlayerId(nPlayerId);

	TouchInputInfo info = {};
	info.nTouchEvent = nEvent;
	info.fTouchX = fTouchX;
	info.fTouchY = fTouchY;
	info.nTouchCnt = 0;

	DELAY_INPUT()->AddTouchInfo(info, nPlayerId);
}

//-----------------------------------------
//-----------------------------------------
bool Engine::FindDelayTouchInfo(TouchInputInfo& info, int nEvent, int nPlayerId) const
{
	nPlayerId = fixNetPlayerId(nPlayerId);
	return DELAY_INPUT()->FindDelayTouchInfo(info, nEvent, nPlayerId);
}

//-----------------------------------------
//-----------------------------------------
void Engine::SetTimePoint(const TIME_POINT & timePoint)
{
	auto sub = timePoint - m_TimePoint;
	m_DeltaTime = std::chrono::duration_cast<DURATION>(sub);
	m_TimePoint = timePoint;
}

//-----------------------------------------
//-----------------------------------------
float Engine::GetDeltaTime() const
{
	return m_DeltaTime.count() / 1000.0f;
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
bool Engine::IsShowSoftwareKeyboard() const
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
void Engine::DisconnectNearbyAllEndPoint()
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "DisconnectNearbyAllEndPoint");
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
void Engine::Create()
{
	if( !s_AoYumeEngine ){
		s_AoYumeEngine = new Engine();
	}
	Random::Initialize();
	DELAY_INPUT()->Initialize(0.1f);
	DELAY_INPUT()->StartDelayInput();
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
Engine* Engine::GetEngine()
{
	return s_AoYumeEngine;
}

//-----------------------------------------
//-----------------------------------------
int Engine::fixNetPlayerId(int nPlayerId) const
{
	if(nPlayerId == -1){
		auto pManager = TransferManager::Get();
		nPlayerId = 0;
		if(pManager->IsConnectSucess()){
			if(pManager->GetSelfConnect().nPlayerId >= 0){
				nPlayerId = pManager->GetSelfConnect().nPlayerId;
			}
		}
	}
	return nPlayerId;
}