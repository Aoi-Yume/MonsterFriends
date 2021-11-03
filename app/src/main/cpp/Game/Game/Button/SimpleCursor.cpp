//
// Created by 葵ユメ on 2018/09/22.
//


#include <LayoutComponent.h>
#include <CollisionComponent.h>
#include <TransformComponent.h>
#include <CameraComponent.h>
#include <Button/ButtonManager.h>
#include <DelayInput.h>
#include "SimpleCursor.h"

SimpleCursor::SimpleCursor(const char* pResName, ButtonManager* pManager)
: Super()
, m_pLayoutComponent(nullptr)
, m_pButtonManager(pManager)
{
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
		const auto pEngine = Engine::GetEngine();
		const int nPlayerId = m_pButtonManager->GetControlPlayerId();
		TouchInputInfo info = {};
		if(!pEngine->FindDelayTouchInfo(info, eTouchEvent_DOWN, nPlayerId)){
			pEngine->FindDelayTouchInfo(info, eTouchEvent_MOVE, nPlayerId);
		}
		const float fTouchX = info.fTouchX;
		const float fTouchY = info.fTouchY;
		const float fScreenX = Engine::GetEngine()->GetScreenInfo().m_nScreenX;
		const float fScreenY = Engine::GetEngine()->GetScreenInfo().m_nScreenY;
		const float fFixDisplaySizeX = 1920.0f / (float) fScreenX;
		const float fFixDisplaySizeY = 1080.0f / (float) fScreenY;
		auto pCamera = (CameraComponent *) Engine::GetEngine()->GetCameraComponent();
		VEC3 projPos = pCamera->ScreentoProjection(fTouchX, fTouchY);
		VEC3 pos = VEC3(projPos.X() * fFixDisplaySizeX * fScreenX * 0.5f, projPos.Y() * fFixDisplaySizeX * fScreenY * 0.5f, 0.0f);
		SetPosition(pos.X(), pos.Y(), 0.0f);
	}

	EntityBase::EntityUpdate(message, param);
}


