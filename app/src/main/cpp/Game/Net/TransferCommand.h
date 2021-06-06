﻿//
// Created by 葵ユメ on 2021/06/06.
//


#ifndef AOIYUME_TRANSFER_COMMAND_H
#define AOIYUME_TRANSFER_COMMAND_H

#include <thread>
#include <list>
#include "../Engine/Engine.h"
#include "TransferBase.h"

class TransferManager;

class TransferCommand : public TransferBase
{
	friend class TransferManager;
public:
	enum CommandKind : uint8_t {
		eCommand_None,
		eCommand_PushAdv,
		eCommand_PushNext,
		eCommand_PushItem,
		eCommand_PushBag,
		eCommand_PushShop,
		eCommand_PushStopDice,
		eCommand_PushItem_1,
		eCommand_PushItem_2,
		eCommand_PushItem_3,
		eCommand_PushItemLeft,
		eCommand_PushItemRight,
		eCommand_Buy,
		eCommand_BuyCancel,
		eCommand_Back,
		eCommand_Use,
		eCommand_UseCancel,
		eCommand_GameMainNextMessage,
		eCommand_ShopNextMessage,
		eCommand_UseItemNextMessage,
	};

	struct Command {
		int nPlayerId;
		CommandKind uCommand;
	};

public:
	TransferCommand();
	virtual ~TransferCommand();

protected:
	void initialize() override ;
	bool updateTransfer() override;
	void updateReceive(const char* Id, void* pData) override;

public:
	void SetSendCommand(CommandKind uCommand){ m_SendData.command.uCommand = uCommand; }
	bool IsCommandEmpty() const { return m_CommandQue.empty(); }
	const Command& GetCommand() const { return m_CommandQue.front(); }
	void PopFrontCommand() { m_CommandQue.pop_front(); }

	void Dump() override;

private:
	
	struct Data {
		uint8_t  	uKind;
		Command		command;
	};
	Data m_SendData;
	std::list<Command>	m_CommandQue;
};

#endif