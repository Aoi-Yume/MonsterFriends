//
// Created by 葵ユメ on 2021/09/11.
//


#ifndef AOIYUME_APP_OPTION_MENU_H
#define AOIYUME_APP_OPTION_MENU_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class ButtonManager;

class OptionMenu : public GameEntity
{
	typedef GameEntity Super;

public:
	enum {
		eBtn_Tutorial,
		eBtn_License,
		eBtn_Cancel,
	};

public:
	OptionMenu();
	virtual ~OptionMenu();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	void Open();
	void Close();

	int GetResult() const;

private:
};

#endif