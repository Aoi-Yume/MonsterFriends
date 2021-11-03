//
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
	void ChangeItemNo(int nIdx, int nNo);
	void ClearItemNo();
	int GetSelectItemNo() const;
	int GetCurrentItemIdx() const;
	int GetRegisterItemNum() const { return m_aItemNo.size(); }

private:
	void updateItemButton();
	void updateItemIcon();
	void updateInvisibleItemIcon(bool bMoveRight);

private:
	int m_nCurrentItemIdx;
	int m_nSelectItemNo;
	std::vector<int>	m_aItemNo;
	std::vector<ButtonManager*> m_aButtonManager;
};

#endif