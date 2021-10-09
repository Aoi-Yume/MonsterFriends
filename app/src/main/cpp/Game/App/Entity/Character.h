//
// Created by 葵ユメ on 2020/11/29.
//


#ifndef AOIYUME_APP_CHARACTER_H
#define AOIYUME_APP_CHARACTER_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class Dice;
class Shadow;
class Character : public GameEntity
{
	typedef GameEntity Super;
public:
	enum State{
		eState_None,
		eState_Wait,
		eState_Attention,
		eState_ComBattleStandby,
		eState_StopDice,
		eState_CheckEndDice,
		eState_EndDice,
	};
	enum {
		eDice_Max = 3,
	};

public:
	Character(int charaId);
	virtual ~Character();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	int GetCharaId() const{ return m_nCharaId; }
	void SetCom(bool bCom){ m_bCom = bCom; }
	bool IsCom() const { return m_bCom; }

	void SetUseDice(int nUseDice) { m_nUseDice = CLAMP(nUseDice, 1, (int)eDice_Max); }
	void ResetStopDice();

	void BeginAttention();

	void BeginDice();
	void StopDice();
	int GetDiceSumVal() const;

	void BeginComDice();
	void InVisibleDice();

	bool IsStopDiceState() const { return GetState() == eState_StopDice || GetState() == eState_CheckEndDice; }
	bool IsEndDice() const { return GetState() == eState_EndDice; }

private:
	void actAttention();
	void actComBattleStandbyState();
	void actStopDiceState();
	void actCheckEndDiceState();

	void jump(int nFrame, int nFrameMax, float fJumpPow);

private:
	bool 	m_bCom;
	int 	m_nCharaId;
	int 	m_nUseDice;
	int 	m_nStopDice;
	float	m_fJumpStartY;

	Shadow* m_pShadow;
};

#endif