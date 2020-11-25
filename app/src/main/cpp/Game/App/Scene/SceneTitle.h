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

class SceneTitle : public SceneBase
{
	typedef SceneBase Super;
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
private:
	int 		m_nStep;
	int 		m_nNextStep;
	int 		m_nStepCnt;
	int 		m_nEggCnt;
	int 		m_nTranslateCnt;
	Entity*		m_pBgImage;
	Entity*		m_pEggImage;
	Entity*		m_pTitleImage;
	Entity*		m_pPlayerName;
	ButtonManager*	m_pButtonManager;
};

#endif