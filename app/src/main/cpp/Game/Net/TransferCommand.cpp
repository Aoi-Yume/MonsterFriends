//
// Created by 葵ユメ on 2021/06/06.
//


#include "TransferCommand.h"
#include "TransferManager.h"

TransferCommand::TransferCommand()
: TransferBase(TransferManager::eTransferKind_Command, 50 * 1000 * 1000)
, m_Mutex()
, m_aSendData()
, m_CommandQue()
{
}

TransferCommand::~TransferCommand()
{
}

void TransferCommand::initialize()
{
	DEBUG_LOG_A("【Net】%s\n", __PRETTY_FUNCTION__ );
	m_aSendData.clear();
	m_CommandQue.clear();
}

bool TransferCommand::updateTransfer()
{
	if(m_aSendData.empty()){ return false; }

	const auto pManager = TransferManager::Get();
	for(const auto& it : m_aSendData) {
		pManager->BroadCast((jbyte *) &it, sizeof(Data));
	}
	DEBUG_LOG("-----【Net】Set Command---\n");
	for(const auto& it : m_aSendData){
		DEBUG_LOG_A("[%d]\n", it.command.uCommand);
	}
	DEBUG_LOG("-----------------------\n");
	m_aSendData.clear();

	return true;
}
bool TransferCommand::updateReceive(const char* Id, void* pData, size_t  size)
{
	if(IsEnd()){ return false; }
	if(size != sizeof(Data)){ return false; }

	std::lock_guard<std::mutex> lock(m_Mutex);

	Data* pReceiveData = (Data*)pData;
	m_CommandQue.emplace_back(pReceiveData->command);

	DEBUG_LOG("----【Net】Receive Command----\n");
	for(const auto& it : m_CommandQue){
		DEBUG_LOG_A("[%d]\n", it.uCommand);
	}
	DEBUG_LOG("----------------------------\n");

	return true;
}

void TransferCommand::SetSendCommand(CommandKind uCommand)
{
	Data data ={};
	data.uKind = TransferManager::eTransferKind_Command;
	data.command.nPlayerId = TransferManager::Get()->GetSelfConnect().nPlayerId;
	data.command.uCommand = uCommand;
	m_aSendData.emplace_back(data);
}

void TransferCommand::Dump()
{
}