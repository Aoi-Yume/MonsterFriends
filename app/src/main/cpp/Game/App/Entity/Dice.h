//
// Created by 葵ユメ on 2020/11/23.
//


#ifndef AOIYUME_APP_DICE_H
#define AOIYUME_APP_DICE_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class Dice : public GameEntity
{
	typedef GameEntity Super;
public:
	enum {
		eDICE_MAX = 6
	};

public:
	Dice();
	virtual ~Dice();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	void SetVisible(int nIdx, bool bVisible);
	void InVisible();

	void StartDice();
	void StopDice();

	uint8_t GetDice() const;

private:
	void shuffleDicePattern();

private:
	bool m_bDiceStart;
	int m_nCurrentDiceIdx;
	int m_nCurrentDiceNo;
	int m_nDiceChangeInterval;
	uint8_t  m_uDiceRoulette[eDICE_MAX];
};

#endif