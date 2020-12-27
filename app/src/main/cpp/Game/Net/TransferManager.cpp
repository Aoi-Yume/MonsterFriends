//
// Created by 葵ユメ on 2020/12/19.
//


#include "TransferManager.h"
#include "TransferBase.h"
#include "TransferConnect.h"
USE_SINGLETON_VARIABLE(TransferManager);

TransferManager::TransferManager()
	: m_aTransfer()
	, m_aConnectInfo()
{
	DEBUG_LOG("Call TransferManager Constructor");
	m_aTransfer.resize(eTransferKind_Max);
	SetTransfer(eTransferKind_Connect, new TransferConnect);
}

TransferManager::~TransferManager()
{
    DEBUG_LOG("Call TransferManager Deconstructor");
	// 終了要求
	for(auto& it : m_aTransfer){
		it->RequestEnd();
	}
	// 終了待ち
	do{
		bool bAllEnd = true;
		for(auto& it : m_aTransfer){
			if(!it->IsEnd()){
				bAllEnd = false;
				break;
			}
		}
		if(bAllEnd){ break; }
	} while(true);
	for(auto& it : m_aTransfer) {
		delete it;
		it = nullptr;
	}
}

void TransferManager::SetTransfer(int nKind, TransferBase* pTransfer)
{
	assert(nKind >= 0 && nKind < eTransferKind_Max);
	m_aTransfer[nKind] = pTransfer;
}

void TransferManager::StartTransfer(int nKind)
{
	assert(nKind >= 0 && nKind < eTransferKind_Max);
	m_aTransfer[nKind]->startTransfer();
}

void TransferManager::StopTransfer(int nKind)
{
	assert(nKind >= 0 && nKind < eTransferKind_Max);
	m_aTransfer[nKind]->stopTransfer();
}

void TransferManager::AddConnect(const ConnectInfo &info)
{
	m_aConnectInfo.emplace_back(info);
	DEBUG_LOG_A("TransferManager:AddConnect[%s, %s]\n", info.Id.c_str(), info.Name.c_str());
}

const TransferManager::ConnectInfo& TransferManager::GetConnect(int nNo) const
{
	return m_aConnectInfo[nNo];
}

int TransferManager::GetConnectNum() const
{
	return m_aConnectInfo.size();
}

void TransferManager::BroadCast(jbyte *pData, int nSize)
{
	for(const auto& it : m_aConnectInfo){
		jbyteArray byteData = GetEnv()->NewByteArray(nSize);
		GetEnv()->SetByteArrayRegion(byteData, 0, nSize, pData);

		jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
		jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "SendData");
		jstring Id = GetEnv()->NewStringUTF(it.Id.c_str());
		GetEnv()->CallStaticVoidMethod(classID, methodID_1, Id, byteData);
	}
}

void TransferManager::ReceiveData(const char *id, jbyte* pData, int nSize)
{
	uint8_t uKind = *pData;
	if(uKind >= 0 && uKind < eTransferKind_Max){
		m_aTransfer[uKind]->updateReceive(id, pData);
	}
}