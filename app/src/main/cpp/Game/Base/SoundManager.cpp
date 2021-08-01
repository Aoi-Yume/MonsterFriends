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
SoundResourceLabel SoundManager::LoadSE(const char *pPath)
{
	const auto& hit = m_SoundMap.find(pPath);
	if(hit != m_SoundMap.end()){ return hit->second.label; }

	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SOUND);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SOUND, "LoadSE");
	jstring path = GetEnv()->NewStringUTF(pPath);
	SoundResourceLabel label = GetEnv()->CallStaticIntMethod(classID, methodID_1, path);
	if(label >= 0){
		m_SoundMap[pPath].bLoad = false;
		m_SoundMap[pPath].label = label;
	}
	return label;
}

//-----------------------------------------
//-----------------------------------------
void SoundManager::UnLoadSE(SoundResourceLabel label)
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SOUND);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SOUND, "UnLoadSE");
	GetEnv()->CallStaticVoidMethod(classID, methodID_1, label);

	const std::string* pPath = nullptr;
	if(getSoundPathFromLabel(label, &pPath)){
		m_SoundMap.erase(*pPath);
	}
}

//-----------------------------------------
//-----------------------------------------
bool SoundManager::IsLoadSE(SoundResourceLabel label) const
{
	SoundInfo info = {};
	const bool bRet = getSoundInfoFromLabel(label, &info);
	return bRet && info.bLoad;
}

//-----------------------------------------
//-----------------------------------------
SoundStreamHandle SoundManager::PlaySE(SoundResourceLabel label, float fLeftVol, float fRightVol, bool bLoop)
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SOUND);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SOUND, "PlaySE");
	return GetEnv()->CallStaticIntMethod(classID, methodID_1, label, fLeftVol, fRightVol, bLoop);
}

//-----------------------------------------
//-----------------------------------------
void SoundManager::StopSE(SoundStreamHandle handle)
{
	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SOUND);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SOUND, "StopSE");
	GetEnv()->CallStaticVoidMethod(classID, methodID_1, handle);
}

//-----------------------------------------
//-----------------------------------------
void SoundManager::LoadCompleteCallBack(SoundResourceLabel label, int nStatus)
{
	const std::string* pPath = nullptr;
	if(!getSoundPathFromLabel(label, &pPath)){ return; }
	if(nStatus == 0) {
		m_SoundMap[*pPath].bLoad = true;
	}
	else{
		DEBUG_LOG_A("Load Error:[%s]\n", pPath->c_str());
	}
}

//-----------------------------------------
//-----------------------------------------
bool SoundManager::getSoundInfoFromLabel(SoundResourceLabel label, SoundManager::SoundInfo* pInfo) const
{
	const SoundInfo* pHitInfo = nullptr;
	for(auto& it : m_SoundMap) {
		if(it.second.label == label){
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
bool SoundManager::getSoundPathFromLabel(SoundResourceLabel label, const std::string** pPath) const
{
	const std::string* pHitPath = nullptr;
	for(auto& it : m_SoundMap) {
		if(it.second.label == label){
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
