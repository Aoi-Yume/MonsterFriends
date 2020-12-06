//
// Created by 葵ユメ on 2020/11/29.
//


#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "CollisionComponent.h"
#include "Character.h"

Character::Character(CHARA_ID charaId)
: GameEntity()
, m_nCharaId(charaId)
{
	DEBUG_LOG("Create Character");
}

Character::~Character()
{
}

void Character::GameEntitySetup(const void* param)
{
	Super::GameEntitySetup(param);

	char imagePath[64];
	snprintf(imagePath, sizeof(imagePath), "image/chara_%02d.png", m_nCharaId + 1);
	imagePath[sizeof(imagePath) - 1] = '\0';
	Entity::CreateLayoutComponent(this, imagePath);
	reinterpret_cast<LayoutComponent*>(GetComponent(eComponentKind_Layout))->SetOrtho(true);
}

void Character::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);

	switch(GetState()){
		case eState_None:{ break; }
		case eState_Wait:{ break; }
		case eState_Work:{ actWorkState(); break; }
		default:{ break; }
	}
}

void Character::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}

void Character::actWorkState()
{
	const int nStateCnt = GetStateCount();
	const int nHalfFrame = 360;
	const int nLoopCnt = nStateCnt % (nHalfFrame * 2);
	const float fMoveWidth = (float)Engine::GetEngine()->GetScreenInfo().m_nScreenX + 1200.0f;
	if(nLoopCnt < nHalfFrame){
		const float fRatio = (float)nLoopCnt / (float)nHalfFrame;
		auto pos = GetPosition();
		pos.SetX(fMoveWidth * fRatio - fMoveWidth * 0.5f);
		SetPosition(pos);
		auto scale = GetScale();
		scale.SetX(std::abs(scale.GetX()));
		SetScale(scale);
	}
	else {
		const float fRatio = 1.0f - (float)(nLoopCnt - nHalfFrame) / (float)nHalfFrame;
		auto pos = GetPosition();
		pos.SetX(fMoveWidth * fRatio - fMoveWidth * 0.5f);
		SetPosition(pos);
		auto scale = GetScale();
		scale.SetX(-std::abs(scale.GetX()));
		SetScale(scale);
	}
}