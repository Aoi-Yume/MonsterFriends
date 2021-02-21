//
// Created by 葵ユメ on 2020/11/23.
//


#include <CameraComponent.h>
#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "CollisionComponent.h"
#include "Dice.h"
#include <Random.h>

Dice::Dice()
: GameEntity()
, m_bDiceStart(false)
, m_nCurrentDiceIdx(0)
, m_nCurrentDiceNo(1)
, m_nDiceChangeInterval(10)
, m_uDiceRoulette()
{
	DEBUG_LOG("Create InformationPlate");
	for(int i = 0; i < eDICE_MAX; ++i){
		m_uDiceRoulette[i] = i + 1;
	}
	Random::SyncShuffle(m_uDiceRoulette, eDICE_MAX);
	ResetStopDice();
}

Dice::~Dice()
{
}

void Dice::GameEntitySetup(const void* param)
{
	Entity::CreateTransformComponent(this);
	Super::GameEntitySetup(param);

	for(int i = 0; i < 6; ++i) {
		char imagePath[64];
		snprintf(imagePath, sizeof(imagePath), "image/dice_%d.png", i + 1);
		imagePath[sizeof(imagePath) - 1] = '\0';
		auto pChild = new Entity();
		Entity::CreateLayoutComponent(pChild, imagePath);
		auto pComponent = (LayoutComponent *)pChild->GetComponent(eComponentKind_Layout);
		pComponent->SetOrtho(true);
		pComponent->SetVisible(i == 0);
		AddChild(pChild);
	}
}

void Dice::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);

	if(m_bDiceStart){
		if(GetStateCount() % m_nDiceChangeInterval != 0){ return; }
		for(int i = 0; i < eDICE_MAX; ++i){
			SetVisible(i, (i + 1) == m_uDiceRoulette[m_nCurrentDiceIdx]);
		}
		m_nCurrentDiceNo = m_uDiceRoulette[m_nCurrentDiceIdx];
		m_nCurrentDiceIdx = (m_nCurrentDiceIdx + 1) % eDICE_MAX;
	}
}

void Dice::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}

void Dice::SetVisible(int nIdx, bool bVisible)
{
	auto pComponent = reinterpret_cast<LayoutComponent*>(GetChild(nIdx)->GetComponent(eComponentKind_Layout));
	pComponent->SetVisible(bVisible);
}

void Dice::InVisible()
{
	for(int i = 0; i < eDICE_MAX; ++i) {
		auto pComponent = reinterpret_cast<LayoutComponent*>(GetChild(i)->GetComponent(eComponentKind_Layout));
		pComponent->SetVisible(false);
	}
}

void Dice::StartDice()
{
	m_bDiceStart = true;
}

void Dice::StopDice()
{
	m_bDiceStart = false;
	for(int i = 0; i < eDICE_MAX; ++i){
		SetVisible(i, (i + 1) == m_nStopDiceNo);
	}
	m_nCurrentDiceNo = m_nStopDiceNo;
}

void Dice::ResetStopDice()
{
	m_nStopDiceNo = Random::GetSyncInt(1, eDICE_MAX);
}

uint8_t Dice::GetDice() const
{
	return m_nCurrentDiceNo;
}