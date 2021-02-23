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

	const int nParamNum = 5;
	m_aSkillInfo.resize(aSplitText.size() / nParamNum);
	for(int i = 0; i < aSplitText.size(); ++i){
		const int nIdx = i / nParamNum;
		const int nParamNo = i % nParamNum;
		auto& info = m_aSkillInfo[nIdx];
		if(nParamNo == 0){ info.name = aSplitText[i]; }
		else if(nParamNo == 1){ info.Type = aSplitText[i].c_str(); }
		else if(nParamNo == 2){ info.Duration = atoi(aSplitText[i].c_str()); }
		else if(nParamNo == 3){ info.minParam = atoi(aSplitText[i].c_str()); }
		else if(nParamNo == 4){ info.maxParam = atoi(aSplitText[i].c_str()); }
	}
	for(int i = 0; i < m_aSkillInfo.size(); ++i) {
		auto& it = m_aSkillInfo[i];
		DEBUG_LOG_A("SkillNo[%d]：[%s][%s][%d][%d][%d]\n",
				i + 1, it.name.c_str(), it.Type.c_str(), it.Duration, it.minParam, it.maxParam);
	}
}

void AppSkillList::InitializeSkillTransfer()
{
	auto pManager = TransferManager::Get();
	auto p = pManager->GetTransfer<TransferSkillInfo>(TransferManager::eTramsferKind_SkillInfo);
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

		UpdateSkillInfo info ={};
		info.Type = skill.Type;
		info.nSendPlayer = data->SendPlayer;
		info.nTargetPlayer = data->TargetPlayer;
		info.Duration = data->Duration;
		info.nParam = data->Param;

		UpdateSkill(info);
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
	UpdateSkillInfo info ={};
	info.Type = skill.Type;
	info.nSendPlayer = nPlayer;
	info.nTargetPlayer = -1;
	if(pManager->IsConnectSucess()) {
		const int nNum = pManager->GetConnectNum();
		do {
			info.nTargetPlayer = Random::GetInt(0, nNum);
		} while(info.nTargetPlayer == info.nSendPlayer);
	}
	info.Duration = skill.Duration;
	info.nParam = Random::GetInt(skill.minParam, skill.maxParam);
	if(pManager->GetSelfConnect().nPlayerId == nPlayer){
		UpdateSkill(info);
	}
	sendNetSkillInfo(nSkillNo, info);
}
bool AppSkillList::IsEndItemSkill() const
{
	const auto pManager = TransferManager::Get();
	const auto p = pManager->GetTransfer<TransferSkillInfo>(TransferManager::eTramsferKind_SkillInfo);
	return p->IsEnd();
}

void AppSkillList::UpdateSkill(const UpdateSkillInfo& info)
{
	if(info.Type == "キズナ奪い") {
		AppParam::Get()->AddKizunaPoint(info.nSendPlayer, info.nParam);
		AppParam::Get()->SubKizunaPoint(info.nTargetPlayer, info.nParam);
	}
}

void AppSkillList::sendNetSkillInfo(int nSkillNo, const UpdateSkillInfo& info)
{
	auto pManager = TransferManager::Get();
	auto& netSkillInfo = AppParam::Get()->GetSkillInfo();
	netSkillInfo.Standby = true;
	netSkillInfo.SkillNo = nSkillNo;
	netSkillInfo.Duration = info.Duration;
	netSkillInfo.SendPlayer = info.nSendPlayer;
	netSkillInfo.TargetPlayer = info.nTargetPlayer;
	netSkillInfo.Param = info.nParam;

	auto p = pManager->GetTransfer<TransferSkillInfo>(TransferManager::eTramsferKind_SkillInfo);
	p->SetSkillInfoData(&netSkillInfo, sizeof(AppParam::SkillNetworkInfo), info.nSendPlayer);
	pManager->StartTransfer(TransferManager::eTramsferKind_SkillInfo);
}