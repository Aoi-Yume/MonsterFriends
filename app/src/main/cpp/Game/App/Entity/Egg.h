//
// Created by 葵ユメ on 2021/05/04.
//


#ifndef AOIYUME_APP_EGG_H
#define AOIYUME_APP_EGG_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class Egg : public GameEntity
{
	typedef GameEntity Super;
	
public:
	enum {
		eEGG_BORNE_KIND_MAX = 5
	};

	enum {
		eSTATE_WAIT,
		eSTATE_BORNE,
		eSTATE_BORNE_END,
	};

public:
	Egg();
	virtual ~Egg();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	void StartBorne();
	bool IsBorneEnd() const { return GetState() == eSTATE_BORNE_END; }

private:
	void actWaitState();
	void actBorneState();

private:
	int m_nCurrentImageNo;
};

#endif