//
// Created by 葵ユメ on 2021/10/09.
//


#ifndef AOIYUME_APP_SHADOW_H
#define AOIYUME_APP_SHADOW_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"
#include <AppShadowList.h>

class Shadow : public GameEntity
{
	typedef GameEntity Super;
public:
	Shadow(const char* pShadowName, Entity* pEntity);
	virtual ~Shadow();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	void ChangeReferenceEntity(Entity* pEntity) { m_pRefEntity = pEntity; }
	void SetAerial(bool bAerial);

private:

private:
	bool 		m_bAerial;
	float 		m_fGroundY;
	Entity*		m_pRefEntity;
	AppShadowList::ShadowInfo m_ShadowInfo;
};

#endif