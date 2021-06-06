//
// Created by 葵ユメ on 2021/06/06.
//


#include "TransferCommand.h"
#include "TransferManager.h"

TransferCommand::TransferCommand()
: TransferBase(TransferManager::eTransferKind_Command, 50 * 1000 * 1000)
, m_SendData()
, m_CommandQue()
{
}

TransferCommand::~TransferCommand()
{
}

void TransferCommand::initialize()
{
	m_SendData.uKind = TransferManager::eTransferKind_Command;
	m_SendData.command.nPlayerId = TransferManager::Get()->GetSelfConnect().nPlayerId;
	m_CommandQue.clear();
}

bool TransferCommand::updateTransfer()
{
	if(m_SendData.command.uCommand == eCommand_None){ return false; }

	const auto pManager = TransferManager::Get();
	pManager->BroadCast((jbyte *) &m_SendData, sizeof(m_SendData));
	m_SendData.command.uCommand = eCommand_None;

	return true;
}
void TransferCommand::updateReceive(const char* Id, void* pData)
{
	if(IsEnd()){ return; }
	TransferBase::updateReceive(Id, pData);

	Data* pReceiveData = (Data*)pData;
	m_CommandQue.emplace_back(pReceiveData->command);
}

void TransferCommand::Dump()
{
}