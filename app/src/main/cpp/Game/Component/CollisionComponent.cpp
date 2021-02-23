//
// Created by 葵ユメ on 2018/09/22.
//


#include "CollisionComponent.h"
#include "CameraComponent.h"

//------------------------------------------
//------------------------------------------

static CollisionIntersectFunc s_CollisionFunc[CollisionComponent::eCollisionKind_Max][CollisionComponent::eCollisionKind_Max] =
{
	// eCollisionKind_2DRect
	{
		CollisionComponent::IsIntersect2DRect2DRect,		// eCollisionKind_2DRect
	},
};

//------------------------------------------
//------------------------------------------
CollisionComponent::CollisionComponent(EntityBase* pEntityBase)
: Super(eComponentKind_Collision, pEntityBase)
, m_bActive(true)
, m_nCollisionTag(-1)
, m_nCollisionKind(eCollisionKind_2DRect)
, m_pTransform(nullptr)
, m_pParam(nullptr)
{
//    DEBUG_LOG("Call CollisionComponent Constructor");
}

//------------------------------------------
//------------------------------------------
CollisionComponent::~CollisionComponent()
{
//	   DEBUG_LOG("Call CollisionComponent Deconstructor");
}

//------------------------------------------
//------------------------------------------
void CollisionComponent::SetParam(void* pParam)
{
	m_pParam = pParam;
}

//------------------------------------------
//------------------------------------------
void CollisionComponent::SetTransform(const MAT4* pTransform)
{
	m_pTransform = pTransform;
}

//------------------------------------------
//------------------------------------------
const MAT4* CollisionComponent::GetTransform() const
{
    return m_pTransform;
}

//------------------------------------------
//------------------------------------------
void CollisionComponent::SetActive(bool bActive)
{
	m_bActive = bActive;
}

//------------------------------------------
//------------------------------------------
bool CollisionComponent::IsActive() const
{
	return m_bActive;
}

//------------------------------------------
//------------------------------------------
void CollisionComponent::SetCollisionTag(int nCollisionTag)
{
	m_nCollisionTag = nCollisionTag;
}

//------------------------------------------
//------------------------------------------
int CollisionComponent::GetCollisionTag() const
{
	return m_nCollisionTag;
}

//------------------------------------------
//------------------------------------------
bool CollisionComponent::IsIntersect(const CollisionComponent* pComponent) const
{
	const int nCollKind = pComponent->m_nCollisionKind;
	const void* pParam = pComponent->m_pParam;

	if(!m_pParam || !pParam) { return false; }
	return s_CollisionFunc[m_nCollisionKind][nCollKind](m_pParam, pParam, m_pTransform, pComponent->m_pTransform);
}

//------------------------------------------
//------------------------------------------
bool CollisionComponent::IsIntersect2DRect2DRect(const void* p1, const void* p2, const MAT4* p3, const MAT4* p4)
{
	CameraComponent *pCamera = reinterpret_cast<CameraComponent*>(Engine::GetEngine()->GetCameraComponent());
	auto pCamMat = pCamera->GetViewOrtho();

	auto param1 = reinterpret_cast<const CollisionParam_2DRect*>(p1);
	auto param2 = reinterpret_cast<const CollisionParam_2DRect*>(p2);
	VEC3 pos1 = VEC3(*(p3->Get() + 12), *(p3->Get() + 13), *(p3->Get() + 14));
	VEC3 pos2 = VEC3(*(p4->Get() + 12), *(p4->Get() + 13), *(p4->Get() + 14));

	const float l1 = (pCamMat * VEC3(param1->m_Rect.GetX() + pos1.GetX(), 0, 0)).GetX();
	const float t1 = (pCamMat * VEC3(0, param1->m_Rect.GetY() + pos1.GetY(), 0)).GetY();
	const float r1 = (pCamMat * VEC3(param1->m_Rect.GetZ() + pos1.GetX(), 0, 0)).GetX();
	const float b1 = (pCamMat * VEC3(0, param1->m_Rect.GetW() + pos1.GetY(), 0)).GetY();
	
	const float l2 = (pCamMat * VEC3(param2->m_Rect.GetX() + pos2.GetX(), 0, 0)).GetX();
	const float t2 = (pCamMat * VEC3(0, param2->m_Rect.GetY() + pos2.GetY(), 0)).GetY();
	const float r2 = (pCamMat * VEC3(param2->m_Rect.GetZ() + pos2.GetX(), 0, 0)).GetX();
	const float b2 = (pCamMat * VEC3(0, param2->m_Rect.GetW() + pos2.GetY(), 0)).GetY();


#if 0
	DEBUG_LOG_A("[%.1f] <= [%.1f] && [%.1f] <= [%.1f] && [%.1f] >= [%.1f] && [%.1f] >= [%.1f] "
	, l1, r2, t1, b2, r1, l2 , b1, t2);
#endif

	return l1 <= r2 && t1 >= b2 && r1 >= l2 && b1 <= t2;
}

//------------------------------------------
//------------------------------------------
Collision2DRectComponent::Collision2DRectComponent(EntityBase* pEntityBase)
: Super(pEntityBase)
, m_rectParam(100, 100)
{
	SetParam(&m_rectParam);
}

//------------------------------------------
//------------------------------------------
Collision2DRectComponent::~Collision2DRectComponent()
{
}

//------------------------------------------
//------------------------------------------
void Collision2DRectComponent::SetSize(float fSizeW, float fSizeH)
{
	/*
	VEC3 rectSize(fSizeW, fSizeH, 0.0f);
	auto pCamera = (CameraComponent*)Engine::GetEngine()->GetCameraComponent();
	MAT4 viewOrtho = pCamera->GetViewOrtho();
	VEC4 orthoSize = viewOrtho * rectSize;
	orthoSize /= orthoSize.GetW();
	DEBUG_LOG_A("orthoSize:[%.1f, %.1f]", orthoSize.GetX(), orthoSize.GetY());
	 */
	m_rectParam = CollisionParam_2DRect(fSizeW, fSizeH);
}

