//
// Created by 葵ユメ on 2020/12/06.
//


#ifndef AOIYUME_APP_PARAM_H
#define AOIYUME_APP_PARAM_H

#include "../Engine/Engine.h"
#include <Singleton.h>

class AppParam : public Singleton<AppParam>
{
	USE_SINGLETON(AppParam)
public:
	struct GameNetworkInfo {
		int8_t  nCurrentPlayerId;
		int8_t  nCurrentTurn;
		struct CharaInfo {
			int nKizunaPoint;
		};
		CharaInfo	ChharaInfo[NET_CONNECT_MAX];
	};

	struct CharacterInfo {
		char cCharaName[32];
		int nKizunaPoint;
	};

public:
	void SetCharaName(const char* pName);
	const char* GetCharaName() const;

	void AddKizunaPoint(int nAdd);
	int GetKizunaPoint() const;

	GameNetworkInfo& GetNetworkInfo();
	void DumpNetworkInfo();

private:
	CharacterInfo 		m_CharaInfo;
	GameNetworkInfo		m_NetworkGameInfo;
};

#endif