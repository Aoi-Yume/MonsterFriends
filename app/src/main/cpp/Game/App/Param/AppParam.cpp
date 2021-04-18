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
, m_NetworkSkillInfo()
{
	DEBUG_LOG("Create AppParam");
	m_CharaInfo.nKizunaPoint = 100;
	for(int i = 0; i < NET_CONNECT_MAX; ++i){
		m_NetworkGameInfo.CharaInfo[i].nKizunaPoint = 100;
	}
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
		auto& nKizuna = m_NetworkGameInfo.CharaInfo[nPlayerId].nKizunaPoint;
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
		m_NetworkGameInfo.CharaInfo[nPlayerId].nKizunaPoint = nKizuna;
	}
}
void AppParam::SubKizunaPoint(int nPlayerId, int nSub)
{
	// 接続完了している時は通信用情報を更新
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()){
		auto& nKizuna = m_NetworkGameInfo.CharaInfo[nPlayerId].nKizunaPoint;
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
		m_NetworkGameInfo.CharaInfo[nPlayerId].nKizunaPoint = nKizuna;
	}
}
int AppParam::GetKizunaPoint(int nPlayerId) const
{
	return m_NetworkGameInfo.CharaInfo[nPlayerId].nKizunaPoint;
}

void AppParam::AddItem(int nPlayerId, int nItemNo, int nNum)
{
	ASSERT(nItemNo >= 0 && nItemNo < eItemKind_Max);

	// 接続完了している時は通信用情報を更新
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()) {
		auto& uItemNum = m_NetworkGameInfo.CharaInfo[nPlayerId].uItemNum[nItemNo];
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
	ASSERT(nItemNo >= 0 && nItemNo < eItemKind_Max);

	// 接続完了している時は通信用情報を更新
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()) {
		auto& uItemNum = m_NetworkGameInfo.CharaInfo[nPlayerId].uItemNum[nItemNo];
		uItemNum = std::max((int)uItemNum - nNum, 0);

		// プレイヤーIDが自身のIDなら自分用も更新
		const int nNetPlayerId = pManager->GetPlayerIdFromNetId(pManager->GetSelfConnect().Id.c_str());
		if(nNetPlayerId == nPlayerId){
			m_CharaInfo.uItemNum[nItemNo] = uItemNum;
		}
	}
	// 接続できていない時はプレイヤーIDが０の時だけ更新
	else if(nPlayerId == 0){
		auto& uItemNum = m_CharaInfo.uItemNum[nItemNo];
		uItemNum = std::max((int)uItemNum - nNum, 0);
	}
}

int AppParam::GetItemNum(int nPlayerId, int nItemNo) const
{
	ASSERT(nItemNo >= 0 && nItemNo < eItemKind_Max);

	// 接続完了している時は通信用情報から取得
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()) {
		return m_NetworkGameInfo.CharaInfo[nPlayerId].uItemNum[nItemNo];
	}
	// 接続できていない時は自身の情報から取得
	return m_CharaInfo.uItemNum[nItemNo];
}

void AppParam::SetClear(bool bClear) {
	m_NetworkGameInfo.bClear = true;
}

bool AppParam::IsClear() const {
	return m_NetworkGameInfo.bClear;
}

void AppParam::SetUseSkillInfo(int nNo, int nDuration, int nParam, int nSendPlayer, int nTargetPlayer)
{
	ASSERT(nNo >= 0 && nNo < AppSkillList::eSkillList_Max);

	// 接続完了している時は通信用情報を更新
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()) {
		auto &skillInfo = m_NetworkGameInfo.CharaInfo[nSendPlayer].useSkill[nNo];
		skillInfo.bEnable = true;
		skillInfo.Duration = nDuration;
		skillInfo.Param = nParam;
		skillInfo.SendPlayer = nSendPlayer;
		skillInfo.TargetPlayer = nTargetPlayer;

		// プレイヤーIDが自身のIDなら自分用も更新
		const int nNetPlayerId = pManager->GetPlayerIdFromNetId(pManager->GetSelfConnect().Id.c_str());
		if(nNetPlayerId == nSendPlayer){
			m_CharaInfo.useSkill[nNo] = skillInfo;
		}
	}
	// 接続できていない時はプレイヤーIDが０の時だけ更新
	else if(nSendPlayer == 0){
		auto &skillInfo = m_CharaInfo.useSkill[nNo];
		skillInfo.bEnable = true;
		skillInfo.Duration = nDuration;
		skillInfo.Param = nParam;
		skillInfo.SendPlayer = nSendPlayer;
		skillInfo.TargetPlayer = nTargetPlayer;
	}
}

const AppParam::UseSkillInfo* AppParam::GetUseSkillInfo(int nPlayerId) const
{
	// 接続完了している時は通信用情報から取得
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()) {
		return m_NetworkGameInfo.CharaInfo[nPlayerId].useSkill;
	}
	// 接続できていない時は自身の情報から取得
	return m_CharaInfo.useSkill;
}

void AppParam::UpdateSkillDuration(int nNo, int nPlayerId)
{
	// 接続完了している時は通信用情報を更新
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()) {
		auto& skillInfo = m_NetworkGameInfo.CharaInfo[nPlayerId].useSkill[nNo];
		if(skillInfo.Duration < 0){ return; }	// 無限継続
		skillInfo.Duration--;
		if(skillInfo.Duration == 0){ skillInfo.bEnable = false; }

		// プレイヤーIDが自身のIDなら自分用も更新
		const int nNetPlayerId = pManager->GetPlayerIdFromNetId(pManager->GetSelfConnect().Id.c_str());
		if(nNetPlayerId == nPlayerId){
			m_CharaInfo.useSkill[nNo] = skillInfo;
		}
	}
	// 接続できていない時はプレイヤーIDが０の時だけ更新
	else if(nPlayerId == 0) {
		auto& skillInfo = m_CharaInfo.useSkill[nNo];
		if (skillInfo.Duration < 0) { return; }    // 無限継続
		skillInfo.Duration--;
		if (skillInfo.Duration == 0) { skillInfo.bEnable = false; }
	}
}

void AppParam::DumpUseSkillInfo(int nPlayerId)
{
	auto pUseSkill = GetUseSkillInfo(nPlayerId);
	for(int i = 0; i < AppSkillList::eSkillList_Max; ++i){
		auto& skill = AppSkillList::Get()->GetSkillInfo(i);
		DEBUG_LOG_A("[%d][%s]:[%d][%d][%d][%d][%d]\n",
				i + 1, skill.name.c_str(), pUseSkill[i].bEnable, pUseSkill[i].Duration, pUseSkill[i].Param, pUseSkill[i].SendPlayer, pUseSkill[i].TargetPlayer);
	}
}

int AppParam::GetDiceNum(int nPlayer) const
{
	const int nSkillNo = AppSkillList::Get()->GetSkillNoFromSkillName("サイコロプラス");
	auto pUseSkill = GetUseSkillInfo(nPlayer);
	return pUseSkill[nSkillNo].Param + 1;
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
		DEBUG_LOG_A("Player[%d]:Kizuna[%d]\n", i + 1, m_NetworkGameInfo.CharaInfo[i].nKizunaPoint);
	}
}

AppParam::SkillNetworkInfo& AppParam::GetSkillInfo()
{
	return m_NetworkSkillInfo;
}

void AppParam::DumpNetworkSkillInfo()
{
	DEBUG_LOG_A("SkillNo[%d]\n", m_NetworkSkillInfo.SkillNo);
	DEBUG_LOG_A("SendPlayerId[%d]\n", m_NetworkSkillInfo.SendPlayer);
	DEBUG_LOG_A("TargetPlayer[%d]\n", m_NetworkSkillInfo.TargetPlayer);
	DEBUG_LOG_A("Duration[%d]\n", m_NetworkSkillInfo.Duration);
	DEBUG_LOG_A("Param[%d]\n", m_NetworkSkillInfo.Param);
}

