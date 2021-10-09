//
// Created by 葵ユメ on 2021/03/27.
//


#ifndef AOIYUME_APP_CHARA_LIST_H
#define AOIYUME_APP_CHARA_LIST_H

#include "../Engine/Engine.h"
#include <Singleton.h>
#include <vector>

class AppCharaList : public Singleton<AppCharaList>
{
	USE_SINGLETON(AppCharaList)
public:
	struct CharaInfo {
		std::string name;
		std::string fileName;
		std::string shadowName;
		int nDice;
		float fDiceOffsetY;
		int nGetPoint;
	};
	
public:
	void Load();
	const CharaInfo& GetCharaInfo(int nIdx) const;
	int GetCharaListSize() const { return m_aCharaInfo.size(); }

private:
	std::vector<CharaInfo>	m_aCharaInfo;
};

#endif