//
// Created by 葵ユメ on 2018/09/05.
//


#include <CollisionComponent.h>
#include "EntityManager.h"
#include "ComponentBase.h"

EntityManager::EntityManager()
{
	DEBUG_LOG("Call EntityManager Constructor");
}

EntityManager::~EntityManager()
{
    DEBUG_LOG("Call EntityManager Deconstructor");
	const size_t size = m_aEntity.size();
	for(uint i = 0; i < size; ++i){
		delete m_aEntity.at(i);
	}
}

EntityBase* EntityManager::GetEntity(uint idx)
{
	return m_aEntity.at(idx);
}

void EntityManager::Update(GameMessage message, const void* param)
{
	const size_t size = m_aEntity.size();
	for(uint i = 0; i < size; ++i){
		auto pEntityBaseA = m_aEntity.at(i);
		pEntityBaseA->Update(message, param);

		if(message == eGameMessage_Update){
			for (uint j = i + 1; j < size; ++j) {
				auto pEntityBaseB = m_aEntity.at(j);
				// 親同士の衝突判定
				checkCollisin(pEntityBaseA, pEntityBaseB);

				// 親子の判定
				const size_t iChildSize = pEntityBaseA->GetChildSize();
				const size_t jChildSize = pEntityBaseB->GetChildSize();
				for(uint jChild = 0; jChild < jChildSize; ++jChild) {
					checkCollisin(pEntityBaseA, pEntityBaseB->GetChild<EntityBase*>(jChild));
				}
				// 逆の親子関係もやる
				for(uint iChild = 0; iChild < iChildSize; ++iChild) {
					checkCollisin(pEntityBaseB, pEntityBaseA->GetChild<EntityBase*>(iChild));
				}

				// 子同士の衝突判定
				for(uint iChild = 0; iChild < iChildSize; ++iChild) {
					for(uint jChild = 0; jChild < jChildSize; ++jChild) {
						checkCollisin(pEntityBaseA->GetChild<EntityBase*>(iChild), pEntityBaseB->GetChild<EntityBase*>(jChild));
					}
				}
			}
		}
	}
}

void EntityManager::AttachEntity(EntityBase* pEntity)
{
	m_aEntity.push_back(pEntity);
}

void EntityManager::checkCollisin(EntityBase *pEntityBaseA, EntityBase *pEntityBaseB)
{
	if(!pEntityBaseA){ return; }
	auto pCollComponent = reinterpret_cast<CollisionComponent*>(pEntityBaseA->GetComponent(eComponentKind_Collision));
	if(!pCollComponent || !pCollComponent->IsActive()) { return; }

	if(!pEntityBaseB){ return; }
	auto pCollComponent2 = reinterpret_cast<CollisionComponent*>(pEntityBaseB->GetComponent(eComponentKind_Collision));
	if (!pCollComponent2 || !pCollComponent2->IsActive()) { return; }

	if (pCollComponent->IsIntersect(pCollComponent2)) {
		// TODO 排出処理
		HitCollisionParam hitParam(pCollComponent2->GetCollisionTag(), VEC3::Zero());
		HitCollisionParam hitParam2(pCollComponent->GetCollisionTag(), VEC3::Zero());
		pEntityBaseA->Update(eGameMessage_HitCollision, &hitParam);
		pEntityBaseB->Update(eGameMessage_HitCollision, &hitParam2);
	}
}