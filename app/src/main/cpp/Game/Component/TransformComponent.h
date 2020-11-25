//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_TRANSFORM_COMPONENT_H
#define AOIYUME_TRANSFORM_COMPONENT_H

#include "../Engine/Engine.h"
#include "../../Math/math_Vector.h"
#include "../../Math/math_Matrix.h"
#include "ComponentBase.h"


class TransformComponent : public ComponentBase
{
    typedef ComponentBase Super;
public:
	TransformComponent(EntityBase* pEntityBase);
	virtual ~TransformComponent();

	void SetScale(const VEC3& scale);
	void SetRotate(const VEC3& rotate);
    void SetTranslate(const VEC3& translate);
	
	VEC3 GetScale() const;
	VEC3 GetRotate() const;
	VEC3 GetTranslate() const;

	void CalcLocalMatrix();

	const MAT4* GetLocalMatrix() const;
	const MAT4* GetWorldMatrix();
private:
	MAT4 getParentMatrix();
private:
	VEC3		m_scale;
	VEC3		m_rotate;
	VEC3		m_translate;
	MAT4		m_transform;
	MAT4		m_worldTransform;
};

#endif