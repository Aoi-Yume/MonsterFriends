﻿//
// Created by 葵ユメ on 2020/12/19.
//


#include "TransferManager.h"
#include "TransferBase.h"
#include "TransferConnect.h"
#include "TransferPlayerId.h"
#include "TransferSyncScene.h"
#include "TransferCommand.h"
#include "TransferTouchInfo.h"
#include "TransferGameInfo.h"
#include "TransferSkillInfo.h"

USE_SINGLETON_VARIABLE(TransferManager);

TransferManager::TransferManager()
	: m_bHost(false)
	, m_bConnectSucess(false)
	, m_SelfInfo()
	, m_aTransfer()
	, m_aConnectInfo()
{
	DEBUG_LOG("Call TransferManager Constructor");
}

TransferManager::~TransferManager()
{
    DEBUG_LOG("Call TransferManager Deconstructor");
	ResetConnect();
}

void TransferManager::Initialize(bool bHost)
{
	m_bHost = bHost;
	m_aTransfer.resize(eTransferKind_Max);
	SetTransfer(eTransferKind_Connect, new TransferConnect);
	SetTransfer(eTransferKind_PlayerId, new TransferPlayerId);
	SetTransfer(eTransferKind_SyncScene, new TransferSyncScene);
	SetTransfer(eTransferKind_Command, new TransferCommand);
	SetTransfer(eTransferKind_TouchInfo, new TransferTouchInfo);
	SetTransfer(eTransferKind_GameInfo, new TransferGameInfo);
	SetTransfer(eTransferKind_SkillInfo, new TransferSkillInfo);
}

void TransferManager::SetConnectSuccess(bool bConnect)
{
	m_bConnectSucess = bConnect;
}
bool TransferManager::IsConnectSucess() const
{
	return m_bConnectSucess;
}

void TransferManager::ResetConnect()
{
	m_SelfInfo = {};
	m_aConnectInfo.clear();
	// 終了要求
	for(auto& it : m_aTransfer){
		it->RequestEnd();
	}
	// 終了待ち
	do{
		bool bAllEnd = true;
		for(auto& it : m_aTransfer){
			if(!it->IsEnd() && it->IsStart()){
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
	ASSERT(nKind >= 0 && nKind < eTransferKind_Max);
	m_aTransfer[nKind] = pTransfer;
}

void TransferManager::StartTransfer(int nKind)
{
	ASSERT(nKind >= 0 && nKind < eTransferKind_Max);
	m_aTransfer[nKind]->startTransfer();
}

void TransferManager::StopTransfer(int nKind)
{
	ASSERT(nKind >= 0 && nKind < eTransferKind_Max);
	m_aTransfer[nKind]->stopTransfer();
}

void TransferManager::StopAllTransfer()
{
	for(int i = 0; i < eTransferKind_Max; ++i){
		m_aTransfer[i]->stopTransfer();
	}
}

bool TransferManager::IsStartTransfer(int nKind) const
{
	ASSERT(nKind >= 0 && nKind < eTransferKind_Max);
	return m_aTransfer[nKind]->IsStart();
}

bool TransferManager::IsHost() const
{
	return m_bHost;
}

void TransferManager::SetSelfConnect(const ConnectInfo &info)
{
	m_SelfInfo = info;
}

const TransferManager::ConnectInfo& TransferManager::GetSelfConnect() const
{
	return m_SelfInfo;
}

void TransferManager::SetSelfCharaId(uint8_t uCharaId)
{
	m_SelfInfo.uCharaId = uCharaId;
}

uint8_t TransferManager::GetSelfCharaId() const
{
	return m_SelfInfo.uCharaId;
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

const TransferManager::ConnectInfo& TransferManager::GetHostConnect() const
{
	if(m_bHost){ return m_SelfInfo; }
	for(auto& info : m_aConnectInfo){
		if(info.bHost){ return info; }
	}
	abort();
}

int TransferManager::GetConnectNum() const
{
	return m_aConnectInfo.size();
}

void TransferManager::SetConnectHost(const char* id, bool bHost)
{
	if(IsSelfConnectId(id)) {
		m_SelfInfo.bHost = bHost;
	}
	else {
		int nNo = GetConnectNoFromNetId(id);
		if(nNo >= 0) {
			m_aConnectInfo[nNo].bHost = bHost;
		}
	}
}

void TransferManager::SetConnectCharaId(const char* id, uint8_t uCharaId)
{
	if(IsSelfConnectId(id)) {
		m_SelfInfo.uCharaId = uCharaId;
	}
	else {
		int nNo = GetConnectNoFromNetId(id);
		if(nNo >= 0) {
			m_aConnectInfo[nNo].uCharaId = uCharaId;
		}
	}
}

void TransferManager::SetConnectPlayerId(const char* id, int nPlayerId)
{
	if(IsSelfConnectId(id)){
		m_SelfInfo.nPlayerId = nPlayerId;
	}
	else{
		int nNo = GetConnectNoFromNetId(id);
		if(nNo >= 0) {
			m_aConnectInfo[nNo].nPlayerId = nPlayerId;
		}
	}
}

void TransferManager::SendHost(jbyte* pData, int nSize)
{
	sendData(GetHostConnect().Id.c_str(), pData, nSize);
}

void TransferManager::SendPlayer(int nPlayerId, jbyte* pData, int nSize)
{
	const int nNo = GetConnectNoFromPlayerId(nPlayerId);
	sendData(m_aConnectInfo[nNo].Id.c_str(), pData, nSize);
}

void TransferManager::BroadCast(jbyte *pData, int nSize)
{
	for(const auto& it : m_aConnectInfo){
		sendData(it.Id.c_str(), pData, nSize);
	}
}

void TransferManager::AddReceiveData(const char* id, const jbyte* pData, int nSize)
{
	uint8_t uKind = *pData;
	if(uKind >= 0 && uKind < eTransferKind_Max){
		m_aTransfer[uKind]->AddReceive(id, pData, nSize);
	}
}

bool TransferManager::IsSelfConnectId(const char* id) const
{
	return (std::strcmp(m_SelfInfo.Id.c_str(), id) == 0);
}

int TransferManager::GetConnectNoFromNetId(const char* id) const
{
	int i = 0;
	for(const auto& info : m_aConnectInfo){
		if(std::strcmp(info.Id.c_str(), id) == 0) {
			return i;
		}
		i++;
	}
	return -1;
}

int TransferManager::GetConnectNoFromPlayerId(int nPlayerId) const
{
	int i = 0;
	for(const auto& info : m_aConnectInfo) {
		if(info.nPlayerId == nPlayerId){
			return i;
		}
		i++;
	}
	return -1;
}

int TransferManager::GetPlayerIdFromNetId(const char* id) const
{
	if(IsSelfConnectId(id)){
		return m_SelfInfo.nPlayerId;
	}
	else {
		int nNo = GetConnectNoFromNetId(id);
		if(nNo >= 0) {
			return m_aConnectInfo[nNo].nPlayerId;
		}
	}
	return -1;
}

uint8_t TransferManager::GetCharaIdFromPlayerId(int nPlayerId) const
{
	const int nNo = GetConnectNoFromPlayerId(nPlayerId);
	return (nNo >= 0) ? m_aConnectInfo[nNo].uCharaId : m_SelfInfo.uCharaId;
}

void TransferManager::sendData(const char* id, jbyte* pData, int nSize)
{
	jbyteArray byteData = GetEnv()->NewByteArray(nSize);
	GetEnv()->SetByteArrayRegion(byteData, 0, nSize, pData);

	jclass classID = ObjectLoader::GetObjectLoader()->GetClassID(CLASS_NAME_SYSTEM);
	jmethodID  methodID_1 = ObjectLoader::GetObjectLoader()->GetMethodID(CLASS_NAME_SYSTEM, "SendData");
	jstring Id = GetEnv()->NewStringUTF(id);
	GetEnv()->CallStaticVoidMethod(classID, methodID_1, Id, byteData);
	GetEnv()->DeleteLocalRef(byteData);
	GetEnv()->DeleteLocalRef(Id);
}