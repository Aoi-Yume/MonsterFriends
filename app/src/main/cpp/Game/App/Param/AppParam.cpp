//
// Created by 葵ユメ on 2020/12/06.
//


#include "AppParam.h"
#include <../Net/TransferManager.h>
USE_SINGLETON_VARIABLE(AppParam);

AppParam::AppParam()
: Singleton<AppParam>()
, m_CharaInfo()
, m_NetworkGameInfo()
{
	DEBUG_LOG("Create AppParam");
}

AppParam::~AppParam()
{
	DEBUG_LOG("Destroy AppParam");
}

void AppParam::SetCharaName(const char *pName)
{
	std::snprintf(m_CharaInfo.cCharaName, sizeof(m_CharaInfo.cCharaName), "%s", pName);
	m_CharaInfo.cCharaName[sizeof(m_CharaInfo.cCharaName) - 1] = '\0';
}

const char* AppParam::GetCharaName() const
{
	return m_CharaInfo.cCharaName;
}

void AppParam::AddKizunaPoint(int nAdd)
{
	m_CharaInfo.nKizunaPoint = std::min(m_CharaInfo.nKizunaPoint + nAdd, 9999);

	// 通信用情報も更新
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()){
		const int nPlayerId = pManager->GetPlayerIdFromNetId(pManager->GetSelfConnect().Id.c_str());
		if(nPlayerId >= 0){
			m_NetworkGameInfo.ChharaInfo[nPlayerId].nKizunaPoint = m_CharaInfo.nKizunaPoint;
		}
	}
};
int AppParam::GetKizunaPoint() const
{
	return m_CharaInfo.nKizunaPoint;
}

AppParam::GameNetworkInfo& AppParam::GetNetworkInfo()
{
	return m_NetworkGameInfo;
}

void AppParam::DumpNetworkInfo()
{
	DEBUG_LOG_A("CurrentPlayerId[%d]\n", m_NetworkGameInfo.nCurrentPlayerId);
	DEBUG_LOG_A("CurrentTurn[%d]\n", m_NetworkGameInfo.nCurrentTurn);
	for(int i = 0; i < NET_CONNECT_MAX; ++i){
		DEBUG_LOG_A("Player[%d]:Kizuna[%d]\n", i + 1, m_NetworkGameInfo.ChharaInfo[i].nKizunaPoint);
	}
}