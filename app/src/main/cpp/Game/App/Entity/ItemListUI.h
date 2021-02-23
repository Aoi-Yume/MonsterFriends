﻿//
// Created by 葵ユメ on 2021/02/21.
//


#ifndef AOIYUME_APP_ITEM_LIST_UI_H
#define AOIYUME_APP_ITEM_LIST_UI_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class ButtonManager;
class ItemListUI : public GameEntity
{
	typedef GameEntity Super;
public:
	enum {
		eButtonManager_ItemList,
		eButtonManager_LeftRight,
	};

	enum {
		eButtonVisibleNum = 3
	};

public:
	ItemListUI();
	virtual ~ItemListUI();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	void Open();
	void Close();
	void Lock();
	void UnLock();
	void Reset();

	void AddItemNo(int nNo);
	void ClearItemNo();
	int GetSelectItemNo() const;

private:
	void updateItemButton();

private:
	int m_nCurrentItemNo;
	int m_nSelectItemNo;
	std::vector<int>	m_aItemNo;
	std::vector<ButtonManager*> m_aButtonManager;
};

#endif