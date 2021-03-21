//
// Created by 葵ユメ on 2021/02/23.
//


#ifndef AOIYUME_APP_SKILL_LIST_H
#define AOIYUME_APP_SKILL_LIST_H

#include "../Engine/Engine.h"
#include <Singleton.h>
#include <array>

class AppSkillList : public Singleton<AppSkillList>
{
	USE_SINGLETON(AppSkillList)
public:
	enum {
		eSkillList_Max = 10
	};

	enum SkillTiming{
		eSKillTiming_None,
		eSkillTiming_Now,
		eSkillTiming_SelfTurn,
	};

	struct SkillInfo {
		std::string name;
		std::string Type;
		std::string Timing;
		int Duration;
		int minParam;
		int maxParam;
	};

public:
	void Load();

	void InitializeSkillTransfer();

	const SkillInfo& GetSkillInfo(int nIdx) const;
	int GetSkillNoFromSkillName(const char* pSkillName);

	void BeginItemSkill(int nPlayer, int nSkillNo);
	bool IsEndItemSkill() const;

	void UpdateSkill(int nPlayer, SkillTiming timing);

private:
	void sendNetSkillInfo(int nSkillNo, int nDuration, int nParam, int nSendPlayer, int nTargetPlayer);

	int getSkillParam(int nSkillNo, int nPlayer) const;
	int getSkillTiming(const char* str) const;

private:
	std::array<SkillInfo, eSkillList_Max>	m_aSkillInfo;
};

#define SKILL_LIST() AppSkillList::Get()

#endif