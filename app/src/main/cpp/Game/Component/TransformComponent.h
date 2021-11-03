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
	void SetAnimScale(const VEC3& scale);
	void SetAnimRotate(const VEC3& rotate);
    void SetAnimTranslate(const VEC3& translate);
	
	const VEC3& GetScale() const;
	const VEC3& GetRotate() const;
	const VEC3& GetTranslate() const;
	const VEC3& GetAnimScale() const;
	const VEC3& GetAnimRotate() const;
	const VEC3& GetAnimTranslate() const;

	void CalcLocalMatrix();

	const MAT4* GetLocalMatrix();
	const MAT4* GetWorldMatrix();

	virtual GameMessageResult Update(GameMessage message, const void* param) override;

private:
	MAT4 getParentMatrix();
private:
	bool 		m_bBusy;
	VEC3		m_scale;
	VEC3		m_rotate;
	VEC3		m_translate;
	VEC3		m_AnimScale;
	VEC3		m_AnimRotate;
	VEC3		m_AnimTranslate;
	MAT4		m_transform;
	MAT4		m_worldTransform;
};

#endif