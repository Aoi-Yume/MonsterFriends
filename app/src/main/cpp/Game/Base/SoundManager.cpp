//
// Created by 葵ユメ on 2021/07/25.
//

#include "SoundManager.h"
#include "../../aoiyume_pch.h"


//-----------------------------------------
//-----------------------------------------
SoundManager::SoundManager()
: m_SoundMap()
{
}

//-----------------------------------------
//-----------------------------------------
SoundManager::~SoundManager()
{
}

//-----------------------------------------
//-----------------------------------------
void SoundManager::Initialize()
{
	DEBUG_LOG("Initialize SoundManager\n");
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SOUND);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SOUND, "Initialize");
	GetEnv()->CallStaticVoidMethod(classID, methodID_1);
	m_SoundMap.clear();
	m_SoundMap.reserve(64);
}

//-----------------------------------------
//-----------------------------------------
void SoundManager::Finalize()
{
	DEBUG_LOG("Finalize SoundManager\n");
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SOUND);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SOUND, "Finalize");
	GetEnv()->CallStaticVoidMethod(classID, methodID_1);
}

//-----------------------------------------
//-----------------------------------------
SoundHandle SoundManager::LoadSE(const char *pPath)
{
	const auto& hit = m_SoundMap.find(pPath);
	if(hit != m_SoundMap.end()){ return hit->second.Handle; }

	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SOUND);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SOUND, "LoadSE");
	jstring path = GetEnv()->NewStringUTF(pPath);
	SoundHandle handle = GetEnv()->CallStaticIntMethod(classID, methodID_1, path);
	if(handle >= 0){
		m_SoundMap[pPath].bLoad = false;
		m_SoundMap[pPath].Handle = handle;
	}
	return handle;
}

//-----------------------------------------
//-----------------------------------------
void SoundManager::UnLoadSE(SoundHandle handle)
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SOUND);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SOUND, "UnLoadSE");
	GetEnv()->CallStaticVoidMethod(classID, methodID_1, handle);

	const std::string* pPath = nullptr;
	if(getSoundPathFromHandle(handle, &pPath)){
		m_SoundMap.erase(*pPath);
	}
}

//-----------------------------------------
//-----------------------------------------
bool SoundManager::IsLoadSE(SoundHandle handle) const
{
	SoundInfo info = {};
	const bool bRet = getSoundInfoFromHandle(handle, &info);
	return bRet && info.bLoad;
}

//-----------------------------------------
//-----------------------------------------
void SoundManager::PlaySE(SoundHandle handle, float fLeftVol, float fRightVol, bool bLoop)
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SOUND);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SOUND, "PlaySE");
	GetEnv()->CallStaticVoidMethod(classID, methodID_1, handle, fLeftVol, fRightVol, bLoop);
}

//-----------------------------------------
//-----------------------------------------
void SoundManager::StopSE(SoundHandle handle)
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SOUND);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SOUND, "StopSE");
	GetEnv()->CallStaticVoidMethod(classID, methodID_1, handle);
}

//-----------------------------------------
//-----------------------------------------
void SoundManager::LoadCompleteCallBack(SoundHandle handle, int nStatus)
{
	const std::string* pPath = nullptr;
	if(!getSoundPathFromHandle(handle, &pPath)){ return; }
	if(nStatus == 0) {
		m_SoundMap[*pPath].bLoad = true;
	}
	else{
		DEBUG_LOG_A("Load Error:[%s]\n", pPath->c_str());
	}
}

//-----------------------------------------
//-----------------------------------------
bool SoundManager::getSoundInfoFromHandle(SoundHandle handle, SoundManager::SoundInfo* pInfo) const
{
	const SoundInfo* pHitInfo = nullptr;
	for(auto& it : m_SoundMap) {
		if(it.second.Handle == handle){
			pHitInfo = &it.second;
			break;
		}
	}
	if(!pHitInfo){ return false; }
	if(pInfo){
		*pInfo = *pHitInfo;
	}

	return true;
}

//-----------------------------------------
//-----------------------------------------
bool SoundManager::getSoundPathFromHandle(SoundHandle handle, const std::string** pPath) const
{
	const std::string* pHitPath = nullptr;
	for(auto& it : m_SoundMap) {
		if(it.second.Handle == handle){
			pHitPath = &it.first;
			break;
		}
	}
	if(!pHitPath){ return false; }
	if(pPath){
		*pPath = pHitPath;
	}
	return true;
}
