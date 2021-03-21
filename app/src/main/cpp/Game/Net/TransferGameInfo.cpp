//
// Created by 葵ユメ on 2020/12/31.
//


#include "TransferGameInfo.h"
#include "TransferManager.h"

TransferGameInfo::TransferGameInfo()
: TransferBase(TransferManager::eTransferKind_GameInfo, 50 * 1000 * 1000)
, m_bReceive()
, m_Data()
, m_nSize(0)
, m_pReceiveCallBack()
, m_pDumpCallBack()
{
}

TransferGameInfo::~TransferGameInfo()
{
}

void TransferGameInfo::initialize()
{
	m_Data.uKind = TransferManager::eTransferKind_GameInfo;
	m_Data.bReqEnd = false;
	for(int i = 0; i < NET_CONNECT_MAX; ++i){
		m_bReceive[i] = false;
	}
}

bool TransferGameInfo::updateTransfer()
{
	const auto pManager = TransferManager::Get();
	if(pManager->IsHost()) {
		jbyte data[1024];
		assert(m_nSize + 2 < sizeof(data));
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
		pManager->SendHost((jbyte*)&m_Data, sizeof(m_Data));
		DEBUG_LOG("SendReceive\n");
		return true;
	}
	return false;
}
void TransferGameInfo::updateReceive(const char* Id, void* pData)
{
	if(IsEnd()){ return; }
	TransferBase::updateReceive(Id, pData);

	Data* pReceiveData = (Data*)pData;
	const int nConnectNum = TransferManager::Get()->GetConnectNum();

	auto pManager = TransferManager::Get();
	if(pManager->IsHost()){
		// ホスト側での接続Noに対応させて全員分の返答を待つ
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
		if(m_pReceiveCallBack){
			m_pReceiveCallBack(&((jbyte*)pData)[2]);
		}
		if(pReceiveData->bReqEnd){
			RequestEnd();
		}
	}
}

void TransferGameInfo::SetGameInfoData(void* pData, int nSize)
{
	m_Data.pData = pData;
	m_nSize = nSize;
}

void TransferGameInfo::SetReceiveCallBack(std::function<void(void*)> pCallBack)
{
	m_pReceiveCallBack = pCallBack;
}
void TransferGameInfo::SetDumpCallBack(std::function<void()> pCallBack)
{
	m_pDumpCallBack = pCallBack;
}

void TransferGameInfo::Dump()
{
	if(m_pDumpCallBack){
		m_pDumpCallBack();
	}
}