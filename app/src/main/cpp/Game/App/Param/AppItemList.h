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
	struct ItemInfo {
		std::string name;
		std::string useType;
		std::string fileName;
		int nCost;
		std::string explain;
		std::string skillName;
		int nAppearVal;
		bool bBuyUse;
	};
	
public:
	void Load();
	const ItemInfo& GetItemInfo(int nIdx) const;
	int GetItemNoFromName(const char* pName) const;
	int GetItemListSize() const { return m_aItemInfo.size(); }

	bool IsUsePossible(int nIdx, int nPlayerId) const;

private:
	std::vector<ItemInfo>	m_aItemInfo;
};

#endif