//
// Created by 葵ユメ on 2021/01/04.
//


#include "TransferSyncScene.h"
#include "TransferManager.h"

TransferSyncScene::TransferSyncScene()
: TransferBase(TransferManager::eTransferKind_SyncScene, 100 * 1000 * 1000)
, m_bReceive()
, m_Data()
{
}

TransferSyncScene::~TransferSyncScene()
{
}

void TransferSyncScene::initialize()
{
	m_Data.uKind = TransferManager::eTransferKind_SyncScene;
	m_Data.bReqEnd = false;
	for(int i = 0; i < NET_CONNECT_MAX; ++i){
		m_bReceive[i] = false;
	}
}

bool TransferSyncScene::updateTransfer()
{
	const auto pManager = TransferManager::Get();
	if(pManager->IsHost()) {
		pManager->BroadCast((jbyte *) &m_Data, sizeof(m_Data));
		if(m_Data.bReqEnd){
			RequestEnd();
		}
		return true;
	}
	else if(getReceiveCnt() >= 1 && getSendCnt() == 0){
		pManager->SendHost((jbyte*)&m_Data, sizeof(m_Data));
		return true;
	}
	return false;
}
void TransferSyncScene::updateReceive(const char* Id, void* pData)
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
		if(pReceiveData->bReqEnd){
			RequestEnd();
		}
	}
}

void TransferSyncScene::Dump()
{
}