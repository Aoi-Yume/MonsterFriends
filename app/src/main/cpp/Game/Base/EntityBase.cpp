//
// Created by 葵ユメ on 2018/08/25.
//


#include "EntityBase.h"
#include "ComponentBase.h"

EntityBase::EntityBase()
: m_pParentEntity(nullptr)
, m_bLinkTransform(false)
, m_nMessageCnt(0)
, m_prevMessage(eGameMessage_Max)
{
	for(int i = 0; i < eComponentKind_Max; ++i){
		m_pComponent[i] = nullptr;
	}
}

EntityBase::~EntityBase()
{
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
	if(message != m_prevMessage){
		m_nMessageCnt = 0;
	}

	EntityUpdate(message, param);
	ComponentUpdate(message, param);

	const size_t size = m_svpChild.size();
	for(uint32_t i = 0; i < size; ++i){
		if(m_svpChild.at(i)) {
			m_svpChild.at(i)->Update(message, param);
		}
	}

	m_nMessageCnt++;
}

void EntityBase::AddChild(EntityBase* pEntity, bool bLinkTransform)
{
	pEntity->SetParent(this, bLinkTransform);
	m_svpChild.emplace_back(pEntity);
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

int EntityBase::GetMessageCnt() const
{
	return m_nMessageCnt;
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