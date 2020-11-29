//
// Created by 葵ユメ on 2020/11/29.
//


#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "CollisionComponent.h"
#include "Character.h"

Character::Character(CHARA_ID charaId)
: GameEntity()
, m_nCharaId(charaId)
{
	DEBUG_LOG("Create Character");
}

Character::~Character()
{
}

void Character::GameEntitySetup(const void* param)
{
	Super::GameEntitySetup(param);

	char imagePath[64];
	snprintf(imagePath, sizeof(imagePath), "image/chara_%02d.png", m_nCharaId + 1);
	imagePath[sizeof(imagePath) - 1] = '\0';
	Entity::CreateLayoutComponent(this, imagePath);
	reinterpret_cast<LayoutComponent*>(GetComponent(eComponentKind_Layout))->SetOrtho(true);
}

void Character::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);
}

void Character::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}