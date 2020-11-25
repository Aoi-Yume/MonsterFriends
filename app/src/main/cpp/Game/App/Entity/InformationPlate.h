//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_APP_INFORMATION_PLATE_H
#define AOIYUME_APP_INFORMATION_PLATE_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class InformationPlate : public GameEntity
{
	typedef GameEntity Super;
public:

public:
	InformationPlate();
	virtual ~InformationPlate();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

private:
};

#endif