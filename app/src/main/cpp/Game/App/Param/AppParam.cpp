//
// Created by 葵ユメ on 2020/12/06.
//


#include "AppParam.h"
template<> AppParam* Singleton<AppParam>::s_pSingleton = nullptr;

AppParam::AppParam()
: Singleton<AppParam>()
, m_CharaInfo()
{
	DEBUG_LOG("Create AppParam");
}

AppParam::~AppParam()
{
	DEBUG_LOG("Destroy AppParam");
}

void AppParam::SetCharaName(const char *pName)
{
	std::snprintf(m_CharaInfo.charaName, sizeof(m_CharaInfo.charaName), "%s", pName);
	m_CharaInfo.charaName[sizeof(m_CharaInfo.charaName) - 1] = '\0';
}

const char* AppParam::GetCharaName() const
{
	return m_CharaInfo.charaName;
}

void AppParam::AddKizunaPoint(int nAdd)
{
	m_CharaInfo.nKizunaPoint = std::min(m_CharaInfo.nKizunaPoint + nAdd, 9999);
};
int AppParam::GetKizunaPoint() const
{
	return m_CharaInfo.nKizunaPoint;
}