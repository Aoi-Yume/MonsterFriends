//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_CAMERA_COMPONENT_H
#define AOIYUME_CAMERA_COMPONENT_H

#include "../Engine/Engine.h"
#include "../../Math/math_Vector.h"
#include "../../Math/math_Matrix.h"
#include "ComponentBase.h"


class CameraComponent : public ComponentBase
{
    typedef ComponentBase Super;
public:
	CameraComponent(EntityBase* pEntityBase);
	virtual ~CameraComponent();

	GameMessageResult Update(GameMessage message, const void* param) override;

	const MAT4& GetView() const;
	const MAT4& GetViewInv() const;
	const MAT4&	 GetProjectionInv() const;
	const MAT4& GetViewProjection() const;
	const MAT4& GetViewProjectionInv() const;
	const MAT4& GetViewOrtho() const;
	const MAT4& Get2D() const;
	const MAT4& GetViewport() const;
	const MAT4& GetViewportInv() const;

	const VEC3 ScreentoProjection(float fSx, float fSY) const;
	const VEC3 ProjectiontoWorld(const VEC3& projPos, const float fZRatio) const;
private:
	float 		m_nCnt;

	float 		m_fNear;
	float 		m_fFar;

	MAT4		m_camView;
	MAT4		m_camViewInv;
	MAT4		m_camProj;
	MAT4		m_camProjInv;
	MAT4		m_camOrtho;
	MAT4		m_cam2D;
	MAT4		m_camViewProj;
	MAT4		m_camViewProjInv;
	MAT4		m_camViewOrtho;
	MAT4		m_camViewport;
	MAT4		m_camViewportInv;
};

#endif