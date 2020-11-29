//
// Created by 葵ユメ on 2018/10/08.
//


#ifndef AOIYUME_SCENE_GAME_ADV_H
#define AOIYUME_SCENE_GAME_ADV_H

#include <Entity.h>
#include <Button/ButtonManager.h>
#include "../Engine/Engine.h"
#include "entity_define.h"
#include "SceneBase.h"

class ComponentBase;
class Character;
class InformationPlate;
class MessageWindow;
class Dice;

class SceneAdv : public SceneBase
{
	typedef SceneBase Super;
public:
	static SceneBase* CreateScene();

public:
	enum {
		eSTEP_APPEAR_ENEMY,
		eSTEP_BATTLE,
	};

public:
	SceneAdv();
	virtual ~SceneAdv();

protected:
	virtual void SceneSetup() override ;
	virtual void SceneUpdate() override ;
	virtual void SceneFinalize() override ;
	void EntityUpdate(GameMessage message, const void* param) override;

private:
	void updateAppearEnemyStep();
	void updateBattleStep();

private:
	struct CharaInfo{
		Character* 	pChara = nullptr;
		Dice*		pDice = nullptr;

		~CharaInfo(){
			delete pChara;
			delete pDice;
		}
	};

	int 		m_nStep;
	int 		m_nNextStep;
	int 		m_nStepCnt;

	int 		m_nSubStep;
	int 		m_nSubStepCnt;

	Entity*		m_pBgImage;
	CharaInfo	m_Chara;
	CharaInfo	m_Enemy;

	ButtonManager*	m_pBtnManager;
	InformationPlate* m_pInformationPlate;
	MessageWindow* m_pMessageWindow;
};

#endif