//
// Created by 葵ユメ on 2021/08/27.
//

#include "PermissionExplain.h"
#include "Button/ButtonManager.h"
#include "Button/SimpleButton.h"

PermissionExplain::PermissionExplain()
: m_pButtonManager(nullptr)
, m_aPermissionExplain()
{
}

PermissionExplain::~PermissionExplain()
{
	delete m_pButtonManager;
}

void PermissionExplain::GameEntitySetup(const void* param)
{
	Super::GameEntitySetup(param);
	m_pButtonManager = new ButtonManager();
	auto btn = m_pButtonManager->CreateButton("image/button_backStr.png");
	btn->SetPosition(0, -400.0f, 0);
	btn->SetSelectSELabel(SE_LABEL_CANCEL);

	m_pButtonManager->SetVisible(false);
	m_pButtonManager->Lock();
}

void PermissionExplain::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);
}

void PermissionExplain::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
	m_pButtonManager->Update(message, param);
}

void PermissionExplain::SetVisible(bool bVisible)
{
	for(auto& it : m_aPermissionExplain){
		it.second->SetVisible(false);
	}
}

void PermissionExplain::AddPermissionExplain(const char* pName, const char* pResPath)
{
	Entity* pEntity = new Entity();
	Entity::CreateLayoutComponent(pEntity, pResPath)->SetOrtho(true);
	auto pAnim = Entity::CreateAnimationComponent(pEntity);
	pEntity->Update(eGameMessage_Setup, nullptr);

	pAnim->AddAnimation("Open", new LinearAnimation({1.0f, 1.0f}, {0.0f, 0.0f}, {VEC3(0, 55.0f, 0), VEC3(0, 25.0f, 0)}, {0.0f, 1.0f}));
	pAnim->AddAnimation("Close", new LinearAnimation({1.0f, 1.0f}, {0.0f, 0.0f}, {VEC3(0, 25.0f, 0), VEC3(0, 55.0f, 0)}, {1.0f, 0.0f}));
	pEntity->SetVisible(false);
	AddChild(pEntity);

	m_aPermissionExplain[pName] = pEntity;
}

void PermissionExplain::Open(const char* pName)
{
	auto pChild = m_aPermissionExplain[pName];
	pChild->GetComponent<AnimationComponent*>(eComponentKind_Animation)->Play("Open");
	pChild->SetVisible(true);

	m_pButtonManager->Unlock();
	m_pButtonManager->Reset();
	m_pButtonManager->SetVisible(true);
}

void PermissionExplain::Close(const char *pName)
{
	auto pChild = m_aPermissionExplain[pName];
	pChild->GetComponent<AnimationComponent*>(eComponentKind_Animation)->Play("Close");

	m_pButtonManager->Lock();
	m_pButtonManager->SetVisible(false);
}

bool PermissionExplain::IsNext() const
{
	return m_pButtonManager->GetDecide() != -1;
}