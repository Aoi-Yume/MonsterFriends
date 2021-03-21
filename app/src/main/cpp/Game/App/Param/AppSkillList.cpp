//
// Created by 葵ユメ on 2021/02/23.
//


#include <Util.h>
#include "AppSkillList.h"
#include <AppParam.h>
#include <Random.h>
#include <TransferManager.h>
#include <TransferSkillInfo.h>
USE_SINGLETON_VARIABLE(AppSkillList);

AppSkillList::AppSkillList()
: Singleton<AppSkillList>()
, m_aSkillInfo()
{
	DEBUG_LOG("Create AppSkillList");
}

AppSkillList::~AppSkillList()
{
	DEBUG_LOG("Destroy AppSkillList");
}

void AppSkillList::Load()
{
	void* pBuffer = nullptr;
	long lSize = 0L;
	Engine::GetEngine()->LoadAsset("param/skillList.csv", &pBuffer, &lSize, true);
	std::string str = (const char*)pBuffer;
	std::vector<std::string>	aSplitText;
	Util::SplitCSVText(aSplitText, str);

	const int nParamNum = 6;
	assert(aSplitText.size() / nParamNum < eSkillList_Max);
	for(int i = 0; i < aSplitText.size(); ++i){
		const int nIdx = i / nParamNum;
		const int nParamNo = i % nParamNum;
		auto& info = m_aSkillInfo[nIdx];
		if(nParamNo == 0){ info.name = aSplitText[i]; }
		else if(nParamNo == 1){ info.Type = aSplitText[i]; }
		else if(nParamNo == 2){ info.Timing = aSplitText[i]; }
		else if(nParamNo == 3){ info.Duration = atoi(aSplitText[i].c_str()); }
		else if(nParamNo == 4){ info.minParam = atoi(aSplitText[i].c_str()); }
		else if(nParamNo == 5){ info.maxParam = atoi(aSplitText[i].c_str()); }
	}
	for(int i = 0; i < m_aSkillInfo.size(); ++i) {
		auto& it = m_aSkillInfo[i];
		DEBUG_LOG_A("SkillNo[%d]：[%s][%s][%s][%d][%d][%d]\n",
				i + 1, it.name.c_str(), it.Type.c_str(), it.Timing.c_str(), it.Duration, it.minParam, it.maxParam);
	}
}

void AppSkillList::InitializeSkillTransfer()
{
	auto pManager = TransferManager::Get();
	if(!pManager->IsConnectSucess()){ return; }

	auto p = pManager->GetTransfer<TransferSkillInfo>(TransferManager::eTransferKind_SkillInfo);
	p->SetReceiveCallBack([this](void* pData){
		const auto data = (AppParam::SkillNetworkInfo*)(pData);
		const auto skill = m_aSkillInfo[data->SkillNo];

		auto& netSkillInfo = AppParam::Get()->GetSkillInfo();
		// 通信準備が整う前に来たら無効化
		if(!netSkillInfo.Standby){ return; }
		netSkillInfo.Standby = false;
		netSkillInfo.SkillNo = data->SkillNo;
		netSkillInfo.Duration = data->Duration;
		netSkillInfo.SendPlayer = data->SendPlayer;
		netSkillInfo.TargetPlayer = data->TargetPlayer;
		netSkillInfo.Param = data->Param;

		AppParam::Get()->SetUseSkillInfo(data->SkillNo, skill.Duration, data->Param, data->SendPlayer, data->TargetPlayer);
	});
	p->SetDumpCallBack([](){ AppParam::Get()->DumpNetworkSkillInfo(); });
}

const AppSkillList::SkillInfo& AppSkillList::GetSkillInfo(int nIdx) const
{
	return m_aSkillInfo[nIdx];
}

int AppSkillList::GetSkillNoFromSkillName(const char* pSkillName)
{
	const int nSize = m_aSkillInfo.size();
	for(int i = 0; i < nSize; ++i){
		if(std::strcmp(m_aSkillInfo[i].name.c_str(), pSkillName) == 0){
			return i;
		}
	}
	return -1;
}

void AppSkillList::BeginItemSkill(int nPlayer, int nSkillNo)
{
	const auto skill = m_aSkillInfo[nSkillNo];
	auto pManager = TransferManager::Get();

	int nTargetPlayer = -1;
	const int nParam = getSkillParam(nSkillNo, nPlayer);
	if(pManager->IsConnectSucess()) {
		const int nNum = pManager->GetConnectNum();
		do {
			nTargetPlayer = Random::GetInt(0, nNum);
		} while(nTargetPlayer == nPlayer);

		if(pManager->GetSelfConnect().nPlayerId == nPlayer){
			AppParam::Get()->SetUseSkillInfo(nSkillNo, skill.Duration, nParam, nPlayer, nTargetPlayer);
		}
		sendNetSkillInfo(nSkillNo, skill.Duration, nParam, nPlayer, nTargetPlayer);
	}
	else{
		AppParam::Get()->SetUseSkillInfo(nSkillNo, skill.Duration, nParam, nPlayer, nTargetPlayer);
	}
}
bool AppSkillList::IsEndItemSkill() const
{
	const auto pManager = TransferManager::Get();
	if(!pManager->IsConnectSucess()){ return true; }

	const auto p = pManager->GetTransfer<TransferSkillInfo>(TransferManager::eTransferKind_SkillInfo);
	return p->IsEnd();
}

void AppSkillList::UpdateSkill(int nPlayer, SkillTiming timing)
{
	auto pUseSkill = AppParam::Get()->GetUseSkillInfo(nPlayer);
	for(int i = 0; i < eSkillList_Max; ++i) {
		const auto& skill = m_aSkillInfo[i];
		if(!pUseSkill[i].bEnable){ continue; }
		if(getSkillTiming(skill.Timing.c_str()) != timing){ continue; }

		DEBUG_LOG_A("スキル更新[%s]\n", skill.Type.c_str());
		if (skill.Type == "キズナプラス") {
			AppParam::Get()->AddKizunaPoint(pUseSkill[i].SendPlayer, pUseSkill[i].Param);
		} else if (skill.Type == "キズナ奪い") {
			AppParam::Get()->AddKizunaPoint(pUseSkill[i].SendPlayer, pUseSkill[i].Param);
			AppParam::Get()->SubKizunaPoint(pUseSkill[i].TargetPlayer, pUseSkill[i].Param);
		}
		AppParam::Get()->UpdateSkillDuration(i, nPlayer);
	}
}

void AppSkillList::sendNetSkillInfo(int nSkillNo, int nDuration, int nParam, int nSendPlayer, int nTargetPlayer)
{
	auto pManager = TransferManager::Get();
	if(!pManager->IsConnectSucess()){ return; }

	auto& netSkillInfo = AppParam::Get()->GetSkillInfo();
	netSkillInfo.Standby = true;
	netSkillInfo.SkillNo = nSkillNo;
	netSkillInfo.Duration = nDuration;
	netSkillInfo.SendPlayer = nSendPlayer;
	netSkillInfo.TargetPlayer = nTargetPlayer;
	netSkillInfo.Param = nParam;

	auto p = pManager->GetTransfer<TransferSkillInfo>(TransferManager::eTransferKind_SkillInfo);
	p->SetSkillInfoData(&netSkillInfo, sizeof(AppParam::SkillNetworkInfo), nSendPlayer);
	pManager->StartTransfer(TransferManager::eTransferKind_SkillInfo);
}

int AppSkillList::getSkillParam(int nSkillNo, int nPlayer) const
{
	const auto skill = m_aSkillInfo[nSkillNo];
	int nParam = Random::GetInt(skill.minParam, skill.maxParam);

	// 特定のスキルは効果が重複する
	if(skill.Type == "キズナプラス") {
		nParam += AppParam::Get()->GetUseSkillInfo(nPlayer)[nSkillNo].Param;
	}
	return nParam;
}

int AppSkillList::getSkillTiming(const char* str) const
{
	if(std::strcmp(str, "なし") == 0) {
		return eSKillTiming_None;
	}
	else if(std::strcmp(str, "直後") == 0){
		return eSkillTiming_Now;
	}
	else if(std::strcmp(str, "自分のターン開始") == 0) {
		return eSkillTiming_SelfTurn;
	}
	DEBUG_LOG("スキルタイミング不正\n");
	abort();
	return eSKillTiming_None;
}