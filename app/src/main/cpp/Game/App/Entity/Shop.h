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

class Shop : public GameEntity
{
	typedef GameEntity Super;
public:

public:
	Shop();
	virtual ~Shop();

	void Open();
	void Close();
	void SetInformationPlate(InformationPlate* pPlate);
	void SetVisible(bool bVisible);

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

private:
	enum {
		eStep_SelectItemWait,
		eStep_ShowDetail,
		eStep_MessageWait,
		eStep_BuyOrCancel,
		eStep_BuyCheck,
		eSteo_BuyAfterMessageWait,
		eStep_Reset,
	};
	enum {
		eBtnManager_Item,
		eBtnManager_BuyOrCancel,
	};
	enum {
		eBtn_Item_01 = 0,
		eBtn_Item_02,
		eBtn_Item_03,
		eBtn_Item_Max,

		eBtn_Buy = 0,
		eBtn_BuyCancel,
	};

	int 				m_nStep;
	int 				m_nItemNo[eBtn_Item_Max];
	MessageWindow* 		m_pMessageWindow;
	InformationPlate*	m_pInformationPlate;
	std::vector<ButtonManager*>		m_aButtonManager;
};

#endif