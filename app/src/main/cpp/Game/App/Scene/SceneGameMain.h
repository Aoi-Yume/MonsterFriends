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
class BackGround;
class Character;
class InformationPlate;
class MessageWindow;
class Adv;
class Shop;
class UseItem;
class PlayerNotice;

//==========================================
//==========================================
class SceneGameMain : public SceneBase
{
	typedef SceneBase Super;

public:
	enum {
		eBtnKind_Main,
		eBtnKind_ItemOrShop,
	};

	enum {
		eState_FadeIn,
		eState_GameMain,
		eState_Adv,
		eState_SelectItemUseOrShop,
		eState_Shop,
		eState_UseItem,
		eState_NextPlayer,
		eState_ClearFademOut,
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

public:
	Entity*		m_pCounter;
	BackGround*	m_pBg;
	Character*	m_pChara;
	Adv*		m_pAdv;
	Shop*		m_pShop;
	UseItem*	m_pUseItem;
	PlayerNotice*		m_pPlayerNotice;

	InformationPlate* m_pInformationPlate;
	MessageWindow* m_pMessageWindow;
	StateManager*	m_pStateManager;
	std::vector<ButtonManager*>	m_aBtnManager;
};

#endif