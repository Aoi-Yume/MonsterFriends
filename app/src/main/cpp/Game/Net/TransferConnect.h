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
	void updateTransfer() override;
	void updateReceive(const char* Id, void* pData) override;

public:
	void Dump() override;

private:
	struct Data {
		uint8_t  	uKind;
		char SelfId[16];
		struct IDInfo {
			char OpponentId[16];
		};
		IDInfo		IDInfo[NET_CONNECT_MAX - 1];
	};
	Data m_Data;
};

#endif