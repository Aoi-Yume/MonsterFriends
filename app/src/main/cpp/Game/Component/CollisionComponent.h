//
// Created by 葵ユメ on 2018/09/22.
//


#ifndef AOIYUME_COLLISION_COMPONENT_H
#define AOIYUME_COLLISION_COMPONENT_H

#include "../Engine/Engine.h"
#include "../../Math/math_Vector.h"
#include "../../Math/math_Matrix.h"
#include "ComponentBase.h"

struct CollisionParam_2DRect{
	VEC4	m_Rect;
	
	CollisionParam_2DRect(float w, float h)
	: m_Rect(-w * 0.5f, h * 0.5f, w * 0.5f, -h * 0.5f)
	{
	}
};

struct HitCollisionParam{
	int		m_nCollisionTag;
	VEC3	m_ReboundVec;

	HitCollisionParam(int nCollisionTag, const VEC3& reboundVec)
	: m_nCollisionTag(nCollisionTag)
	, m_ReboundVec(reboundVec)
	{
	}
};

typedef bool(*CollisionIntersectFunc)(const void*, const void*, const MAT4*, const MAT4*);

class CollisionComponent : public ComponentBase
{
    typedef ComponentBase Super;
public:
	enum CollisionKind{
		eCollisionKind_2DRect,
		eCollisionKind_Max
	};
public:
	CollisionComponent(EntityBase* pEntityBase);
	virtual ~CollisionComponent();

	void SetTransform(const MAT4* pTransform);
	const MAT4* GetTransform() const;

	void SetActive(bool bActive);
	bool IsActive() const;
	void SetCollisionTag(int nCollisionTag);
	int GetCollisionTag() const;
	
	bool IsIntersect(const CollisionComponent* pComponent) const;

public:
	static bool IsIntersect2DRect2DRect(const void* p1, const void* p2, const MAT4* p3, const MAT4* p4);

protected:
	void SetParam(void* pParam);

private:
	bool 			m_bActive;
	int 			m_nCollisionTag;
	int				m_nCollisionKind;
	const MAT4*		m_pTransform;
	void*			m_pParam;
};

class Collision2DRectComponent : public CollisionComponent
{
	typedef CollisionComponent Super;
public:
	Collision2DRectComponent(EntityBase* pEntityBase);
	virtual ~Collision2DRectComponent();

public:
	void SetSize(float fSizeW, float fSizeH);

private:
	CollisionParam_2DRect	m_rectParam;
};

#endif