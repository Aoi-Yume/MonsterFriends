//
// Created by 葵ユメ on 2020/12/27.
//


#ifndef AOIYUME_TRANSFER_CONNECT_H
#define AOIYUME_TRANSFER_CONNECT_H

#include <thread>
#include "../Engine/Engine.h"
#include "TransferBase.h"

class TransferManager;

class TransferConnect : public TransferBase
{
	friend class TransferManager;
public:
	TransferConnect();
	virtual ~TransferConnect();

protected:
	void initialize() override;
	bool updateTransfer() override;
	bool updateReceive(const char* Id, void* pData, size_t size) override;

public:
	void Dump() override;

private:
	struct Data {
		uint8_t  	uKind;
		bool bHost;
		unsigned int uRandomSeed;
		uint8_t 	uCharaId;
		struct IDInfo {
			char OpponentId[16];
		};
		IDInfo		IDInfo[NET_CONNECT_MAX - 1];
	};
	Data m_Data;

	bool m_bReceiveSelfConnect;
	bool m_bReceiveHost;
};

#endif