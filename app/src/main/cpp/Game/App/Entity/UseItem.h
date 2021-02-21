//
// Created by 葵ユメ on 2021/02/21.
//


#ifndef AOIYUME_APP_USE_ITEM_H
#define AOIYUME_APP_USE_ITEM_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class ButtonManager;
class MessageWindow;
class InformationPlate;
class ItemListUI;

class UseItem : public GameEntity
{
	typedef GameEntity Super;
public:
	enum {
		eStep_SelectItemWait,
		eStep_ShowDetail,
		eStep_MessageWait,
		eStep_ShowExplain,
		eStep_MessageWait2,
		eStep_UseOrCancel,
		eStep_Use,
		eStep_UseWait,
		eStep_Reset,
		eStep_End
	};

	enum {
		eBtnManager_UseOrCancel,
		eBtnManager_Back,
	};

	enum {
		eBtn_Use,
		eBtn_UseCancel,

		eBtn_Back = 0,
	};

public:
	UseItem();
	virtual ~UseItem();

	void Open();
	void Close();
	bool IsEnd() const;

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

	void SetInformationPlate(InformationPlate* pPlate);
	void SetVisible(bool bVisible);

private:
	void updateItemList();

private:

	int 				m_nStep;
	MessageWindow* 		m_pMessageWindow;
	InformationPlate*	m_pInformationPlate;
	ItemListUI*			m_pItemListUI;
	std::vector<ButtonManager*>		m_aButtonManager;
};

#endif