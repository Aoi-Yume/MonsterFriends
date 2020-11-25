﻿//
// Created by 葵ユメ on 2018/09/22.
//


#include <LayoutComponent.h>
#include <CollisionComponent.h>
#include <TransformComponent.h>
#include <CameraComponent.h>
#include "SimpleCursor.h"

SimpleCursor::SimpleCursor(const char* pResName)
: Super()
, m_pLayoutComponent(nullptr)
{
#if 0
	{
		Entity *pEntity = new Entity();
		Entity::CreateLayoutComponent(pEntity, pResName);
		AddChild(pEntity);

		m_pLayoutComponent = (LayoutComponent *) pEntity->GetComponent(eComponentKind_Layout);
		m_pLayoutComponent->SetOrtho(true);
	}
#endif
	Entity::CreateTransformComponent(this);
	Entity::CreateCollision2DRectComponent(this);
	auto pColl = (Collision2DRectComponent *)GetComponent(eComponentKind_Collision);
	auto pTransform = (TransformComponent*)GetComponent(eComponentKind_Transform);
	pColl->SetSize(10.0f, 10.0);
	pColl->SetTransform(pTransform->GetLocalMatrix());
}

SimpleCursor::~SimpleCursor()
{
}

void SimpleCursor::EntityUpdate(GameMessage message, const void *param) {
	if(message == eGameMessage_Update){
		const float fTouchX = Engine::GetEngine()->GetTouchInputInfo().m_fTouchX;
		const float fTouchY = Engine::GetEngine()->GetTouchInputInfo().m_fTouchY;
		const float fScreenX = Engine::GetEngine()->GetScreenInfo().m_nScreenX;
		const float fScreenY = Engine::GetEngine()->GetScreenInfo().m_nScreenY;
		auto pCamera = (CameraComponent *) Engine::GetEngine()->GetCameraComponent();
		VEC3 projPos = pCamera->ScreentoProjection(fTouchX, fTouchY);
		VEC3 pos = VEC3(projPos.GetX() * fScreenX * 0.5f, projPos.GetY() * fScreenY * 0.5f, 0.0f);
		SetPosition(pos.GetX(), pos.GetY(), 0.0f);
	}

	EntityBase::EntityUpdate(message, param);
}


