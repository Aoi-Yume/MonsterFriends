//
// Created by 葵ユメ on 2021/10/09.
//


#include "Shadow.h"

Shadow::Shadow(const char* pShadowName, Entity* pEntity)
: GameEntity()
, m_bAerial(false)
, m_fGroundY(0.0f)
, m_pRefEntity(pEntity)
, m_ShadowInfo()
{
	DEBUG_LOG_A("Create Shadow[%s]", pShadowName);
	m_ShadowInfo = AppShadowList::Get()->GetShadowInfo(pShadowName);
}

Shadow::~Shadow()
{
}

void Shadow::GameEntitySetup(const void* param)
{
	Super::GameEntitySetup(param);

	Entity::CreateLayoutComponent(this, m_ShadowInfo.fileName.c_str());
	const auto pLayoutComponent = GetComponent<LayoutComponent*>(eComponentKind_Layout);
	pLayoutComponent->SetOrtho(true);
	pLayoutComponent->SetBlendMode(DrawComponent::eBlend_Multiply);
	pLayoutComponent->SetDiscardFactor(0.9f);
	SetVisible(false);
}

void Shadow::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);

	VEC3 scale = m_pRefEntity->GetScale();
	SetScale(scale);
	VEC3 pos = m_pRefEntity->GetPosition();
	if(m_bAerial){
		pos.SetY(m_fGroundY);
	}
	else{
		m_fGroundY = pos.GetY();
	}
	VEC3 offset = VEC3(m_ShadowInfo.offsetX * scale.GetX(), m_ShadowInfo.offsetY, 0);
	pos += offset;
	SetPosition(pos);
	SetVisible(m_pRefEntity->IsVisible());
}

void Shadow::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}

void Shadow::SetAerial(bool bAerial)
{
	m_bAerial = bAerial;
}