//
// Created by 葵ユメ on 2020/12/06.
//


#ifndef AOIYUME_APP_PARAM_H
#define AOIYUME_APP_PARAM_H

#include "../Engine/Engine.h"
#include <Singleton.h>
#include <AppSkillList.h>

class AppParam : public Singleton<AppParam>
{
	USE_SINGLETON(AppParam)
public:
	enum {
		eItemKind_Max = 10
	};

	struct UseSkillInfo {
		bool bEnable;
		int Duration;
		int Param;
		int SendPlayer;
		int TargetPlayer;
	};

	struct GameNetworkInfo {
		bool bClear;
		int8_t  nCurrentPlayerId;
		int8_t  nCurrentTurn;
		struct CharaInfo {
			int 	nKizunaPoint;
			uint8_t uItemNum[eItemKind_Max];
			UseSkillInfo useSkill[AppSkillList::eSkillList_Max];
		};
		CharaInfo	CharaInfo[NET_CONNECT_MAX];
	};

	struct CharacterInfo {
		char cCharaName[32];
		int 	nKizunaPoint;
		uint8_t uItemNum[eItemKind_Max];
		UseSkillInfo useSkill[AppSkillList::eSkillList_Max];
	};

	struct SkillNetworkInfo {
		bool Standby;
		int	SkillNo;
		int Duration;
		int Param;
		int SendPlayer;
		int TargetPlayer;
	};

public:
	void SetCharaName(const char* pName);
	const char* GetCharaName() const;
	const char* GetCharaName(int nPlayerId) const;

	void AddKizunaPoint(int nPlayerId, int nAdd);
	void SubKizunaPoint(int nPlayerId, int nSub);
	int GetKizunaPoint(int nPlayerId) const;

	void AddItem(int nPlayerId, int nItemNo, int nNum);
	void SubItem(int nPlayerId, int nItemNo, int nNum);
	int GetItemNum(int nPlayerId, int nItemNo) const;

	void SetClear(bool bClear);
	bool IsClear() const;

	void SetUseSkillInfo(int nNo, int nDuration, int nParam, int nSendPlayer, int nTargetPlayer);
	const UseSkillInfo* GetUseSkillInfo(int nPlayerId) const;
	void UpdateSkillDuration(int nNo, int nPlayerId);
	void DumpUseSkillInfo(int nPlayerId);

	GameNetworkInfo& GetNetworkInfo();
	void DumpNetworkInfo();

	SkillNetworkInfo& GetSkillInfo();
	void DumpNetworkSkillInfo();
private:
	CharacterInfo 		m_CharaInfo;
	GameNetworkInfo		m_NetworkGameInfo;
	SkillNetworkInfo	m_NetworkSkillInfo;
};

#endif