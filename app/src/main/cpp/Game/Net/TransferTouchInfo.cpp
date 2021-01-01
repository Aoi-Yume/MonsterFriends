//
// Created by 葵ユメ on 2020/12/31.
//


#include "TransferTouchInfo.h"
#include "TransferManager.h"

TransferTouchInfo::TransferTouchInfo()
: TransferBase(TransferManager::eTransferKind_TouchInfo, 50 * 1000 * 1000)
, m_lastSendFrame(0U)
, m_Data()
, m_pReceiveCallBack()
, m_pDumpCallBack()
{
}

TransferTouchInfo::~TransferTouchInfo()
{
}

void TransferTouchInfo::initialize()
{
	m_Data.uKind = TransferManager::eTransferKind_TouchInfo;
	m_lastSendFrame = 0U;
}

bool TransferTouchInfo::updateTransfer()
{
	const auto pManager = TransferManager::Get();
	const int nPlayerId = pManager->GetSelfConnect().nPlayerId;
	if(DELAY_INPUT()->IsInputEmpty(nPlayerId)){ return false; }

	m_Data.info = DELAY_INPUT()->GetNextDelayTouchInfo(nPlayerId);
	if(m_lastSendFrame == m_Data.info.uFrame){ return false; }
	m_lastSendFrame = m_Data.info.uFrame;

	pManager->BroadCast((jbyte *)&m_Data, sizeof(m_Data));
	return true;
}
void TransferTouchInfo::updateReceive(const char* Id, void* pData)
{
	if(IsEnd()){ return; }
	TransferBase::updateReceive(Id, pData);

	Data* pReceiveData = (Data*)pData;
	DELAY_INPUT()->AddDelayTouchInfo(pReceiveData->info, TransferManager::Get()->GetPlayerIdFromNetId(Id));
}

void TransferTouchInfo::SetReceiveCallBack(std::function<void(void*, int)> pCallBack)
{
	m_pReceiveCallBack = pCallBack;
}
void TransferTouchInfo::SetDumpCallBack(std::function<void()> pCallBack)
{
	m_pDumpCallBack = pCallBack;
}

void TransferTouchInfo::Dump()
{
	if(m_pDumpCallBack){
		m_pDumpCallBack();
	}
}