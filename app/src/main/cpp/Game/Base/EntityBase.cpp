//
// Created by 葵ユメ on 2018/08/25.
//


#include "EntityBase.h"
#include "ComponentBase.h"

EntityBase::EntityBase()
: m_pParentEntity(nullptr)
, m_bLinkTransform(false)
{
	DEBUG_LOG("Call EntityBase Constructor");
	for(int i = 0; i < eComponentKind_Max; ++i){
		m_pComponent[i] = nullptr;
	}
}

EntityBase::~EntityBase()
{
    DEBUG_LOG("Call EntityBase Deconstructor");
	for(int i = 0; i < eComponentKind_Max; ++i){
		delete m_pComponent[i];
	}
	const size_t size = m_svpChild.size();
	for(uint32_t i = 0; i < size; ++i) {
		delete m_svpChild.at(i);
	}
}

void EntityBase::Update(GameMessage message, const void* param)
{
	EntityUpdate(message, param);
	ComponentUpdate(message, param);

	const size_t size = m_svpChild.size();
	for(uint32_t i = 0; i < size; ++i){
		if(m_svpChild.at(i)) {
			m_svpChild.at(i)->Update(message, param);
		}
	}
}

ComponentBase* EntityBase::GetComponent(ComponentKind nKind)
{
    return m_pComponent[nKind];
}

void EntityBase::AddChild(EntityBase* pEntity, bool bLinkTransform)
{
	pEntity->SetParent(this, bLinkTransform);
	m_svpChild.push_back(pEntity);
}

void EntityBase::ResizeChild(unsigned long num)
{
	m_svpChild.resize(num, nullptr);
}

void EntityBase::SetChild(unsigned long idx, EntityBase* pEntity, bool bLinkTransform)
{
	assert(idx >= 0 && idx < m_svpChild.size());
	pEntity->SetParent(this, bLinkTransform);
	m_svpChild.at(idx) = pEntity;
}

EntityBase* EntityBase::GetChild(unsigned long idx)
{
	assert(idx >= 0 && idx < m_svpChild.size());
	return m_svpChild.at(idx);
}

size_t EntityBase::GetChildSize() const
{
	return m_svpChild.size();
}

void EntityBase::SetParent(EntityBase *pEntity, bool bLinkTransform)
{
	m_pParentEntity = pEntity;
	m_bLinkTransform = bLinkTransform;
}

EntityBase* EntityBase::GetParent()
{
	return m_pParentEntity;
}

bool EntityBase::GetLinkTransform() const
{
	return m_bLinkTransform;
}

void EntityBase::EntityUpdate(GameMessage message, const void* param)
{
	// 継承先で実装
}

void EntityBase::ComponentUpdate(GameMessage message, const void *param)
{
	for(int i = 0; i < eComponentKind_Max; ++i){
		if(m_pComponent[i]){
			m_pComponent[i]->Update(message, param);
		}
	}
}