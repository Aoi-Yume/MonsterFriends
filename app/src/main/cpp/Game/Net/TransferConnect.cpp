//
// Created by 葵ユメ on 2020/12/27.
//


#include "TransferConnect.h"
#include "TransferManager.h"
#include <Random.h>

TransferConnect::TransferConnect()
: TransferBase(TransferManager::eTransferKind_Connect, 500 * 1000 * 1000)
, m_Data()
{
}

TransferConnect::~TransferConnect()
{
}

void TransferConnect::initialize()
{
	m_Data.uKind = TransferManager::eTransferKind_Connect;
	m_Data.bHost = TransferManager::Get()->IsHost();
	m_Data.uRandomSeed = Random::GetSyncSeed();
}

bool TransferConnect::updateTransfer()
{
	const int nConnectNum = TransferManager::Get()->GetConnectNum();
	for(int i = 0; i < nConnectNum; ++i){
		const auto& info = TransferManager::Get()->GetConnect(i);
		std::snprintf(m_Data.IDInfo[i].OpponentId, sizeof(m_Data.IDInfo[i].OpponentId), "%s", info.Id.c_str());
		m_Data.IDInfo[i].OpponentId[sizeof(m_Data.IDInfo[i].OpponentId) - 1] = '\0';
	}
	TransferManager::Get()->BroadCast((jbyte*)&m_Data, sizeof(m_Data));
	return true;
}
void TransferConnect::updateReceive(const char* Id, void* pData)
{
	if(IsEnd()){ return; }
	if(std::strlen(m_Data.SelfId) > 0){ return; }
	TransferBase::updateReceive(Id, pData);

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
		std::snprintf(m_Data.SelfId, sizeof(m_Data.SelfId), "%s", idInfo.OpponentId);
		m_Data.SelfId[sizeof(m_Data.SelfId) - 1] = '\0';

		TransferManager::ConnectInfo info(m_Data.SelfId, "");
		pManager->SetSelfConnect(info);
		RequestEnd();
	}
	if(pReceiveData->bHost){
		Random::SetSyncSeed(pReceiveData->uRandomSeed);
	}
	pManager->SetConnectHost(Id, pReceiveData->bHost);
}

void TransferConnect::Dump()
{
	DEBUG_LOG_A("SelfId:[%s]\n", m_Data.SelfId);
	for(int i = 0; i < TransferManager::Get()->GetConnectNum(); ++i){
		DEBUG_LOG_A("ConnectId:[%s]\n", m_Data.IDInfo[i].OpponentId);
	}
}