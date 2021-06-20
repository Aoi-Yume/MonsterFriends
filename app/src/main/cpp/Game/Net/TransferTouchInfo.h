//
// Created by 葵ユメ on 2020/12/31.
//


#ifndef AOIYUME_TRANSFER_TOUCHINFO_H
#define AOIYUME_TRANSFER_TOUCHINFO_H

#include <thread>
#include "../Engine/Engine.h"
#include "TransferBase.h"
#include <DelayInput.h>

class TransferManager;

class TransferTouchInfo : public TransferBase
{
	friend class TransferManager;
public:
	TransferTouchInfo();
	virtual ~TransferTouchInfo();

protected:
	void initialize() override ;
	bool updateTransfer() override;
	bool updateReceive(const char* Id, void* pData, size_t size) override;

public:
	void SetReceiveCallBack(std::function<void(void*, int)> pCallBack);
	void SetDumpCallBack(std::function<void()> pCallBack);
	void Dump() override;

private:
	struct Data {
		uint8_t uKind;
		DelayTouchInfo info;
	};

	float 	m_lastSendTime;
	Data		m_Data;
	std::function<void(void*, int)>	m_pReceiveCallBack;
	std::function<void()>	m_pDumpCallBack;
};

#endif