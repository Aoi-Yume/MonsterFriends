//
// Created by 葵ユメ on 2018/08/25.
//


#include "ComponentBase.h"
#include "EntityBase.h"

ComponentBase::ComponentBase(ComponentKind nKind, EntityBase* pEntityBase)
: m_nKind(nKind)
, m_pEntityBase(pEntityBase)
{
}

ComponentBase::~ComponentBase()
{
}

GameMessageResult ComponentBase::Update(GameMessage message, const void* param)
{
    return eGameMessageResult_Break;
}


EntityBase* ComponentBase::GetEntityBase()
{
    return m_pEntityBase;
}