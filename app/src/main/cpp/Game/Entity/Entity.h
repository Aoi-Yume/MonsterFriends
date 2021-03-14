//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_ENTITY_H
#define AOIYUME_ENTITY_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "EntityBase.h"
#include "../../Math/math_Vector.h"

class ComponentBase;
class LayoutComponent;
class AnimationComponent;

class Entity : public EntityBase
{
public:
	Entity();
	virtual ~Entity();

public:
	void SetPosition(float x, float y, float z);
	void SetPosition(const VEC3& pos);
	const VEC3 GetPosition();

	void SetRotate(float x, float y, float z);
	void SetRotate(const VEC3& rot);

	void SetScale(float x, float y, float z);
	void SetScale(const VEC3& scale);
	const VEC3 GetScale();

	void SetVisible(bool bVisible);
	bool IsVisible();
public:
	static bool CreateTextImageComponent(Entity* pEntity, const char* pText, int nFontSize);
	static LayoutComponent* CreateLayoutComponent(Entity* pEntity, const char* pResName);
	static bool CreateTransformComponent(Entity* pEntity);
	static bool CreateCollision2DRectComponent(Entity* pEntity);
    static bool CreateCameraComponent(Entity* pEntity);
    static AnimationComponent* CreateAnimationComponent(Entity* pEntity);

	static bool CreateDebugSquareImageComponent(Entity* pEntity);
};

#endif