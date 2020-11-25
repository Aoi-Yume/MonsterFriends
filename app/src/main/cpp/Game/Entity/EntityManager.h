//
// Created by 葵ユメ on 2018/09/05.
//


#ifndef AOIYUME_ENTITY_MANAGER_H
#define AOIYUME_ENTITY_MANAGER_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "EntityBase.h"
#include "vector"

class ComponentBase;

class EntityManager
{
public:
	EntityManager();
	virtual ~EntityManager();

	EntityBase* GetEntity(uint idx);
	void Update(GameMessage message, const void* param);
	
	void AttachEntity(EntityBase* pEntity);

private:
	void checkCollisin(EntityBase* pEntityBaseA, EntityBase* pEntityBaseB);

private:
	std::vector<EntityBase*>	m_aEntity;
};

#endif