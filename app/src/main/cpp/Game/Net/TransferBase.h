//
// Created by 葵ユメ on 2020/12/19.
//


#ifndef AOIYUME_TRANSFER_BASE_H
#define AOIYUME_TRANSFER_BASE_H

#include "../Engine/Engine.h"
#include <thread>
#include <deque>

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

	void AddReceive(const char* pId, const jbyte* pData, size_t size) {
		std::lock_guard<std::mutex> guard(m_Mutex);
		m_aReceiveQue.emplace_back(pId, pData, size);
	}

protected:
	void startTransfer();
	void stopTransfer();

	int getSendCnt() const;
	int getReceiveCnt() const;
	bool isSendPossible() const{ return (m_nReceiveCnt >= m_nSendCnt); }

	virtual void initialize() = 0;
	virtual bool updateTransfer() = 0;
	virtual bool updateReceive(const char* Id, void* pData, size_t size);
	
private:
	bool checkReceive();
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

	struct ReceiveData{
		ReceiveData(const char* pId, const jbyte* pData, size_t size){
			ASSERT_MSG_A(size < sizeof(data), "%d < %d", size, sizeof(data));
			std::snprintf(Id, sizeof(Id), "%s", pId);
			memcpy(data, pData, size);
			this->size = size;
		}
		char 	Id[16];
		jbyte	data[1024];
		size_t	size;
	};

	uint 		m_uReqest;
	int 		m_nKind;
	int 		m_nSendCnt;
	int 		m_nReceiveCnt;
	int			m_nSleepNanoSec;
	std::thread	m_thread;
	std::deque<ReceiveData>	m_aReceiveQue;
	std::mutex m_Mutex;
};

#endif