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

const char* AppParam::GetCharaName(int nPlayerId) const
{
	const int nNo = TransferManager::Get()->GetConnectNoFromPlayerId(nPlayerId);
	if(nNo == -1){
		return GetCharaName();
	}
	return TransferManager::Get()->GetConnect(nNo).Name.c_str();
}

void AppParam::AddKizunaPoint(int nPlayerId, int nAdd)
{
	// 接続完了している時は通信用情報を更新
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()){
		auto& nKizuna = m_NetworkGameInfo.ChharaInfo[nPlayerId].nKizunaPoint;
		nKizuna = std::min(nKizuna + nAdd, 9999);

		// プレイヤーIDが自身のIDなら自分用も更新
		const int nNetPlayerId = pManager->GetPlayerIdFromNetId(pManager->GetSelfConnect().Id.c_str());
		if(nNetPlayerId == nPlayerId){
			m_CharaInfo.nKizunaPoint = nKizuna;
		}
	}
	// 接続できていない時はプレイヤーIDが０の時だけ更新
	else if(nPlayerId == 0){
		auto& nKizuna = m_CharaInfo.nKizunaPoint;
		nKizuna = std::min(nKizuna + nAdd, 9999);
		m_NetworkGameInfo.ChharaInfo[nPlayerId].nKizunaPoint = nKizuna;
	}
}
void AppParam::SubKizunaPoint(int nPlayerId, int nSub)
{
	// 接続完了している時は通信用情報を更新
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()){
		auto& nKizuna = m_NetworkGameInfo.ChharaInfo[nPlayerId].nKizunaPoint;
		nKizuna = std::max(nKizuna - nSub, 0);

		// プレイヤーIDが自身のIDなら自分用も更新
		const int nNetPlayerId = pManager->GetPlayerIdFromNetId(pManager->GetSelfConnect().Id.c_str());
		if(nNetPlayerId == nPlayerId){
			m_CharaInfo.nKizunaPoint = nKizuna;
		}
	}
		// 接続できていない時はプレイヤーIDが０の時だけ更新
	else if(nPlayerId == 0){
		auto& nKizuna = m_CharaInfo.nKizunaPoint;
		nKizuna = std::max(nKizuna - nSub, 0);
		m_NetworkGameInfo.ChharaInfo[nPlayerId].nKizunaPoint = nKizuna;
	}
}
int AppParam::GetKizunaPoint(int nPlayerId) const
{
	return m_NetworkGameInfo.ChharaInfo[nPlayerId].nKizunaPoint;
}

void AppParam::AddItem(int nPlayerId, int nItemNo, int nNum)
{
	assert(nItemNo >= 0 && nItemNo < eItemKind_Max);

	// 接続完了している時は通信用情報を更新
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()) {
		auto& uItemNum = m_NetworkGameInfo.ChharaInfo[nPlayerId].uItemNum[nItemNo];
		uItemNum = std::min((int)uItemNum + nNum, UINT8_MAX);

		// プレイヤーIDが自身のIDなら自分用も更新
		const int nNetPlayerId = pManager->GetPlayerIdFromNetId(pManager->GetSelfConnect().Id.c_str());
		if(nNetPlayerId == nPlayerId){
			m_CharaInfo.uItemNum[nItemNo] = uItemNum;
		}
	}
	// 接続できていない時はプレイヤーIDが０の時だけ更新
	else if(nPlayerId == 0){
		auto& uItemNum = m_CharaInfo.uItemNum[nItemNo];
		uItemNum = std::min((int)uItemNum + nNum, UINT8_MAX);
	}
}
void AppParam::SubItem(int nPlayerId, int nItemNo, int nNum)
{
	assert(nItemNo >= 0 && nItemNo < eItemKind_Max);

	// 接続完了している時は通信用情報を更新
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()) {
		auto& uItemNum = m_NetworkGameInfo.ChharaInfo[nPlayerId].uItemNum[nItemNo];
		uItemNum = std::max((int)uItemNum + nNum, 0);

		// プレイヤーIDが自身のIDなら自分用も更新
		const int nNetPlayerId = pManager->GetPlayerIdFromNetId(pManager->GetSelfConnect().Id.c_str());
		if(nNetPlayerId == nPlayerId){
			m_CharaInfo.uItemNum[nItemNo] = uItemNum;
		}
	}
	// 接続できていない時はプレイヤーIDが０の時だけ更新
	else if(nPlayerId == 0){
		auto& uItemNum = m_CharaInfo.uItemNum[nItemNo];
		uItemNum = std::max((int)uItemNum + nNum, 0);
	}
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