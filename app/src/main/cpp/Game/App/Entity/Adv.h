﻿//
// Created by 葵ユメ on 2018/10/08.
//


#ifndef AOIYUME_ADV_H
#define AOIYUME_ADV_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class ComponentBase;
class Character;
class InformationPlate;
class MessageWindow;
class Dice;
class ButtonManager;

class Adv : public GameEntity
{
	typedef GameEntity Super;

public:
	enum {
		eSTEP_APPEAR_ENEMY,
		eSTEP_BATTLE,
		eSTEP_END,
	};

public:
	Adv();
	virtual ~Adv();

	void Open();
	void Close();
	bool IsEnd() const;
	void SetCharacter(Character* pChara);
	void SetMessageWindow(MessageWindow* pMessageWindow);

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

private:
	void updateAppearEnemyStep();
	void updateBattleStep();

	int getWinSkillPoint() const;
	void updateGetKizuna(int nGetPoint);

private:

	int 		m_nStep;
	int 		m_nNextStep;
	int 		m_nStepCnt;

	int 		m_nSubStep;
	int 		m_nSubStepCnt;

	Character*	m_pChara;
	Character*	m_pEnemy;

	ButtonManager*	m_pBtnManager;
	MessageWindow* m_pMessageWindow;

	SoundStreamHandle m_diceLoopSE;
};

#endif