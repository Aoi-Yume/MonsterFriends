//
// Created by 葵ユメ on 2021/02/23.
//


#include "TransferSkillInfo.h"
#include "TransferManager.h"

TransferSkillInfo::TransferSkillInfo()
: TransferBase(TransferManager::eTransferKind_SkillInfo, 50 * 1000 * 1000)
, m_bReceive()
, m_Data()
, m_nSize(0)
, m_nSendPlayer(-1)
, m_pReceiveCallBack()
, m_pDumpCallBack()
{
}

TransferSkillInfo::~TransferSkillInfo()
{
}

void TransferSkillInfo::initialize()
{
	DEBUG_LOG_A("【Net】%s\n", __PRETTY_FUNCTION__ );
	m_Data.uKind = TransferManager::eTransferKind_SkillInfo;
	m_Data.bReqEnd = false;
	for(int i = 0; i < NET_CONNECT_MAX; ++i){
		m_bReceive[i] = false;
	}
}

bool TransferSkillInfo::updateTransfer()
{
	const auto pManager = TransferManager::Get();
	if(pManager->GetSelfConnect().nPlayerId == m_nSendPlayer) {
		jbyte data[512];
		ASSERT(m_nSize + 2 < sizeof(data));
		data[0] = m_Data.uKind;
		data[1] = m_Data.bReqEnd;
		memcpy(&data[2], m_Data.pData, m_nSize);
		pManager->BroadCast((jbyte *) data, m_nSize + 2);
		if(m_Data.bReqEnd){
			RequestEnd();
		}
		return true;
	}
	else if(getReceiveCnt() >= 1 && getSendCnt() == 0){
		pManager->SendPlayer(m_nSendPlayer, (jbyte*)&m_Data, sizeof(m_Data));
		return true;
	}
	return false;
}
void TransferSkillInfo::updateReceive(const char* Id, void* pData)
{
	if(IsEnd()){ return; }
	TransferBase::updateReceive(Id, pData);

	Data* pReceiveData = (Data*)pData;
	const int nConnectNum = TransferManager::Get()->GetConnectNum();

	auto pManager = TransferManager::Get();
	if(pManager->GetSelfConnect().nPlayerId == m_nSendPlayer){
		// 送り開始側での接続Noに対応させて全員分の返答を待つ
		const int nNo = pManager->GetConnectNoFromNetId(Id);
		if(nNo >= 0){
			m_bReceive[nNo] = true;
		}
		int nReceiveCnt = 0;
		for(int i = 0; i < NET_CONNECT_MAX; ++i){
			if(m_bReceive[i]){ nReceiveCnt++; }
		}
		// 全員分の返答が来た
		if(nReceiveCnt >= nConnectNum){
			m_Data.bReqEnd = true;
		}
	}
	else{
		if (m_pReceiveCallBack) {
			m_pReceiveCallBack(&((jbyte *) pData)[2]);
		}
		if(pReceiveData->bReqEnd){
			RequestEnd();
		}
	}
}

void TransferSkillInfo::SetSkillInfoData(void* pData, int nSize, int nSendPlayer)
{
	m_Data.pData = pData;
	m_nSize = nSize;
	m_nSendPlayer = nSendPlayer;
}

void TransferSkillInfo::SetReceiveCallBack(std::function<void(void*)> pCallBack)
{
	m_pReceiveCallBack = pCallBack;
}
void TransferSkillInfo::SetDumpCallBack(std::function<void()> pCallBack)
{
	m_pDumpCallBack = pCallBack;
}

void TransferSkillInfo::Dump()
{
	if(m_pDumpCallBack){
		m_pDumpCallBack();
	}
}