//
// Created by 葵ユメ on 2018/09/19.
//


#ifndef AOIYUME_SCENE_GAME_MAIN_H
#define AOIYUME_SCENE_GAME_MAIN_H

#include <Entity.h>
#include <Button/ButtonManager.h>
#include "../Engine/Engine.h"
#include "entity_define.h"
#include "SceneBase.h"
#include <State.h>

class StateManager;
class ComponentBase;
class Character;
class InformationPlate;
class MessageWindow;
class Adv;
class Shop;
class UseItem;

//==========================================
//==========================================
class StateGameMain : public StateBase
{
	enum {
		eBtn_Adv,
		eBtn_Work,
		eBtn_Item,
		eBtn_Max
	};

public:
	void Begin(void* pUserPtr) override;
	void Update(void* pUserPtr) override;
	void End(void* pUserPtr) override;
};

//==========================================
//==========================================
class StateAdv : public StateBase
{
public:
	void Begin(void* pUserPtr) override;
	void Update(void* pUserPtr) override;
	void End(void* pUserPtr) override;
};

//==========================================
//==========================================
class StateUseOrShopSelect : public StateBase
{
	enum {
		eBtn_Use,
		eBtn_Shop,
	};
public:
	void Begin(void* pUserPtr) override;
	void Update(void* pUserPtr) override;
	void End(void* pUserPtr) override;
};

//==========================================
//==========================================
class StateShop : public StateBase
{
	void Begin(void* pUserPtr) override;
	void Update(void* pUserPtr) override;
	void End(void* pUserPtr) override;
};

//==========================================
//==========================================
class StateUseItem : public StateBase
{
	void Begin(void* pUserPtr) override;
	void Update(void* pUserPtr) override;
	void End(void* pUserPtr) override;
};

//==========================================
//==========================================
class StateNextPlayer : public StateBase
{
	void Begin(void* pUserPtr) override;
};

//==========================================
//==========================================
class SceneGameMain : public SceneBase
{
	typedef SceneBase Super;
	friend class StateGameMain;
	friend class StateUseOrShopSelect;
	friend class StateAdv;
	friend class StateShop;
	friend class StateUseItem;
	friend class StateNextPlayer;

public:
	enum {
		eBtnKind_Main,
		eBtnKind_ItemOrShop,
	};

	enum {
		eState_GameMain,
		eState_Adv,
		eState_SelectItemUseOrShop,
		eState_Shop,
		eState_UseItem,
		eState_NextPlayer,
		eState_Max
	};

public:
	static SceneBase* CreateScene();

public:
	SceneGameMain();
	virtual ~SceneGameMain();
	
protected:
	virtual void SceneSetup() override ;
	virtual void SceneSync() override;
	virtual void SceneUpdate() override ;
	virtual void SceneFinalize() override ;

	void EntityUpdate(GameMessage message, const void* param) override;

private:
	Entity*		m_pCounter;
	Entity*		m_pBgImage;
	Character*	m_pChara;
	Adv*		m_pAdv;
	Shop*		m_pShop;
	UseItem*	m_pUseItem;

	InformationPlate* m_pInformationPlate;
	MessageWindow* m_pMessageWindow;
	StateManager*	m_pStateManager;
	std::vector<ButtonManager*>	m_aBtnManager;
};

#endif