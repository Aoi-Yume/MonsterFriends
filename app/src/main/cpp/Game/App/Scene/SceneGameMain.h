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

class ComponentBase;
class InformationPlate;
class MessageWindow;

class SceneGameMain : public SceneBase
{
	typedef SceneBase Super;
public:
	static SceneBase* CreateScene();
public:
	SceneGameMain();
	virtual ~SceneGameMain();
	
protected:
	virtual void SceneSetup() override ;
	virtual void SceneUpdate() override ;
	virtual void SceneFinalize() override ;
	void EntityUpdate(GameMessage message, const void* param) override;

private:
	enum {
		eSTEP_NetworkInfoUpdate,
		eSTEP_NetworkInfoUpdateWait,
		eSTEP_ControlPlayer,
		eSTEP_MAX
	};

	enum {
		eBtn_Adv,
		eBtn_SpendTime,
		eBtn_Work,
		eBtn_Max
	};

	int 		m_nStep;

	Entity*		m_pBgImage;
	Entity*		m_pChara;		// TODO キャラクラスに変更

	ButtonManager*	m_pBtnManager;
	InformationPlate* m_pInformationPlate;
	MessageWindow* m_pMessageWindow;
};

#endif