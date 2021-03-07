//
// Created by 葵ユメ on 2018/09/17.
//


#ifndef AOIYUME_SCENE_LAUNCHER_H
#define AOIYUME_SCENE_LAUNCHER_H

#include <Entity.h>
#include <SceneBase.h>
#include "../Engine/Engine.h"
#include "entity_define.h"

class ComponentBase;
class SimpleButton;
class ButtonManager;

class SceneLauncher : public SceneBase
{
	typedef SceneBase Super;
public:
	SceneLauncher();
	virtual ~SceneLauncher();
	
protected:
	virtual void SceneSetup() override ;
	virtual void SceneUpdate() override ;
	virtual void SceneFinalize() override ;
	void EntityUpdate(GameMessage message, const void* param) override;
private:
	enum SceneButton{
		eButton_Title,
		eButton_GameMain,
		eButton_Result,
		eButton_Max
	};

	ButtonManager*	m_pSimpleBtnMgr;
};

#endif