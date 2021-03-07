//
// Created by 葵ユメ on 2021/01/11.
//


#ifndef AOIYUME_APP_ITEM_LIST_H
#define AOIYUME_APP_ITEM_LIST_H

#include "../Engine/Engine.h"
#include <Singleton.h>
#include <vector>

class AppItemList : public Singleton<AppItemList>
{
	USE_SINGLETON(AppItemList)
public:
	enum {
		eItemList_Max = 10
	};

	struct ItemInfo {
		std::string name;
		std::string useType;
		std::string fileName;
		int nCost;
		std::string explain;
		std::string skillName;
	};
	
public:
	void Load();
	const ItemInfo& GetItemInfo(int nIdx) const;
	int GetItemNoFromName(const char* pName) const;

	bool IsUsePossible(int nIdx, int nPlayerId) const;

private:
	std::vector<ItemInfo>	m_aItemInfo;
};

#endif