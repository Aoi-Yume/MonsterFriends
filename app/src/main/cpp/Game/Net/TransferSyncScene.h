//
// Created by 葵ユメ on 2021/01/04.
//


#ifndef AOIYUME_TRANSFER_SYNC_SCENE_H
#define AOIYUME_TRANSFER_SYNC_SCENE_H

#include <thread>
#include "../Engine/Engine.h"
#include "TransferBase.h"

class TransferManager;

class TransferSyncScene : public TransferBase
{
	friend class TransferManager;
public:
	TransferSyncScene();
	virtual ~TransferSyncScene();

protected:
	void initialize() override ;
	bool updateTransfer() override;
	bool updateReceive(const char* Id, void* pData, size_t size) override;

public:
	void Dump() override;

private:
	struct Data {
		uint8_t  	uKind;
		bool		bReqEnd;
	};
	bool m_bReceive[NET_CONNECT_MAX];
	Data m_Data;
};

#endif