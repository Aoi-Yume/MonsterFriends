//
// Created by 葵ユメ on 2018/09/22.
//


#include <LayoutComponent.h>
#include <CollisionComponent.h>
#include <TransformComponent.h>
#include <AnimationComponent.h>
#include "SimpleButton.h"

SimpleButton::SimpleButton(const char* pResName)
: Super()
, m_pLayoutComponent(nullptr)
{
	{
		Entity::CreateLayoutComponent(this, pResName);
		auto pAnimComponent = Entity::CreateAnimationComponent(this);
		pAnimComponent->AddAnimation("Open", new SimpleOpenAnimation());
		{
			auto pAddAnim = new LinearAnimation({1.0f, 0.9f}, {}, {}, {VEC4::One(), VEC4::One()}, 0.05f);
			pAddAnim->SetCallBack([=]() { pAnimComponent->Play("Off"); });
			pAnimComponent->AddAnimation("On", pAddAnim);
		}
		{
			auto pAddAnim = new LinearAnimation({0.9f, 1.0f}, {}, {}, {VEC4::One(), VEC4::One()}, 0.05f);
			pAnimComponent->AddAnimation("Off", pAddAnim);
		}


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
	GetComponent<AnimationComponent*>(eComponentKind_Animation)->Play("On");
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