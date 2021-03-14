//
// Created by 葵ユメ on 2021/03/05.
//


#ifndef AOIYUME_SCENE_RESULT_H
#define AOIYUME_SCENE_RESULT_H

#include <Entity.h>
#include <Button/ButtonManager.h>
#include "../Engine/Engine.h"
#include "entity_define.h"
#include "SceneBase.h"
#include <State.h>

class StateManager;
class ComponentBase;
class Character;
class MessageWindow;
class PlayerNotice;

//==========================================
//==========================================
class SceneResult : public SceneBase
{
	typedef SceneBase Super;

public:
	enum {
		eState_FadeIn,
		eState_ResultMain,
		eState_FadeOut,
		eState_Max
	};

public:
	static SceneBase* CreateScene();

public:
	SceneResult();
	virtual ~SceneResult();
	
protected:
	virtual void SceneSetup() override ;
	virtual void SceneSync() override;
	virtual void SceneUpdate() override ;
	virtual void SceneFinalize() override ;

	void EntityUpdate(GameMessage message, const void* param) override;

public:
	Entity*		m_pBgImage;
	struct CharaInfo {
		int nCristalNum;
		Character *pChara;
	};
	CharaInfo	m_CharaInfo[NET_CONNECT_MAX];
	PlayerNotice*	m_pPlayerNotice;

	StateManager*	m_pStateManager;
	std::vector<ButtonManager*>	m_aBtnManager;
};

#endif