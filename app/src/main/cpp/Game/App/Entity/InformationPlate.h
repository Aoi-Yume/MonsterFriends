//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_APP_INFORMATION_PLATE_H
#define AOIYUME_APP_INFORMATION_PLATE_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class CristalIcon : public GameEntity
{
	typedef GameEntity Super;

public:
	enum {
		eChild_Gray,
		eChild_Color,
	};

public:
	CristalIcon();
	virtual ~CristalIcon();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	void Setup();
	void Change(bool bGray);
};

class InformationPlate : public GameEntity
{
	typedef GameEntity Super;
public:
	enum {
		eChild_Point,
		eChild_PointPlus,
		eChild_BattlePlus,
		eChild_Cristal,
	};

public:
	InformationPlate();
	virtual ~InformationPlate();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

	void UpdatePlate();
private:
	void updateKizunaRemain(int nCurrentPlayerId);
	void updateKizunaPlusPoint(int nCurrentPlayerId);
	void updateBattlePlusPoint(int nCurrentPlayerId);
	void updateCristalNum(int nCurrentPlayerId);

	int getKizunaPlusPoint(int nCurrentPlayerId) const;
	int getBattlePlusPoint(int nCurrentPlayerId) const;

private:
	int m_nCurrentCristal;
};

#endif