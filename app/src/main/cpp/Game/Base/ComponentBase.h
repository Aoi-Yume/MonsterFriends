//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_COMPONENT_BASE_H
#define AOIYUME_COMPONENT_BASE_H

#include "../Engine/Engine.h"
#include <BaseDefine.h>

class EntityBase;

class ComponentBase
{
public:
	ComponentBase(ComponentKind nKind, EntityBase* pEntityBase);
	virtual ~ComponentBase();

    virtual GameMessageResult Update(GameMessage message, const void* param);

    EntityBase* GetEntityBase();
private:
	ComponentKind   m_nKind;
    EntityBase*     m_pEntityBase;
};

#endif