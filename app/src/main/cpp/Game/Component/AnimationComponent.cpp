//
// Created by 葵ユメ on 2021/03/07.
//


#include <stdio.h>
#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "vector"

//==========================================
//==========================================
AnimationComponent::AnimationComponent(EntityBase* pEntityBase)
: Super(eComponentKind_Animation, pEntityBase)
, m_aAnimation()
{
//	DEBUG_LOG("Call LayoutComponent Constructor");
}

//------------------------------------------
//------------------------------------------
AnimationComponent::~AnimationComponent()
{
	destroy();
}

//------------------------------------------
//------------------------------------------
void AnimationComponent::AddAnimation(const char* pName, AnimationBase *pAnimation)
{
	if(m_aAnimation.find(pName) != m_aAnimation.end()){ return; }
	pAnimation->SetEntityBase(GetEntityBase());
	m_aAnimation.emplace(pName, pAnimation);
}

//------------------------------------------
//------------------------------------------
void AnimationComponent::Play(const char* pName, float fSpeed)
{
	if(m_aAnimation.find(pName) == m_aAnimation.end()){ return; }
	m_aAnimation[pName]->SetSpeed(fSpeed);
	m_aAnimation[pName]->Play();
}
//------------------------------------------
//------------------------------------------
void AnimationComponent::Stop(const char* pName)
{
	if(m_aAnimation.find(pName) == m_aAnimation.end()){ return; }
	m_aAnimation[pName]->Stop();
}

//------------------------------------------
//------------------------------------------
GameMessageResult AnimationComponent::Update(GameMessage message, const void* param)
{
	switch(message)
	{
		case eGameMessage_Setup:{
			break;
		}
		case eGameMessage_Update:{
			update();
			break;
		}
		case eGameMessage_Pause:{
			break;
		}
		case eGameMessage_Destroy:{
			destroy();
			break;
		}
		default:{
			break;
		}
	}
	return eGameMessageResult_Break;
}

//------------------------------------------
//------------------------------------------
void AnimationComponent::update()
{
	for(auto& it : m_aAnimation){
		if(it.second->IsPlay()){
			it.second->update(Engine::GetEngine()->GetDeltaTime());
		}
	}
}

//------------------------------------------
//------------------------------------------
void AnimationComponent::destroy()
{
	DEBUG_LOG("Destroy Animation Component");
	for(auto& it : m_aAnimation) {
		delete it.second;
	}
	m_aAnimation.clear();
}

//==========================================
//==========================================
SimpleOpenAnimation::SimpleOpenAnimation(float fDefaultScale, float fOverScale, float fMaxTime)
	: Super(fMaxTime)
	, m_fDefaultScale(fDefaultScale)
	, m_fOverScale(fOverScale)
{
}

//------------------------------------------
//------------------------------------------
SimpleOpenAnimation::~SimpleOpenAnimation()
{
}

//------------------------------------------
//------------------------------------------
void SimpleOpenAnimation::update(const float fDeltaTime)
{
	auto transform = reinterpret_cast<TransformComponent*>(GetEntityBase()->GetComponent(eComponentKind_Transform));
	const float fBaseRatio = GetRatio();
	const float fOverRatio = std::min(fBaseRatio / 0.8f, 1.0f);
	const float fDefaultRatio = std::min(std::max(fBaseRatio - 0.8f, 0.0f) / 0.2f, 1.0f);
	const float fScale = m_fOverScale * fOverRatio * (1.0f - fDefaultRatio) + m_fDefaultScale * fDefaultRatio;
	transform->SetScale({fScale, fScale, fScale});

	Super::update(fDeltaTime);
}