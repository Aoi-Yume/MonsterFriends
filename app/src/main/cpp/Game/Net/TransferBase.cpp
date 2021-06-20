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
, m_Mutex()
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

bool TransferBase::IsStart() const
{
	return m_thread.joinable() && !IsEnd();
}

bool TransferBase::IsEnd() const
{
	return (m_uReqest & eRequest_Complete) != 0;
}

void TransferBase::startTransfer()
{
	if(m_thread.joinable()){
		m_thread.join();
	}
	m_uReqest = 0;
	m_nSendCnt = 0;
	m_nReceiveCnt = 0;
	m_thread = std::thread(&TransferBase::onThread, this);
}
void TransferBase::stopTransfer()
{
	m_uReqest |= eRequest_Stop;
}

int TransferBase::getSendCnt() const
{
	return m_nSendCnt;
}

int TransferBase::getReceiveCnt() const
{
	return m_nReceiveCnt;
}

bool TransferBase::updateReceive(const char *Id, void *pData, size_t size)
{
	(void)(Id);
	(void)(pData);
	return false;
}

bool TransferBase::checkReceive()
{
	std::lock_guard<std::mutex> guard(m_Mutex);
	if(m_aReceiveQue.empty()){ return false; }
	const ReceiveData& data = m_aReceiveQue.front();
	const bool bRet = updateReceive(data.Id, (void*)data.data, data.size);
	m_aReceiveQue.pop_front();
	return bRet;
}

void TransferBase::onThread()
{
	GetThreadEnv();
	initialize();
	while(true){
		if((m_uReqest & eRequest_Stop) != 0U){
			if((m_uReqest & eRequest_End) != 0U){ break; }
			continue;
		}
		if(checkReceive()){
			m_nReceiveCnt++;
		}
		if(updateTransfer()) {
			m_nSendCnt++;
		}
		if((m_uReqest & eRequest_End) != 0U){ break; }
		std::this_thread::sleep_for(std::chrono::nanoseconds(m_nSleepNanoSec));
	}
	DetachThreadEnv();
	m_uReqest |= eRequest_Complete;
};