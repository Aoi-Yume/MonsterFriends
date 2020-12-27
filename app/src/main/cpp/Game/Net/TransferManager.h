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
		eTransferKind_Max
	};

	struct ConnectInfo {
		std::string Id = "";
		std::string	Name = "";

		ConnectInfo()
		: Id("")
		, Name("")
		{}

		ConnectInfo(const char* pId, const char* pName)
		: Id(pId)
		, Name(pName)
		{}
	};
	
public:
	
	void SetTransfer(int nKind, TransferBase* pTransfer);
	void StartTransfer(int nKind);
	void StopTransfer(int nKind);
	
	template<class T>
	T* GetTransfer(int nKind) {
		assert(nKind >= 0 && nKind < eTransferKind_Max);
		return reinterpret_cast<T*>(m_aTransfer[nKind]);
	}

	void AddConnect(const ConnectInfo& info);
	const ConnectInfo& GetConnect(int nNo) const;
	int GetConnectNum() const;

	void BroadCast(jbyte* pData, int nSize);
	void ReceiveData(const char* id, jbyte* pData, int nSize);

private:

	std::vector<TransferBase*>	m_aTransfer;
	std::vector<ConnectInfo>	m_aConnectInfo;
};

#endif