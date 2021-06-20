//
// Created by 葵ユメ on 2020/12/27.
//


#include "TransferConnect.h"
#include "TransferManager.h"
#include <Random.h>
#include <AppParam.h>

TransferConnect::TransferConnect()
: TransferBase(TransferManager::eTransferKind_Connect, 500 * 1000 * 1000)
, m_Data()
, m_bReceiveSelfConnect(false)
, m_bReceiveHost(false)
{
}

TransferConnect::~TransferConnect()
{
}

void TransferConnect::initialize()
{
	DEBUG_LOG_A("【Net】%s\n", __PRETTY_FUNCTION__ );
	memset(&m_Data, 0, sizeof(m_Data));
	auto pManager = TransferManager::Get();
	m_Data.uKind = TransferManager::eTransferKind_Connect;
	m_Data.bHost = TransferManager::Get()->IsHost();
	m_Data.uCharaId = TransferManager::Get()->GetSelfCharaId();
	m_Data.uRandomSeed = Random::GetSyncSeed();
	Random::SetSyncSeed(m_Data.uRandomSeed);

	m_bReceiveSelfConnect = false;
	m_bReceiveHost = false;
}

bool TransferConnect::updateTransfer()
{
	if(!isSendPossible()){ return false; }

	const int nConnectNum = TransferManager::Get()->GetConnectNum();
	for(int i = 0; i < nConnectNum; ++i){
		const auto& info = TransferManager::Get()->GetConnect(i);
		std::snprintf(m_Data.IDInfo[i].OpponentId, sizeof(m_Data.IDInfo[i].OpponentId), "%s", info.Id.c_str());
		m_Data.IDInfo[i].OpponentId[sizeof(m_Data.IDInfo[i].OpponentId) - 1] = '\0';
	}
	TransferManager::Get()->BroadCast((jbyte*)&m_Data, sizeof(m_Data));
	return true;
}
bool TransferConnect::updateReceive(const char* Id, void* pData, size_t size)
{
	if(IsEnd()){ return false; }
	if(size != sizeof(Data)){ return false; }

	auto pManager = TransferManager::Get();
	Data* pReceiveData = (Data*)pData;
	const int nConnectNum = pManager->GetConnectNum();
	for(int i = 0; i < nConnectNum; ++i){
		const auto& idInfo = pReceiveData->IDInfo[i];
		bool bHit = false;
		for(int j = 0; j < nConnectNum; ++j){
			if(std::strcmp(idInfo.OpponentId, m_Data.IDInfo[j].OpponentId) == 0){
				bHit = true;
				break;
			}
		}
		if(bHit){
			continue;
		}
		char selfId[16];
		std::snprintf(selfId, sizeof(selfId), "%s", idInfo.OpponentId);
		selfId[sizeof(selfId) - 1] = '\0';

		TransferManager::ConnectInfo info(selfId, "", TransferManager::Get()->GetSelfCharaId());
		pManager->SetSelfConnect(info);
		m_bReceiveSelfConnect = true;
	}
	if(pReceiveData->bHost){
		Random::SetSyncSeed(pReceiveData->uRandomSeed);
		m_bReceiveHost = true;
	}
	pManager->SetConnectHost(Id, pReceiveData->bHost);
	pManager->SetConnectCharaId(Id, pReceiveData->uCharaId);

	if(m_bReceiveSelfConnect && (m_bReceiveHost || m_Data.bHost)){
		RequestEnd();
	}
	return true;
}

void TransferConnect::Dump()
{
	auto pManager = TransferManager::Get();
	DEBUG_LOG_A("SelfId:[%s], CharaId[%u]\n", pManager->GetSelfConnect().Id.c_str(), pManager->GetSelfCharaId());
	for(int i = 0; i < TransferManager::Get()->GetConnectNum(); ++i){
		DEBUG_LOG_A("ConnectId:[%s], CharaId[%u]\n", m_Data.IDInfo[i].OpponentId, pManager->GetConnect(i).uCharaId);
	}
}