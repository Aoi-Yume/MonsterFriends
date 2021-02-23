//
// Created by 葵ユメ on 2021/02/23.
//


#ifndef AOIYUME_TRANSFER_SKILLINFO_H
#define AOIYUME_TRANSFER_SKILLINFO_H

#include <thread>
#include "../Engine/Engine.h"
#include "TransferBase.h"

class TransferManager;

class TransferSkillInfo : public TransferBase
{
	friend class TransferManager;
public:
	TransferSkillInfo();
	virtual ~TransferSkillInfo();

protected:
	void initialize() override ;
	bool updateTransfer() override;
	void updateReceive(const char* Id, void* pData) override;

public:
	void SetSkillInfoData(void* pData, int nSize, int nSendPlayer);
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
	int 	m_nSendPlayer;
	std::function<void(void*)>	m_pReceiveCallBack;
	std::function<void()>	m_pDumpCallBack;
};

#endif