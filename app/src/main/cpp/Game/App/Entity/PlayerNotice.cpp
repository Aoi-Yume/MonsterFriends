//
// Created by 葵ユメ on 2021/03/13.
//


#include <CameraComponent.h>
#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "AnimationComponent.h"
#include "PlayerNotice.h"
#include <Random.h>

PlayerNotice::PlayerNotice(NoticeType type, int nNoticePlayer)
: GameEntity()
, m_nType(type)
, m_State(eState_Idle)
, m_nNoticePlayer(nNoticePlayer)
, m_fDelay(0.0f)
, m_bAutoClose(false)
{
	DEBUG_LOG("Create PlayerNotice");
}

PlayerNotice::~PlayerNotice()
{
}

void PlayerNotice::GameEntitySetup(const void* param)
{
	Entity::CreateTransformComponent(this);
	Super::GameEntitySetup(param);

	std::pair<const char*, float> pResInfo[eNoticeType_Max] ={
			{ "image/player_turn_notice.png", 50.0f },
			{ "image/winner_notice.png", 0.0f }
	};
	{
		Entity* pChild = new Entity();
		Entity::CreateLayoutComponent(pChild, pResInfo[m_nType].first)->SetOrtho(true);
		auto pAnimComponent = Entity::CreateAnimationComponent(pChild);
		{
			const VEC3 pos = {-100.0f + pResInfo[m_nType].second, 0, 0};
			const VEC3 targetPos = {pResInfo[m_nType].second, 0, 0};
			auto pAnim = new LinearAnimation({1, 1}, {0, 0}, {pos, targetPos}, {0, 1}, 0.5f);
			pAnimComponent->AddAnimation("Open", pAnim);
		}
		{
			const VEC3 pos = {pResInfo[m_nType].second, 0, 0};
			const VEC3 targetPos = {pResInfo[m_nType].second + 100.0f, 0, 0};
			auto pAnim = new LinearAnimation({1, 1}, {0, 0}, {pos, targetPos}, {1, 0}, 0.5f);
			pAnimComponent->AddAnimation("Close", pAnim);
		}
		AddChild(pChild);
	}
	{
		Entity *pChild = new Entity();
		char resName[128];
		std::snprintf(resName, sizeof(resName), "image/player_no_%d.png", m_nNoticePlayer + 1);
		Entity::CreateLayoutComponent(pChild, resName)->SetOrtho(true);
		auto pAnimComponent = Entity::CreateAnimationComponent(pChild);
		{
			auto pAnim = new LinearAnimation({2, 1}, {0, 0}, {{-460, 48, 0},{-460, 48, 0}}, {0, 1}, 0.5f);
			pAnimComponent->AddAnimation("Open", pAnim);
		}
		{
			auto pAnim = new LinearAnimation({1, 2}, {0, 0}, {{-460, 48, 0},{-460, 48, 0}}, {1, 0}, 0.5f);
			pAnimComponent->AddAnimation("Close", pAnim);
		}
		AddChild(pChild);
	}
	
	SetPosition(200.0f, 300.0f, 0);
	SetVisible(false);
}

void PlayerNotice::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);

	auto pAnimComponent = reinterpret_cast<AnimationComponent*>(GetChild(0)->GetComponent(eComponentKind_Animation));
	if(m_State == eState_Open){
		if(pAnimComponent->IsEnd("Open")){
			m_State = (m_bAutoClose ? eState_Wait : eState_Idle);
		}
	}
	else if(m_State == eState_Wait){
		m_fDelay -= Engine::GetEngine()->GetDeltaTime();
		if(m_fDelay <= 0.0f){
			Close();
			m_fDelay = 0.0f;
		}
	}
	else if(m_State == eState_Close){
		if(pAnimComponent->IsEnd("Close")){
			m_State = eState_Idle;
		}
	}

}

void PlayerNotice::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}

void PlayerNotice::Open(bool bAutoClose, float fDelay)
{
	for(int i = 0; i < GetChildSize(); ++i) {
		auto pChild = GetChild(i);
		reinterpret_cast<AnimationComponent *>(pChild->GetComponent(eComponentKind_Animation))->Play("Open");
	}
	m_State = eState_Open;
	m_bAutoClose = bAutoClose;
	m_fDelay = fDelay;
}

void PlayerNotice::Close()
{
	for(int i = 0; i < GetChildSize(); ++i) {
		auto pChild = GetChild(i);
		reinterpret_cast<AnimationComponent *>(pChild->GetComponent(eComponentKind_Animation))->Play("Close");
	}
	m_State = eState_Close;
}