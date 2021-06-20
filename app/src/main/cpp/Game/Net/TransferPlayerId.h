//
// Created by 葵ユメ on 2020/12/31.
//


#ifndef AOIYUME_TRANSFER_PLAYERID_H
#define AOIYUME_TRANSFER_PLAYERID_H

#include <thread>
#include "../Engine/Engine.h"
#include "TransferBase.h"

class TransferManager;

class TransferPlayerId : public TransferBase
{
	friend class TransferManager;
public:
	TransferPlayerId();
	virtual ~TransferPlayerId();

protected:
	void initialize() override ;
	bool updateTransfer() override;
	bool updateReceive(const char* Id, void* pData, size_t size) override;

public:
	void Dump() override;

private:
	struct Data {
		uint8_t  	uKind;
		bool 		bReqEnd;
		struct IDInfo {
			int8_t	nPlayerId;
			char 	cNetId[16];
		};
		IDInfo		IDInfo[NET_CONNECT_MAX];
	};
	bool m_bReceive[NET_CONNECT_MAX];
	Data m_Data;
};

#endif