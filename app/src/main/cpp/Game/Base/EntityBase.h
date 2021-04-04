//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_ENTITY_BASE_H
#define AOIYUME_ENTITY_BASE_H

#include <vector>
#include "../Engine/Engine.h"
#include "entity_define.h"
#include "BaseDefine.h"

class ComponentBase;
class Entity;

class EntityBase
{
public:
	EntityBase();
	virtual ~EntityBase();

	void Update(GameMessage message, const void* param);

	template<class T = ComponentBase*> T GetComponent(ComponentKind nKind);

	void AddChild(EntityBase* pEntity, bool bLinkTransform = true);
	void ResizeChild(unsigned long num);
	void SetChild(unsigned long idx, EntityBase* pEntity, bool bLinkTransform = true);

	template<class T = Entity*> T GetChild(unsigned long idx);
	template<class T = Entity*> const T GetChild(unsigned long idx) const;
	size_t GetChildSize() const;

	void SetParent(EntityBase* pEntity, bool bLinkTransform);
	EntityBase* GetParent();

	bool GetLinkTransform() const;

	int GetMessageCnt() const;
protected:
	virtual void EntityUpdate(GameMessage message, const void* param);
	void ComponentUpdate(GameMessage message, const void* param);

protected:
	ComponentBase* 	m_pComponent[eComponentKind_Max];
	EntityBase* m_pParentEntity;
	std::vector<EntityBase*>	m_svpChild;

	bool 		m_bLinkTransform;

	int 		m_nMessageCnt;
	GameMessage	m_prevMessage;

};

template<class T>
inline T EntityBase::GetComponent(ComponentKind nKind)
{
	return reinterpret_cast<T>(m_pComponent[nKind]);
}

template <class T>
inline T EntityBase::GetChild(unsigned long idx) {
	assert(idx >= 0 && idx < m_svpChild.size());
	return reinterpret_cast<T>(m_svpChild.at(idx));
}

template <class T>
inline const T EntityBase::GetChild(unsigned long idx) const {
	assert(idx >= 0 && idx < m_svpChild.size());
	return reinterpret_cast<T>(m_svpChild.at(idx));
}

#endif