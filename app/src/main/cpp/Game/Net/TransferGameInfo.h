//
// Created by 葵ユメ on 2020/12/31.
//


#ifndef AOIYUME_TRANSFER_GAMEINFO_H
#define AOIYUME_TRANSFER_GAMEINFO_H

#include <thread>
#include "../Engine/Engine.h"
#include "TransferBase.h"

class TransferManager;

class TransferGameInfo : public TransferBase
{
	friend class TransferManager;
public:
	TransferGameInfo();
	virtual ~TransferGameInfo();

protected:
	void initialize() override ;
	bool updateTransfer() override;
	void updateReceive(const char* Id, void* pData) override;

public:
	void SetGameInfoData(void* pData, int nSize);
	void SetReceiveCallBack(std::function<void(void*)> pCallBack);
	void SetDumpCallBack(std::function<void()> pCallBack);
	void Dump() override;

private:
	struct Data {
		uint8_t uKind;
		bool 	bReqEnd;
		void*	pData;
	};

	bool 	m_bReceive[NET_CONNECT_MAX];
	Data	m_Data;
	int		m_nSize;
	std::function<void(void*)>	m_pReceiveCallBack;
	std::function<void()>	m_pDumpCallBack;
};

#endif