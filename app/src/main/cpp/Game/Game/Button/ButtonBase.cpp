//
// Created by 葵ユメ on 2018/09/22.
//


#include <LayoutComponent.h>
#include <CollisionComponent.h>
#include <TransformComponent.h>
#include <AnimationComponent.h>
#include "ButtonBase.h"

ButtonBase::ButtonBase()
: Entity()
, m_bEnable(true)
, m_bGray(false)
, m_nState(eState_UnSelect)
, m_uDecideCommand(0U)
, m_pColl2DRectComponent(nullptr)
, m_selectSELabel(SoundInvalidLabel)
{
	{
		Entity::CreateTransformComponent(this);
		Entity::CreateCollision2DRectComponent(this);
		auto pTransform = (TransformComponent*)GetComponent(eComponentKind_Transform);
		m_pColl2DRectComponent = (Collision2DRectComponent*)GetComponent(eComponentKind_Collision);
		m_pColl2DRectComponent->SetTransform(pTransform->GetLocalMatrix());
	}
}

ButtonBase::~ButtonBase()
{
}

void ButtonBase::SetSize(float fSizeW, float fSizeH)
{
	m_pColl2DRectComponent->SetSize(fSizeW, fSizeH);
}
	
void ButtonBase::Enable()
{
	m_bEnable = true;
}

void ButtonBase::Disable()
{
	m_bEnable = false;
}

bool ButtonBase::IsEnable() const
{
	return m_bEnable;
}

void ButtonBase::SetVisible(bool bVisible)
{
	Entity::SetVisible(bVisible);
	auto p = reinterpret_cast<AnimationComponent*>(GetComponent(eComponentKind_Animation));
	if(p) {
		p->Play((bVisible ? "Open" : "Close"));
	}
}

void ButtonBase::Select()
{
	m_nState = (m_nState == eState_Select) ? eState_Selected : eState_Select;
	if(m_selectSELabel != SoundInvalidLabel){
		Engine::GetEngine()->GetSoundManager()->PlaySE(m_selectSELabel, 1.0f, 1.0f, false);
	}
	ChangeColor();
}

void ButtonBase::UnSelect()
{
	m_nState = eState_UnSelect;
	ChangeColor();
}
	
bool ButtonBase::IsSelect() const
{
	return m_nState == eState_Select;
}

bool ButtonBase::IsSelected() const
{
	return m_nState == eState_Selected;
}

void ButtonBase::SetGray(bool bGray)
{
	m_bGray = bGray;
	ChangeColor();
}

bool ButtonBase::IsGray() const
{
	return m_bGray;
}

void ButtonBase::SetSelectSELabel(const char* pLabel)
{
	if(pLabel) {
		m_selectSELabel = Engine::GetEngine()->GetSoundManager()->LoadSE(pLabel);
	}
	else{
		m_selectSELabel = SoundInvalidLabel;
	}
}

bool ButtonBase::IsUnSelect() const
{
	return m_nState == eState_UnSelect;
}

void ButtonBase::ChangeColor()
{
}