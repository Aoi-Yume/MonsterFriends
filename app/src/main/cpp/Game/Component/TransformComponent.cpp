//
// Created by 葵ユメ on 2018/08/25.
//


#include <EntityBase.h>
#include "TransformComponent.h"

TransformComponent::TransformComponent(EntityBase* pEntityBase)
: Super(eComponentKind_Transform, pEntityBase)
, m_bBusy(false)
, m_scale(1, 1, 1)
, m_rotate(0, 0, 0)
, m_translate(0, 0, 0)
, m_AnimScale(1, 1, 1)
, m_AnimRotate(0, 0, 0)
, m_AnimTranslate(0, 0, 0)
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
	m_bBusy = true;
}

void TransformComponent::SetRotate(const VEC3 &rotate)
{
    m_rotate = rotate;
	m_bBusy = true;
}

void TransformComponent::SetTranslate(const VEC3 &translate)
{
    m_translate  = translate;
	m_bBusy = true;
}

void TransformComponent::SetAnimScale(const VEC3& scale)
{
	m_AnimScale = scale;
	m_bBusy = true;
}
void TransformComponent::SetAnimRotate(const VEC3& rotate)
{
	m_AnimRotate = rotate;
	m_bBusy = true;
}
void TransformComponent::SetAnimTranslate(const VEC3& translate)
{
	m_AnimTranslate = translate;
	m_bBusy = true;
}

const VEC3& TransformComponent::GetScale() const
{
	return m_scale;
}

const VEC3& TransformComponent::GetRotate() const
{
	return m_rotate;
}

const VEC3& TransformComponent::GetTranslate() const
{
	return m_translate;
}

const VEC3& TransformComponent::GetAnimScale() const
{
	return m_AnimScale;
}

const VEC3& TransformComponent::GetAnimRotate() const
{
	return m_AnimRotate;
}

const VEC3& TransformComponent::GetAnimTranslate() const
{
	return m_AnimTranslate;
}

void TransformComponent::CalcLocalMatrix()
{
    m_transform.Set(m_scale * m_AnimScale, m_rotate + m_AnimRotate, m_translate + m_AnimTranslate);
}

const MAT4* TransformComponent::GetLocalMatrix()
{
	if(m_bBusy){
		CalcLocalMatrix();
		m_bBusy = false;
	}
    return &m_transform;
}

const MAT4* TransformComponent::GetWorldMatrix()
{
	MAT4 worldMtx = getParentMatrix();
	m_worldTransform = worldMtx * (*GetLocalMatrix());

	return &m_worldTransform;
}

//------------------------------------------
//------------------------------------------
GameMessageResult TransformComponent::Update(GameMessage message, const void* param)
{
	switch(message)
	{
		case eGameMessage_Setup:
		case eGameMessage_Update: {
			if(m_bBusy){ CalcLocalMatrix(); }
			break;
		}
		default:{
			break;
		}
	}
	return eGameMessageResult_Break;
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