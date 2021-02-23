//
// Created by 葵ユメ on 2021/02/23.
//


#ifndef AOIYUME_APP_SKILL_LIST_H
#define AOIYUME_APP_SKILL_LIST_H

#include "../Engine/Engine.h"
#include <Singleton.h>
#include <vector>

class AppSkillList : public Singleton<AppSkillList>
{
	USE_SINGLETON(AppSkillList)
public:
	enum {
		eSkillList_Max = 10
	};

	struct SkillInfo {
		std::string name;
		std::string Type;
		int Duration;
		int minParam;
		int maxParam;
	};

	struct UpdateSkillInfo{
		std::string Type;
		int nSendPlayer;
		int nTargetPlayer;
		int Duration;
		int nParam;
	};

public:
	void Load();

	void InitializeSkillTransfer();

	const SkillInfo& GetSkillInfo(int nIdx) const;
	int GetSkillNoFromSkillName(const char* pSkillName);

	void BeginItemSkill(int nPlayer, int nSkillNo);
	bool IsEndItemSkill() const;

	void UpdateSkill(const UpdateSkillInfo& info);

private:
	void sendNetSkillInfo(int nSkillNo, const UpdateSkillInfo& info);

private:
	std::vector<SkillInfo>	m_aSkillInfo;
};

#define SKILL_LIST() AppSkillList::Get()

#endif