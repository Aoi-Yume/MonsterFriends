//
// Created by 葵ユメ on 2018/08/25.
//


#include "Entity.h"

Entity::Entity()
: EntityBase()
{
}

Entity::~Entity()
{
}

void Entity::SetPosition(float x, float y, float z)
{
	auto pTransform = GetComponent<TransformComponent*>(eComponentKind_Transform);
	ASSERT(pTransform);
	pTransform->SetTranslate(VEC3(x, y, z));
}

void Entity::SetPosition(const VEC3& pos)
{
	auto pTransform = GetComponent<TransformComponent*>(eComponentKind_Transform);
	ASSERT(pTransform);
	pTransform->SetTranslate(pos);
}

VEC3 Entity::GetPosition() const
{
	auto pTransform = GetComponent<TransformComponent*>(eComponentKind_Transform);
	ASSERT(pTransform);
	return pTransform->GetTranslate();
}

void Entity::SetRotate(float x, float y, float z)
{
	auto pTransform = GetComponent<TransformComponent*>(eComponentKind_Transform);
	ASSERT(pTransform);
	pTransform->SetRotate(VEC3(x, y, z));
}

void Entity::SetRotate(const VEC3 &rot)
{
	auto pTransform = GetComponent<TransformComponent*>(eComponentKind_Transform);
	ASSERT(pTransform);
	pTransform->SetRotate(rot);
}

VEC3 Entity::GetRotate() const
{
	auto pTransform = GetComponent<TransformComponent*>(eComponentKind_Transform);
	ASSERT(pTransform);
	return pTransform->GetRotate();
}

void Entity::SetScale(float x, float y, float z)
{
	auto pTransform = GetComponent<TransformComponent*>(eComponentKind_Transform);
	ASSERT(pTransform);
	pTransform->SetScale(VEC3(x, y, z));
}

void Entity::SetScale(const VEC3& scale)
{
	auto pTransform = GetComponent<TransformComponent*>(eComponentKind_Transform);
	ASSERT(pTransform);
	pTransform->SetScale(scale);
}

VEC3 Entity::GetScale() const
{
	auto pTransform = GetComponent<TransformComponent*>(eComponentKind_Transform);
	ASSERT(pTransform);
	return pTransform->GetScale();
}

void Entity::SetVisible(bool bVisible)
{
	auto pDrawComponent = GetComponent<DrawComponent*>(eComponentKind_Layout);
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
	auto pDrawComponent = GetComponent<DrawComponent*>(eComponentKind_Layout);
	if(pDrawComponent){
		return pDrawComponent->IsVisible();
	}
	return false;
}

bool Entity::CreateTextImageComponent(Entity* pEntity, const char* pText, int nFontSize)
{
	CreateTransformComponent(pEntity);
	if( pEntity->m_pComponent[eComponentKind_Layout] ){ return false; }
	auto pComponent = new TextImageComponent(pEntity);
	pComponent->SetText(pText);
	pComponent->SetFontSize(nFontSize);

	pEntity->m_pComponent[eComponentKind_Layout] = pComponent;
	return true;
}

LayoutComponent* Entity::CreateLayoutComponent(Entity* pEntity, const char* pResPath)
{
    CreateTransformComponent(pEntity);

	if( pEntity->m_pComponent[eComponentKind_Layout] ){
		return reinterpret_cast<LayoutComponent*>(pEntity->m_pComponent[eComponentKind_Layout]);
	}

	auto pComponent = new LayoutComponent(pEntity);
	pComponent->SetResPath(pResPath);
	
	pEntity->m_pComponent[eComponentKind_Layout] = pComponent;
	return pComponent;
}

bool Entity::CreateTransformComponent(Entity* pEntity)
{
	if(  pEntity->m_pComponent[eComponentKind_Transform] ) {
		return false;
	}
	pEntity->m_pComponent[eComponentKind_Transform] = new TransformComponent(pEntity);
	return true;
}

bool Entity::CreateCollision2DRectComponent(Entity *pEntity)
{
	if(  pEntity->m_pComponent[eComponentKind_Collision] ) {
		return false;
	}
	pEntity->m_pComponent[eComponentKind_Collision] = new Collision2DRectComponent(pEntity);
	return true;
}

bool Entity::CreateCameraComponent(Entity *pEntity)
{
    if( pEntity->m_pComponent[eComponentKind_Camera] ) {
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
	CreateTransformComponent(pEntity);
	if( pEntity->m_pComponent[eComponentKind_Layout] ){ return false; }
	auto pComponent = new DebugSquareImageComponent(pEntity);

	pEntity->m_pComponent[eComponentKind_Layout] = pComponent;
	return true;
}