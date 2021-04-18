//
// Created by 葵ユメ on 2020/11/29.
//


#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "CollisionComponent.h"
#include "Character.h"
#include <AppCharaList.h>
#include <Dice.h>

Character::Character(int charaId)
: GameEntity()
, m_bCom(false)
, m_nCharaId(charaId)
, m_nUseDice(1)
, m_nStopDice(0)
{
	DEBUG_LOG_A("Create Character[%d]", m_nCharaId);
}

Character::~Character()
{
}

void Character::GameEntitySetup(const void* param)
{
	Super::GameEntitySetup(param);

	Entity::CreateLayoutComponent(this, AppCharaList::Get()->GetCharaInfo(m_nCharaId).fileName.c_str());
	reinterpret_cast<LayoutComponent*>(GetComponent(eComponentKind_Layout))->SetOrtho(true);

	for(int i = 0; i < eDice_Max; ++i){
		AddChild(new Dice(), false);
	}
}

void Character::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);

	switch(GetState()){
		case eState_None:{ break; }
		case eState_Wait:{ break; }
		case eState_ComBattleStandby:{ actComBattleStandbyState(); break; }
		case eState_StopDice:{ actStopDiceState(); break; }
		case eState_CheckEndDice:{ actCheckEndDiceState(); break; }
		case eState_EndDice:{ break; }
		default:{ break; }
	}
}

void Character::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}

void Character::ResetStopDice()
{
	m_nStopDice = 0;
	for(int i = 0; i < eDice_Max; ++i){
		GetChild<Dice*>(i)->ResetStopDice();
	}
}

void Character::BeginDice()
{
	const float fWidth = 750.0f;
	const float fAddX = fWidth / (float)(m_nUseDice + 1);
	const float fStartX = -fWidth * 0.5f;
	const float fOffset = AppCharaList::Get()->GetCharaInfo(m_nCharaId).fDiceOffsetY;
	for(int i = 0; i < m_nUseDice; ++i) {
		auto pDice = GetChild<Dice *>(i);
		pDice->StartDice();
		VEC3 pos = GetPosition();
		pos.SetX(pos.GetX() + fStartX + fAddX * (i + 1));
		pos.SetY(pos.GetY() + fOffset);
		pDice->SetPosition(pos);
	}

	m_fJumpStartY = GetPosition().GetY();
}

void Character::StopDice()
{
	SetNextState(eState_StopDice);
}

int Character::GetDiceSumVal() const
{
	int nSum = 0;
	for(int i = 0; i < m_nUseDice; ++i){
		nSum += GetChild<Dice*>(i)->GetDice();
	}
	return nSum;
}

void Character::BeginComDice()
{
	SetNextState(eState_ComBattleStandby);
}

void Character::InVisibleDice()
{
	for(int i = 0; i < eDice_Max; ++i){
		GetChild<Dice*>(i)->InVisible();
	}
}

void Character::actComBattleStandbyState()
{
	const int nStateCnt = GetStateCount();
	if(nStateCnt == 0){
		BeginDice();
	}
	else if(nStateCnt >= 120){
		StopDice();
	}
}

void Character::actStopDiceState()
{
	const int nStateCnt = GetStateCount();
	const int nJumpFrameMaxHalf = 10;
	const float fJumpFrameMax = (float)nJumpFrameMaxHalf * 2.0f;
	const float fRatio = std::min((float)nStateCnt / fJumpFrameMax, 1.0f);
	const float fJumpRatio = cosf(DEGTORAD(189.0f * fRatio));
	const float fJumpPow = 25.0f;

	VEC3 pos = GetPosition();
	pos.SetY(m_fJumpStartY + fJumpPow * fJumpRatio);
	SetPosition(pos);

	if(nStateCnt == nJumpFrameMaxHalf){
		GetChild<Dice*>(m_nStopDice)->StopDice();
	}
	else if(nStateCnt >= (int)fJumpFrameMax){
		SetNextState(eState_CheckEndDice);
	}
}

void Character::actCheckEndDiceState()
{
	m_nStopDice++;
	if(m_nStopDice < m_nUseDice){
		if(!m_bCom){
			SetNextState(eState_None);
		}
		else {
			StopDice();
		}
	}
	else{
		SetNextState(eState_EndDice);
	}
}
