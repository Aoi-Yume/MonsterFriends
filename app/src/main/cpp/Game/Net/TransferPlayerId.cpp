//
// Created by 葵ユメ on 2020/12/31.
//


#include "TransferPlayerId.h"
#include "TransferManager.h"

TransferPlayerId::TransferPlayerId()
: TransferBase(TransferManager::eTransferKind_PlayerId, 500 * 1000 * 1000)
, m_bReceive()
, m_Data()
{
}

TransferPlayerId::~TransferPlayerId()
{
}

void TransferPlayerId::initialize()
{
	DEBUG_LOG_A("【Net】%s\n", __PRETTY_FUNCTION__ );
	m_Data.uKind = TransferManager::eTransferKind_PlayerId;
	m_Data.bReqEnd = false;

	const auto pManager = TransferManager::Get();
	if(pManager->IsHost()) {
		{
			auto& dataInfo = m_Data.IDInfo[0];
			const auto &info = pManager->GetSelfConnect();
			std::snprintf(dataInfo.cNetId, sizeof(dataInfo.cNetId), "%s", info.Id.c_str());
			dataInfo.cNetId[sizeof(dataInfo.cNetId) - 1] = '\0';
			dataInfo.nPlayerId = 0;
			pManager->SetConnectPlayerId(dataInfo.cNetId, dataInfo.nPlayerId);
		}
		for (int i = 0; i < pManager->GetConnectNum(); ++i) {
			auto& dataInfo = m_Data.IDInfo[i + 1];
			const auto& info = pManager->GetConnect(i);
			std::snprintf(dataInfo.cNetId, sizeof(dataInfo.cNetId), "%s", info.Id.c_str());
			dataInfo.cNetId[sizeof(dataInfo.cNetId) - 1] = '\0';
			dataInfo.nPlayerId = i + 1;
			pManager->SetConnectPlayerId(dataInfo.cNetId, dataInfo.nPlayerId);
		}
	}
	for(int i = 0; i < NET_CONNECT_MAX; ++i){
		m_bReceive[i] = false;
	}
}

bool TransferPlayerId::updateTransfer()
{
	if(!isSendPossible()){ return false; }

	const auto pManager = TransferManager::Get();
	if(pManager->IsHost()) {
		pManager->BroadCast((jbyte *) &m_Data, sizeof(m_Data));
		if(m_Data.bReqEnd){
			RequestEnd();
		}
		return true;
	}
	else if(getReceiveCnt() >= 1){
		pManager->SendHost((jbyte*)&m_Data, sizeof(m_Data));
		DEBUG_LOG("SendReceive\n");
		return true;
	}
	return false;
}
bool TransferPlayerId::updateReceive(const char* Id, void* pData, size_t size)
{
	if(IsEnd()){ return false; }
	if(size != sizeof(Data)){ return false; }

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
		// 通信相手 + 自分の分
		for(int i = 0; i < nConnectNum + 1; ++i){
			const auto& idInfo = pReceiveData->IDInfo[i];
			pManager->SetConnectPlayerId(idInfo.cNetId, idInfo.nPlayerId);
		}
		if(pReceiveData->bReqEnd){
			RequestEnd();
		}
	}
	return true;
}

void TransferPlayerId::Dump()
{
	const auto pManager = TransferManager::Get();
	const auto& self = pManager->GetSelfConnect();
	DEBUG_LOG_A("[%s], PlayerId:[%d]\n", self.Id.c_str(), self.nPlayerId);
	for(int i = 0; i < pManager->GetConnectNum(); ++i){
		const auto& info = pManager->GetConnect(i);
		DEBUG_LOG_A("[%s], PlayerId[%d]\n", info.Id.c_str(), info.nPlayerId);
	}
}