//
// Created by 葵ユメ on 2021/10/09.
//


#ifndef AOIYUME_APP_SHADOW_LIST_H
#define AOIYUME_APP_SHADOW_LIST_H

#include "../Engine/Engine.h"
#include <Singleton.h>
#include <vector>

class AppShadowList : public Singleton<AppShadowList>
{
	USE_SINGLETON(AppShadowList)
public:
	struct ShadowInfo {
		std::string name;
		std::string fileName;
		float offsetX;
		float offsetY;
	};
	
public:
	void Load();
	const ShadowInfo& GetShadowInfo(int nIdx) const;
	const ShadowInfo& GetShadowInfo(const char* pName) const;
	const int GetShadowInfoIdx(const char* pName) const;
	int GetShadowListSize() const { return m_aShadowInfo.size(); }

private:
	std::vector<ShadowInfo>	m_aShadowInfo;
};

#endif