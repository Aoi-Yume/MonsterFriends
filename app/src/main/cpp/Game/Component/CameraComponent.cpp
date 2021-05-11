//
// Created by 葵ユメ on 2018/08/25.
//


#include <stdio.h>
#include "CameraComponent.h"

CameraComponent::CameraComponent(EntityBase* pEntityBase)
: Super(eComponentKind_Camera, pEntityBase)
, m_nCnt(0.0f)
, m_fNear(0.1f)
, m_fFar(100.0f)
, m_camProj()
, m_cam2D()
{
    DEBUG_LOG("Call CameraComponent Constructor");
	Engine::GetEngine()->SetCameraComponent(this);
}

CameraComponent::~CameraComponent()
{
    DEBUG_LOG("Call CameraComponent Deconstructor");
}

GameMessageResult CameraComponent::Update(GameMessage message, const void* param)
{
    if(message == eGameMessage_ChangeCamera) {
		DEBUG_LOG("Call ChangeCamera");

		VEC3 camPos = VEC3(0.0f, 0.0f, 5.0f);
		VEC3 eyePos = VEC3(0, 0, 0);
		VEC3 upY = VEC3(0, 1, 0);
		m_camView.SetView(camPos, eyePos, upY);
		m_camViewInv = m_camView;
		m_camViewInv.SetInverse();

		float fAspect = Engine::GetEngine()->GetScreenInfo().m_fAspect;
		m_camProj.SetPerspective(DEGTORAD(20.0f), fAspect, m_fNear, m_fFar);
		m_camProjInv = m_camProj;
		m_camProjInv.SetInverse();

		m_camViewProj = m_camProj * m_camView;
		m_camViewProjInv = m_camProjInv * m_camViewInv;

		const float fScreenW = (float)Engine::GetEngine()->GetScreenInfo().m_nScreenX;
		const float fScreenH = (float)Engine::GetEngine()->GetScreenInfo().m_nScreenY;
		m_camOrtho.SetOrtho(0, fScreenW, 0, fScreenH, m_fNear, m_fFar);

		m_camViewOrtho = m_camOrtho * m_camView;

		{
			m_camViewport.SetScale(fScreenW * 0.5f, fScreenH * 0.5f, 1.0f);
			//m_camViewport = translate * m_camViewport;
			m_camViewportInv = m_camViewport;
			m_camViewportInv.SetInverse();
		}
		{
			const float fScreenX = (float)Engine::GetEngine()->GetScreenInfo().m_nScreenX;
			const float fScreenY = (float)Engine::GetEngine()->GetScreenInfo().m_nScreenY;
			const float fFixDisplaySizeX = 1920.0f / (float)fScreenX;
			const float fFixDisplaySizeY = 1080.0f / (float)fScreenY;
			m_cam2D.SetScale(1.0f / (fScreenW * fFixDisplaySizeX * 0.5f), 1.0f / (fScreenH * fFixDisplaySizeY * 0.5f), 1.0f);
		}
    }
    return eGameMessageResult_Break;
}

const MAT4& CameraComponent::GetView() const
{
	return m_camView;
}

const MAT4& CameraComponent::GetViewInv() const
{
	return m_camViewInv;
}

const MAT4& CameraComponent::GetProjectionInv() const
{
	return m_camProjInv;
}

const MAT4& CameraComponent::GetViewProjection() const
{
    return m_camViewProj;
}

const MAT4& CameraComponent::GetViewProjectionInv() const
{
	return m_camViewProjInv;
}

const MAT4& CameraComponent::GetViewOrtho() const
{
	return m_camViewOrtho;
}

const MAT4& CameraComponent::Get2D() const
{
	return  m_cam2D;
}

const MAT4& CameraComponent::GetViewport() const
{
	return m_camViewport;
}

const MAT4& CameraComponent::GetViewportInv() const
{
	return m_camViewportInv;
}

const VEC3 CameraComponent::ScreentoProjection(float fSx, float fSY) const
{
	const float fScreenW = (float)Engine::GetEngine()->GetScreenInfo().m_nScreenX;
	const float fScreenH = (float)Engine::GetEngine()->GetScreenInfo().m_nScreenY;
	VEC3 ret;
	ret.SetX((fSx - fScreenW * 0.5f) / (fScreenW * 0.5f));
	ret.SetY((fSY - fScreenH * 0.5f) / (fScreenH * 0.5f));
	return ret;
}

const VEC3 CameraComponent::ProjectiontoWorld(const VEC3 &projPos, const float fZRatio) const
{
	MAT4 vpInv = GetViewProjectionInv();
	VEC4 wPos = vpInv * VEC3(projPos.GetX(), projPos.GetY(), fZRatio);
	wPos *= wPos.GetW();
	wPos.SetX(-wPos.GetX());
	wPos.SetY(-wPos.GetY());
	return VEC3(wPos.GetX(), wPos.GetY(), wPos.GetZ());
}