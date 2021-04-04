//
// Created by 葵ユメ on 2018/08/25.
//


#include <EntityBase.h>
#include "TransformComponent.h"

TransformComponent::TransformComponent(EntityBase* pEntityBase)
: Super(eComponentKind_Transform, pEntityBase)
, m_scale(1, 1, 1)
, m_rotate(0, 0, 0)
, m_translate(0, 0, 0)
, m_transform()
, m_worldTransform()
{
//    DEBUG_LOG("Call TransformComponent Constructor");
}

TransformComponent::~TransformComponent()
{
//    DEBUG_LOG("Call TransformComponent Deconstructor");
}

void TransformComponent::SetScale(const VEC3& scale)
{
    m_scale  = scale;
	CalcLocalMatrix();
}

void TransformComponent::SetRotate(const VEC3 &rotate)
{
    m_rotate = rotate;
	CalcLocalMatrix();
}

void TransformComponent::SetTranslate(const VEC3 &translate)
{
    m_translate  = translate;
    CalcLocalMatrix();
}

VEC3 TransformComponent::GetScale() const
{
	return m_scale;
}

VEC3 TransformComponent::GetRotate() const
{
	return m_rotate;
}

VEC3 TransformComponent::GetTranslate() const
{
	return m_translate;
}

void TransformComponent::CalcLocalMatrix()
{
    m_transform.Set(m_scale, m_rotate, m_translate);
}

const MAT4* TransformComponent::GetLocalMatrix() const
{
    return &m_transform;
}

const MAT4* TransformComponent::GetWorldMatrix()
{
	MAT4 worldMtx = getParentMatrix();
	m_worldTransform = worldMtx * m_transform;

	return &m_worldTransform;
}

MAT4 TransformComponent::getParentMatrix()
{
	EntityBase* pEntityBase = GetEntityBase()->GetParent();
	if(pEntityBase && GetEntityBase()->GetLinkTransform()){
		auto pTransform = pEntityBase->GetComponent<TransformComponent*>(eComponentKind_Transform);
		if(pTransform){
			MAT4 mtx = *pTransform->GetLocalMatrix();
			return pTransform->getParentMatrix() * mtx ;
		}
	}
	return MAT4();
}