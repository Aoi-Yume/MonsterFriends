//
// Created by 葵ユメ on 2020/12/19.
//


#ifndef AOIYUME_TRANSFER_BASE_H
#define AOIYUME_TRANSFER_BASE_H

#include "../Engine/Engine.h"
#include <thread>

class TransferManager;

class TransferBase
{
	friend class TransferManager;
public:
	TransferBase(int nKind, int nSleepNanoSec);
	virtual ~TransferBase();

	void RequestEnd();
	bool IsStart() const;
	bool IsEnd() const;

protected:
	void startTransfer();
	void stopTransfer();

	int getSendCnt() const;
	int getReceiveCnt() const;

	virtual void initialize() = 0;
	virtual bool updateTransfer() = 0;
	virtual void updateReceive(const char* Id, void* pData);
	
private:
	void onThread();

public:
	virtual void Dump() = 0;

private:
	enum RequestBit{
		eRequest_None = 0,
		eRequest_End = (1 << 0),
		eRequest_Stop = (1 << 1),
		eRequest_Complete = (1 << 2),
	};

	uint 		m_uReqest;
	int 		m_nKind;
	int 		m_nSendCnt;
	int 		m_nReceiveCnt;
	int			m_nSleepNanoSec;
	std::thread	m_thread;
	std::mutex 	m_mutex;
};

#endif