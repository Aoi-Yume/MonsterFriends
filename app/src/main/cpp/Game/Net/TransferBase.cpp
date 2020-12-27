//
// Created by 葵ユメ on 2020/12/19.
//


#include "TransferBase.h"

TransferBase::TransferBase(int nKind, int nSleepNanoSec)
: m_uReqest(eRequest_None)
, m_nKind(nKind)
, m_nSendCnt()
, m_nReceiveCnt()
, m_nSleepNanoSec(nSleepNanoSec)
, m_thread()
{
}

TransferBase::~TransferBase()
{
	if(m_thread.joinable()){
		m_thread.join();
	}
}

void TransferBase::RequestEnd()
{
	m_uReqest |= eRequest_End;
}

bool TransferBase::IsEnd() const
{
	return (m_uReqest & eRequest_Complete) != 0;
}

void TransferBase::startTransfer()
{
	m_thread = std::thread(&TransferBase::onThread, this);
}
void TransferBase::stopTransfer()
{
	m_uReqest |= eRequest_Stop;
}

void TransferBase::onThread()
{
	GetThreadEnv();
	while(true){
		updateTransfer();
		if((m_uReqest & eRequest_End) != 0U){ break; }
		if((m_uReqest & eRequest_Stop) != 0U){ continue; }
		std::this_thread::sleep_for(std::chrono::nanoseconds(m_nSleepNanoSec));
	}
	DetachThreadEnv();
	m_uReqest |= eRequest_Complete;
};