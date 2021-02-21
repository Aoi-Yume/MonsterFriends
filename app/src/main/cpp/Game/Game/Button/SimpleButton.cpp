//
// Created by 葵ユメ on 2018/09/22.
//


#include <LayoutComponent.h>
#include <CollisionComponent.h>
#include <TransformComponent.h>
#include "SimpleButton.h"

SimpleButton::SimpleButton(const char* pResName)
: Super()
, m_pLayoutComponent(nullptr)
{
	{
		Entity::CreateLayoutComponent(this, pResName);

		m_pLayoutComponent = (LayoutComponent *) GetComponent(eComponentKind_Layout);
		m_pLayoutComponent->SetOrtho(true);
	}
}

SimpleButton::~SimpleButton()
{
}

void SimpleButton::SetSize(float fSizeW, float fSizeH)
{
	Super::SetSize(fSizeW, fSizeH);
}

void SimpleButton::Select()
{
	Super::Select();
}

void SimpleButton::UnSelect()
{
	Super::UnSelect();
}

void SimpleButton::ChangeColor()
{
	if(IsGray()){
		m_pLayoutComponent->SetColor(VEC4(0.5f, 0.5f, 0.5f, 1.0f));
	}
	else{
		m_pLayoutComponent->SetColor(VEC4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}