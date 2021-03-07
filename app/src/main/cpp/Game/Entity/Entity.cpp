//
// Created by 葵ユメ on 2018/08/25.
//


#include <CollisionComponent.h>
#include "Entity.h"
#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "CameraComponent.h"
#include "AnimationComponent.h"

Entity::Entity()
: EntityBase()
{
}

Entity::~Entity()
{
}

void Entity::SetPosition(float x, float y, float z)
{
	TransformComponent* pTransform = reinterpret_cast<TransformComponent*>(GetComponent(eComponentKind_Transform));
	pTransform->SetTranslate(VEC3(x, y, z));
}

void Entity::SetPosition(const VEC3& pos)
{
	TransformComponent* pTransform = reinterpret_cast<TransformComponent*>(GetComponent(eComponentKind_Transform));
	pTransform->SetTranslate(pos);
}

const VEC3 Entity::GetPosition()
{
	TransformComponent* pTransform = reinterpret_cast<TransformComponent*>(GetComponent(eComponentKind_Transform));
	return pTransform->GetTranslate();
}

void Entity::SetRotate(float x, float y, float z)
{
	TransformComponent* pTransform = reinterpret_cast<TransformComponent*>(GetComponent(eComponentKind_Transform));
	pTransform->SetRotate(VEC3(x, y, z));
}

void Entity::SetRotate(const VEC3 &rot)
{
	TransformComponent* pTransform = reinterpret_cast<TransformComponent*>(GetComponent(eComponentKind_Transform));
	pTransform->SetRotate(rot);
}

void Entity::SetScale(float x, float y, float z)
{
	TransformComponent* pTransform = reinterpret_cast<TransformComponent*>(GetComponent(eComponentKind_Transform));
	pTransform->SetScale(VEC3(x, y, z));
}

void Entity::SetScale(const VEC3& scale)
{
	TransformComponent* pTransform = reinterpret_cast<TransformComponent*>(GetComponent(eComponentKind_Transform));
	pTransform->SetScale(scale);
}

const VEC3 Entity::GetScale()
{
	TransformComponent* pTransofem = reinterpret_cast<TransformComponent*>(GetComponent(eComponentKind_Transform));
	return pTransofem->GetScale();
}

void Entity::SetVisible(bool bVisible)
{
	auto pDrawComponent = reinterpret_cast<DrawComponent*>(GetComponent(eComponentKind_Layout));
	if(pDrawComponent){
		pDrawComponent->SetVisible(bVisible);
	}
	for(auto& it : m_svpChild){
		auto pChild = reinterpret_cast<Entity*>(it);
		pChild->SetVisible(bVisible);
	}
}

bool Entity::IsVisible()
{
	auto pDrawComponent = reinterpret_cast<DrawComponent*>(GetComponent(eComponentKind_Layout));
	if(pDrawComponent){
		return pDrawComponent->IsVisible();
	}
	return false;
}

bool Entity::CreateTextImageComponent(Entity* pEntity, const char* pText, int nFontSize)
{
//	DEBUG_LOG("Create Text Image Component");
	CreateTransformComponent(pEntity);
	if( pEntity->m_pComponent[eComponentKind_Layout] ){ return false; }
	TextImageComponent* pComponent = new TextImageComponent(pEntity);
	pComponent->SetText(pText);
	pComponent->SetFontSize(nFontSize);

	pEntity->m_pComponent[eComponentKind_Layout] = pComponent;
	return true;
}

bool Entity::CreateLayoutComponent(Entity* pEntity, const char* pResPath)
{
//    DEBUG_LOG("Create Layout Component");
    CreateTransformComponent(pEntity);

	if( pEntity->m_pComponent[eComponentKind_Layout] ){ return false; }
	
	LayoutComponent* pComponent = new LayoutComponent(pEntity);
	pComponent->SetResPath(pResPath);
	
	pEntity->m_pComponent[eComponentKind_Layout] = pComponent;
	return true;
}

bool Entity::CreateTransformComponent(Entity* pEntity)
{
//	DEBUG_LOG("Create Transform Component");
	if(  pEntity->m_pComponent[eComponentKind_Transform] )
	{
//		DEBUG_LOG("Create Transform Component Filed");
		return false;
	}
	pEntity->m_pComponent[eComponentKind_Transform] = new TransformComponent(pEntity);
	return true;
}

bool Entity::CreateCollision2DRectComponent(Entity *pEntity)
{
//	DEBUG_LOG("Create Collision2DRect Component");
	if(  pEntity->m_pComponent[eComponentKind_Collision] )
	{
//		DEBUG_LOG("Create Collision2DRect Component Filed");
		return false;
	}
	pEntity->m_pComponent[eComponentKind_Collision] = new Collision2DRectComponent(pEntity);
	return true;
}

bool Entity::CreateCameraComponent(Entity *pEntity)
{
//    DEBUG_LOG("Create Camera Component");
    if( pEntity->m_pComponent[eComponentKind_Camera] )
    {
 //       DEBUG_LOG("Create Camera Component Filed");
        return false;
    }
    pEntity->m_pComponent[eComponentKind_Camera] = new CameraComponent(pEntity);
    return true;
}

AnimationComponent* Entity::CreateAnimationComponent(Entity* pEntity)
{
	CreateTransformComponent(pEntity);

	if(pEntity->m_pComponent[eComponentKind_Animation]){
		return reinterpret_cast<AnimationComponent*>(pEntity->m_pComponent[eComponentKind_Animation]);
	}
	pEntity->m_pComponent[eComponentKind_Animation] = new AnimationComponent(pEntity);
	return reinterpret_cast<AnimationComponent*>(pEntity->m_pComponent[eComponentKind_Animation]);
}

bool Entity::CreateDebugSquareImageComponent(Entity *pEntity)
{
//	DEBUG_LOG("Create Debug Square Image Component");
	CreateTransformComponent(pEntity);
	if( pEntity->m_pComponent[eComponentKind_Layout] ){ return false; }
	DebugSquareImageComponent* pComponent = new DebugSquareImageComponent(pEntity);

	pEntity->m_pComponent[eComponentKind_Layout] = pComponent;
	return true;
}