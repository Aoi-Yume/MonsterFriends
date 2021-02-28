//
// Created by 葵ユメ on 2021/02/27.
//

#include <LayoutComponent.h>
#include "FadeCtrl.h"

USE_SINGLETON_VARIABLE(FadeCtrl);

FadeCtrl::FadeCtrl()
: Singleton<FadeCtrl>()
, m_fFadeTime(0.0f)
, m_fFadeDuration(0.0f)
, m_FadeState(eFadeState_None)
, m_pFadeEntity(nullptr)
{
	DEBUG_LOG("Create FadeCtrl");
	
	m_pFadeEntity = new Entity();
	Entity::CreateLayoutComponent(m_pFadeEntity, "common/image/fade.png");
	auto p = (LayoutComponent*)m_pFadeEntity->GetComponent(eComponentKind_Layout);
	p->SetOrtho(true);
	p->SetColor({0, 0, 0, 0});
	const auto& info = Engine::GetEngine()->GetScreenInfo();
	m_pFadeEntity->SetScale(info.m_nScreenX, info.m_nScreenY, 1.0f);
	m_pFadeEntity->Update(eGameMessage_Setup, nullptr);
	m_pFadeEntity->SetVisible(false);
}


FadeCtrl::~FadeCtrl()
{
	delete m_pFadeEntity;
	DEBUG_LOG("Destroy FadeCtrl");
}

void FadeCtrl::In(float fDuration)
{
	m_FadeState = eFadeState_In;
	m_fFadeTime = 0.0f;
	m_fFadeDuration = fDuration;
}

void FadeCtrl::Out(float fDuration)
{
	m_FadeState = eFadeState_Out;
	m_fFadeTime = 0.0f;
	m_fFadeDuration = fDuration;
}

bool FadeCtrl::IsFadeInEnd() const
{
	return m_FadeState == eFadeState_InEnd;
}

bool FadeCtrl::IsFadeOutEnd() const
{
	return m_FadeState == eFadeState_OutEnd;
}

void FadeCtrl::Update(GameMessage message, const void* param)
{
	if(message == eGameMessage_Update){
		fadeUpdate();
	}
	if(message != eGameMessage_Setup){
		m_pFadeEntity->Update(message, param);
	}
}

void FadeCtrl::fadeUpdate()
{
	auto p = (LayoutComponent*)m_pFadeEntity->GetComponent(eComponentKind_Layout);
	if(m_FadeState == eFadeState_In) {
		const float alpha = 1.0f - std::min(m_fFadeTime / m_fFadeDuration, 1.0f);
		p->SetColor({0, 0, 0, alpha});
		m_pFadeEntity->SetVisible(true);

		if(m_fFadeTime >= m_fFadeDuration){
			m_FadeState = eFadeState_InEnd;
		}
	}
	else if(m_FadeState == eFadeState_Out){
		const float alpha = std::min(m_fFadeTime / m_fFadeDuration, 1.0f);
		p->SetColor({0, 0, 0, alpha});
		m_pFadeEntity->SetVisible(true);

		if(m_fFadeTime >= m_fFadeDuration){
			m_FadeState = eFadeState_OutEnd;
		}
	}
	m_fFadeTime += Engine::GetEngine()->GetDeltaTime();
}