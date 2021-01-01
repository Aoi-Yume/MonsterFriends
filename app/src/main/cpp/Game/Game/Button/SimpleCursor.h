//
// Created by 葵ユメ on 2018/09/22.
//


#ifndef AOIYUME_SIMPLE_CURSOR_H
#define AOIYUME_SIMPLE_CURSOR_H

#include "Entity.h"

class Collision2DRectComponent;
class LayoutComponent;
class ButtonManager;

class SimpleCursor : public Entity
{
	typedef Entity Super;

public:
	SimpleCursor(const char* pResName, ButtonManager* pManager);
	virtual ~SimpleCursor();

public:
	void EntityUpdate(GameMessage message, const void* param) override;

private:
	LayoutComponent*	m_pLayoutComponent;
	ButtonManager*		m_pButtonManager;
};

#endif