﻿//
// Created by 葵ユメ on 2021/03/07.
//


#include <stdio.h>
#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "vector"
#include "DrawComponent.h"

//==========================================
//==========================================
AnimationComponent::AnimationComponent(EntityBase* pEntityBase)
: Super(eComponentKind_Animation, pEntityBase)
, m_pLastAnim(nullptr)
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
	pAnimation->setEntityBase(GetEntityBase());
	m_aAnimation.emplace(pName, pAnimation);
}

//------------------------------------------
//------------------------------------------
void AnimationComponent::Play(const char* pName, float fSpeed)
{
	if(m_aAnimation.find(pName) == m_aAnimation.end()){ return; }
	if(m_pLastAnim) { m_pLastAnim->Stop(); }
	m_aAnimation[pName]->SetSpeed(fSpeed);
	m_aAnimation[pName]->Play();
	m_pLastAnim = m_aAnimation[pName];
}
//------------------------------------------
//------------------------------------------
void AnimationComponent::Stop(const char* pName)
{
	if(m_aAnimation.find(pName) == m_aAnimation.end()){ return; }
	m_aAnimation[pName]->Stop();
	m_pLastAnim = nullptr;
}

//------------------------------------------
//------------------------------------------
bool AnimationComponent::IsPlay(const char* pName) const
{
	if(m_aAnimation.find(pName) == m_aAnimation.end()){ return false; }
	return m_aAnimation.at(pName)->IsPlay();
}

//------------------------------------------
//------------------------------------------
bool AnimationComponent::IsEnd(const char* pName) const
{
	if(m_aAnimation.find(pName) == m_aAnimation.end()){ return false; }
	return m_aAnimation.at(pName)->IsEnd();
}

//------------------------------------------
//------------------------------------------
int AnimationComponent::GetState(const char* pName) const
{
	if(m_aAnimation.find(pName) == m_aAnimation.end()){ return -1; }
	return m_aAnimation.at(pName)->GetState();
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
	transform->SetAnimScale({fScale, fScale, fScale});

	Super::update(fDeltaTime);
}

//==========================================
//==========================================
LinearAnimation::LinearAnimation(
	std::pair<float, float>&& scale,
	std::pair<float, float>&& rotZ,
	std::pair<VEC3, VEC3>&& pos,
	std::pair<VEC4, VEC4>&& color,
	float fMaxTime)
	: Super(fMaxTime)
	, m_LinearScale(std::forward<std::pair<float, float>>(scale))
	, m_LinearRotZ(std::forward<std::pair<float, float>>(rotZ))
	, m_LinearPos(std::forward<std::pair<VEC3, VEC3>>(pos))
	, m_LinearColor(std::forward<std::pair<VEC4, VEC4>>(color))
{
}

//------------------------------------------
//------------------------------------------
LinearAnimation::~LinearAnimation()
{
}

//------------------------------------------
//------------------------------------------
void LinearAnimation::update(const float fDeltaTime)
{
	auto transform = reinterpret_cast<TransformComponent*>(GetEntityBase()->GetComponent(eComponentKind_Transform));
	auto draw = reinterpret_cast<DrawComponent*>(GetEntityBase()->GetComponent(eComponentKind_Layout));
	const float fBaseRatio = GetRatio();
	const float fScale = Lerp(m_LinearScale.first, m_LinearScale.second,  fBaseRatio);
	transform->SetAnimScale({fScale, fScale, fScale});

	const float fRotZ = Lerp(m_LinearRotZ.first, m_LinearRotZ.second, fBaseRatio);
	transform->SetAnimRotate({0, 0, DEGTORAD(fRotZ)});

	const auto pos = Lerp(m_LinearPos.first, m_LinearPos.second, fBaseRatio);
	transform->SetAnimTranslate(pos);

	const auto color = Lerp(m_LinearColor.first, m_LinearColor.second, fBaseRatio);
	draw->SetAnimColor(color);

	Super::update(fDeltaTime);
}