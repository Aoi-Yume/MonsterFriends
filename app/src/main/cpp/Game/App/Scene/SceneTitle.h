//
// Created by 葵ユメ on 2018/10/08.
//


#ifndef AOIYUME_SCENE_GAME_TITLE_H
#define AOIYUME_SCENE_GAME_TITLE_H

#include <Entity.h>
#include <Button/ButtonManager.h>
#include "../Engine/Engine.h"
#include "entity_define.h"
#include "SceneBase.h"

class ComponentBase;
class StateManager;
class BackGround;
class Egg;
class Character;
class MessageWindow;
class PermissionExplain;
class OptionMenu;

class SceneTitle : public SceneBase
{
	typedef SceneBase Super;

public:
	enum {
		eState_FadeIn,
		eState_WaitPressButton,
		eState_ExplainPermission,
		eState_CheckPermission,
		eState_BorneStartMonster,
		eState_BorneFadeOut,
		eState_BorneFadeIn,
		eState_BorneMessage,
		eState_CharaAttention,
		eState_WaitKeyboardEnable,
		eState_WaitInput,
		eState_StartNearby,
		eState_WaitNearbyConnect,
		eState_WaitNearbyTransferPlayerId,
		eState_Option,
		eState_ShowLicense,
		eState_FadeOut,
		eState_Max
	};

	enum {
		eBTN_MANAGER_TITLE,
		eBTN_MANAGER_ROOM,
	};

	enum {
		eBTN_SINGLE = 0,
		eBTN_MULTI,
		eBTN_TUTORIAL,

		eBTN_MAKE_ROOM = 0,
		eBTN_JOIN_ROOM,
	};

public:
	static SceneBase* CreateScene();
public:
	SceneTitle();
	virtual ~SceneTitle();
	
protected:
	virtual void SceneSetup() override ;
	virtual void SceneUpdate() override ;
	virtual void SceneFinalize() override ;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	int 		m_nCurrentCharaIdx;
	BackGround*	m_pBg;
	Egg*		m_pEgg;
	Entity*		m_pTitleImage;
	std::vector<Character*>	m_aChara;
	std::vector<ButtonManager*>	m_aButtonManager;
	MessageWindow*	m_pMessageWindow;
	PermissionExplain* m_pPermissionExplain;
	OptionMenu* m_pOptionMenu;

	StateManager*	m_pStateManager;
};

#endif