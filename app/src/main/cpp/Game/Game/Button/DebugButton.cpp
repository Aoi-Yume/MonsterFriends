//
// Created by 葵ユメ on 2018/09/22.
//


#include <LayoutComponent.h>
#include <CollisionComponent.h>
#include <TransformComponent.h>
#include "DebugButton.h"

DebugButton::DebugButton()
: ButtonBase()
, m_unSelectColor()
, m_selectColor()
, m_pDebugSqauareComponent(nullptr)
, m_pTextImageComponent(nullptr) {
	{
		Entity::CreateDebugSquareImageComponent(this);

		m_pDebugSqauareComponent = (DebugSquareImageComponent *)GetComponent(eComponentKind_Layout);
		m_pDebugSqauareComponent->SetOrtho(true);
	}
	{
		Entity *pEntity = new Entity();
		Entity::CreateTextImageComponent(pEntity, "", 32);
		AddChild(pEntity);

		m_pTextImageComponent = (TextImageComponent *) pEntity->GetComponent(eComponentKind_Layout);
		m_pTextImageComponent->SetOrtho(true);
	}
}

DebugButton::~DebugButton()
{
}

void DebugButton::SetText(const char *pText)
{
	m_pTextImageComponent->SetText(pText);
}

void DebugButton::SetFontColor(const VEC4& color)
{
	m_pTextImageComponent->SetFontColor(color);
}

void DebugButton::SetBackColor(const VEC4& unSelectColor, const VEC4& selectColor)
{
	m_unSelectColor = unSelectColor;
	m_selectColor = selectColor;
	
	ChangeColor();
}

void DebugButton::SetSize(float fSizeW, float fSizeH)
{
	m_pDebugSqauareComponent->SetSize(fSizeW, fSizeH);
	Super::SetSize(fSizeW, fSizeH);
}
	
void DebugButton::Select()
{
	Super::Select();
}

void DebugButton::UnSelect()
{
	Super::UnSelect();
}

void DebugButton::ChangeColor()
{
	if(IsSelect() || IsSelected()){
		m_pDebugSqauareComponent->SetColor(m_selectColor);
	}
	else{
		m_pDebugSqauareComponent->SetColor(m_unSelectColor);
	}
}