//
// Created by 葵ユメ on 2018/08/25.
//


#include <CameraComponent.h>
#include <AppParam.h>
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

void InformationPlate::GameEntitySetup(const void* param) {
	Super::GameEntitySetup(param);
	{
		Entity::CreateLayoutComponent(this, "image/information_plate.png");
		auto pComponent = (LayoutComponent *) GetComponent(eComponentKind_Layout);
		pComponent->SetOrtho(true);
	}
	{
		Entity *pEntity = new Entity();
		Entity::CreateTextImageComponent(pEntity, "", 32);
		auto pText = (TextImageComponent *) pEntity->GetComponent(eComponentKind_Layout);
		pText->SetText(AppParam::Get()->GetCharaName());
		pEntity->SetPosition(-100.0f, 60.0f, 0);
		pText->SetOrtho(true);
		AddChild(pEntity);
	}
	{
		Entity *pEntity = new Entity();
		Entity::CreateTextImageComponent(pEntity, "", 32);
		auto pText = (TextImageComponent *) pEntity->GetComponent(eComponentKind_Layout);
		pText->SetText("キズナ");
		pEntity->SetPosition(-100.0f, 0.0f, 0);
		pText->SetOrtho(true);
		AddChild(pEntity);
	}
	{
		Entity *pEntity = new Entity();
		Entity::CreateTextImageComponent(pEntity, "", 32);
		auto pText = (TextImageComponent *) pEntity->GetComponent(eComponentKind_Layout);
		char pointStr[64];
		std::snprintf(pointStr, sizeof(pointStr), "%d", AppParam::Get()->GetKizunaPoint());
		pointStr[sizeof(pointStr) - 1] = '\0';
		pText->SetText(pointStr);
		const int nLength = strlen(pointStr);
		// TODO 内部的にセンタリングや左揃え指定出来るようにする
		pEntity->SetPosition(200.0f - ((float)nLength * 0.5f) * 16.0f, 0.0f, 0);
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