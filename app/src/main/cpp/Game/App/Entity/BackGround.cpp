//
// Created by 葵ユメ on 2021/05/30.
//


#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "BAckGround.h"

BackGround::BackGround()
: GameEntity()
{
	DEBUG_LOG("Create BackGround");
}

BackGround::~BackGround()
{
}

void BackGround::GameEntitySetup(const void* param)
{
	Entity::CreateTransformComponent(this);
	Super::GameEntitySetup(param);

	auto pComponent = Entity::CreateLayoutComponent(this, "image/sky_loop.png");
	pComponent->SetOrtho(true);
	pComponent->SetAnimation(true);
	{
		auto pChild = new Entity();
		Entity::CreateLayoutComponent(pChild, "image/background.png")->SetOrtho(true);
		AddChild(pChild);
	}
}

void BackGround::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);

	auto lyt = GetComponent<LayoutComponent*>(eComponentKind_Layout);
	lyt->AddUVOffset(VEC2(0.0001f, 0));
}

void BackGround::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}