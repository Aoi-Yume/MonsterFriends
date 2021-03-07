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

class SceneTitle : public SceneBase
{
	typedef SceneBase Super;

public:
	enum {
		eState_FadeIn,
		eState_WaitPressButton,
		eState_WaitKeyboardEnable,
		eState_WaitInput,
		eState_WaitNearbyConnect,
		eState_WaitNearbyTransferPlayerId,
		eState_FadeOut,
		eState_Max
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
	float 		m_fEggCnt;
	Entity*		m_pBgImage;
	Entity*		m_pEggImage;
	Entity*		m_pTitleImage;
	Entity*		m_pPlayerName;
	ButtonManager*	m_pButtonManager;

	StateManager*	m_pStateManager;
};

#endif