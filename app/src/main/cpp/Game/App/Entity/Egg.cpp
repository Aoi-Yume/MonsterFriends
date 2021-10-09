//
// Created by 葵ユメ on 2021/05/04.
//


#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "Egg.h"
#include "Shadow.h"

Egg::Egg()
: GameEntity()
, m_nCurrentImageNo(0)
, m_pShadow(nullptr)
{
	DEBUG_LOG("Create Egg");
}

Egg::~Egg()
{
	delete m_pShadow;
}

void Egg::GameEntitySetup(const void* param)
{
	Entity::CreateTransformComponent(this);
	Super::GameEntitySetup(param);

	for(int i = 0; i < eEGG_BORNE_KIND_MAX; ++i) {
		char imagePath[64];
		snprintf(imagePath, sizeof(imagePath), "image/egg_0_%d.png", i);
		imagePath[sizeof(imagePath) - 1] = '\0';
		auto pChild = new Entity();
		auto pComponent = Entity::CreateLayoutComponent(pChild, imagePath);
		pComponent->SetOrtho(true);
		pComponent->SetVisible(false);
		AddChild(pChild);
	}
	GetChild(0)->SetVisible(true);

	m_pShadow = new Shadow("たまご影", GetChild(0));
	m_pShadow->Update(eGameMessage_Setup, nullptr);
}

void Egg::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);

	switch(GetState()){
		case eSTATE_WAIT:{  actWaitState(); break; }
		case eSTATE_BORNE:{ actBorneState(); break; }
		case eSTATE_BORNE_END:{ /*何もしない*/ break; }
	}
}

void Egg::EntityUpdate(GameMessage message, const void* param)
{
	if(m_pShadow){
		m_pShadow->Update(message, param);
	}
	Super::EntityUpdate(message, param);
}

void Egg::StartBorne()
{
	SetNextState(eSTATE_BORNE);
}

void Egg::actWaitState()
{
	VEC3 rot = {0, 0, sinf(DEGTORAD(GetStateTime() * 100.0f)) * 20.0f};
	SetRotate(rot);
}

void Egg::actBorneState()
{
	const int nStateCount = GetStateCount();
	const int nOneCount = 40;
	const int nIdx = std::min(nStateCount / nOneCount, (int)eEGG_BORNE_KIND_MAX - 1);
	if(nIdx < eEGG_BORNE_KIND_MAX - 1 && nStateCount % nOneCount == nOneCount - 1){
		auto pSoundManager = Engine::GetEngine()->GetSoundManager();
		pSoundManager->PlaySE(pSoundManager->LoadSE(SE_LABEL_EGG_CRACK), 1.0f, 1.0f, false);
	}
	for(int i = 0; i < eEGG_BORNE_KIND_MAX; ++i){
		GetChild(i)->SetVisible(i == nIdx);
	}
	if(m_pShadow){
		m_pShadow->ChangeReferenceEntity(GetChild(nIdx));
	}
	if(nStateCount >= nOneCount * (eEGG_BORNE_KIND_MAX + 1)){
		SetNextState(eSTATE_BORNE_END);
	}
}