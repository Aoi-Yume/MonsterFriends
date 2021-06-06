//
// Created by 葵ユメ on 2018/12/19.
//


#ifndef AOIYUME_TRANSFER_MANAGER_H
#define AOIYUME_TRANSFER_MANAGER_H

#include "../Engine/Engine.h"
#include "Singleton.h"
#include <vector>

class TransferBase;

class TransferManager : public Singleton<TransferManager>
{
	USE_SINGLETON(TransferManager)
public:
	enum TransferKind : uint8_t {
		eTransferKind_Connect,
		eTransferKind_PlayerId,
		eTransferKind_SyncScene,
		eTransferKind_Command,
		eTransferKind_TouchInfo,
		eTransferKind_GameInfo,
		eTransferKind_SkillInfo,
		eTransferKind_Max
	};

	struct ConnectInfo {
		bool bHost = false;
		int nPlayerId = -1;
		uint8_t  uCharaId = 0;
		std::string Id = "";
		std::string	Name = "";

		ConnectInfo()
		: bHost(false)
		, nPlayerId(-1)
		, uCharaId(0)
		, Id("")
		, Name("")
		{}

		ConnectInfo(const char* pId, const char* pName, uint8_t uCharaId)
		: bHost(false)
		, nPlayerId(-1)
		, uCharaId(uCharaId)
		, Id(pId)
		, Name(pName)
		{}
	};
	
public:
	void Initialize(bool bHost);

	void SetConnectSuccess(bool bConnect);
	bool IsConnectSucess() const;
	void ResetConnect();

	void SetTransfer(int nKind, TransferBase* pTransfer);
	void StartTransfer(int nKind);
	void StopTransfer(int nKind);
	void StopAllTransfer();
	bool IsStartTransfer(int nKind) const;
	
	template<class T>
	T* GetTransfer(int nKind) {
		ASSERT(nKind >= 0 && nKind < eTransferKind_Max);
		ASSERT(nKind < m_aTransfer.size());
		return reinterpret_cast<T*>(m_aTransfer[nKind]);
	}

	bool IsHost() const;

	void SetSelfConnect(const ConnectInfo& info);
	const ConnectInfo& GetSelfConnect() const;
	void SetSelfCharaId(uint8_t uCharaId);
	uint8_t GetSelfCharaId() const;

	void AddConnect(const ConnectInfo& info);
	const ConnectInfo& GetConnect(int nNo) const;
	const ConnectInfo& GetHostConnect() const;
	int GetConnectNum() const;

	void SetConnectHost(const char* id, bool bHost);
	void SetConnectCharaId(const char* id, uint8_t uCharaId);
	void SetConnectPlayerId(const char* id, int nPlayerId);

	void SendHost(jbyte* pData, int nSize);
	void SendPlayer(int nPlayerId, jbyte* pData, int nSize);
	void BroadCast(jbyte* pData, int nSize);
	void ReceiveData(const char* id, jbyte* pData, int nSize);

	bool IsSelfConnectId(const char* id) const;
	int GetConnectNoFromNetId(const char* id) const;
	int GetConnectNoFromPlayerId(int nPlayerId) const;
	int GetPlayerIdFromNetId(const char* id) const;
	uint8_t GetCharaIdFromPlayerId(int nPlayerId) const;

private:
	void sendData(const char* id, jbyte* pData, int nSize);

private:
	bool m_bHost;
	bool m_bConnectSucess;
	ConnectInfo	m_SelfInfo;

	std::vector<TransferBase*>	m_aTransfer;
	std::vector<ConnectInfo>	m_aConnectInfo;
};

#endif