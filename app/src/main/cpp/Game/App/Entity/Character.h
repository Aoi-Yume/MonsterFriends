//
// Created by 葵ユメ on 2020/11/29.
//


#ifndef AOIYUME_APP_CHARACTER_H
#define AOIYUME_APP_CHARACTER_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class Character : public GameEntity
{
	typedef GameEntity Super;
public:
	enum CHARA_ID {
		eCHARA_NONE = -1,
		eCHARA_01,		// スライム
	};

public:
	Character(CHARA_ID charaId);
	virtual ~Character();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:

private:

private:
	CHARA_ID m_nCharaId;
};

#endif