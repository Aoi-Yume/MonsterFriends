//
// Created by 葵ユメ on 2021/01/10.
//


#ifndef AOIYUME_APP_SHOP_H
#define AOIYUME_APP_SHOP_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class ButtonManager;
class MessageWindow;
class InformationPlate;
class ItemListUI;

class Shop : public GameEntity
{
	typedef GameEntity Super;
public:

public:
	Shop();
	virtual ~Shop();

	void Open();
	void Close();
	bool IsEnd() const;
	void SetInformationPlate(InformationPlate* pPlate);
	void SetVisible(bool bVisible);

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

private:
	void setNewItem(int nIdx);

private:
	enum {
		eStep_SelectItemWait,
		eStep_ShowDetail,
		eStep_MessageWait,
		eStep_ShowExplain,
		eStep_MessageWait2,
		eStep_BuyOrCancel,
		eStep_BuyCheck,
		eStep_BuyAfterMessageWait,
		eStep_BuyUseCheck,
		eStep_BuyUseWait,
		eStep_UseMessage,
		eStep_GetItemMessage,
		eStep_MessageWait3,
		eStep_ClearCheck,
		eStep_ClearMessage,
		eStep_ClearMessageWait,
		eStep_Reset,
		eStep_BackMessage,
		eStep_BackMessage_Wait,
		eStep_End,
	};
	enum {
		eBtnManager_BuyOrCancel,
		eBtnManager_Back,
	};
	enum {
		eBtn_Buy = 0,
		eBtn_BuyCancel,

		eBtn_Back = 0,
	};

	bool 				m_bBuy;
	int 				m_nStep;
	MessageWindow* 		m_pMessageWindow;
	InformationPlate*	m_pInformationPlate;
	ItemListUI*			m_pItemListUI;
	std::vector<ButtonManager*>		m_aButtonManager;
};

#endif