//
// Created by 葵ユメ on 2018/08/25.
//


#include <CameraComponent.h>
#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "CollisionComponent.h"
#include "InformationPlate.h"

InformationPlate::InformationPlate()
: GameEntity()
{
	DEBUG_LOG("Create InformationPlate");
}

InformationPlate::~InformationPlate()
{
}

void InformationPlate::GameEntitySetup(const void* param)
{
	Super::GameEntitySetup(param);
	{
		Entity::CreateLayoutComponent(this, "image/information_plate.png");
		auto pComponent = (LayoutComponent *) GetComponent(eComponentKind_Layout);
		pComponent->SetOrtho(true);
	}
	{
		Entity* pEntity = new Entity();
		Entity::CreateTextImageComponent(pEntity, "", 32);
		auto pText = (TextImageComponent*)pEntity->GetComponent(eComponentKind_Layout);
		pText->SetText("すらっち");
		pEntity->SetPosition(-100.0f, 60.0f, 0);
		pText->SetOrtho(true);
		AddChild(pEntity);
	}
	const float fPosX = (float)-Engine::GetEngine()->GetScreenInfo().m_nScreenX * 0.35f;
	const float fPosY = (float)-Engine::GetEngine()->GetScreenInfo().m_nScreenX * 0.21f;
	SetPosition(fPosX, fPosY, 0);
}

void InformationPlate::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);
}

void InformationPlate::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}