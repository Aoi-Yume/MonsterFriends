//
// Created by 葵ユメ on 2021/05/30.
//


#ifndef AOIYUME_APP_BACKGROUND_H
#define AOIYUME_APP_BACKGROUND_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class BackGround : public GameEntity
{
	typedef GameEntity Super;
	
public:
	BackGround();
	virtual ~BackGround();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:

private:
};

#endif