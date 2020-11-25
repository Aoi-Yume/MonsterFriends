//
// Created by 葵ユメ on 2018/09/22.
//


#ifndef AOIYUME_SIMPLE_BUTTON_H
#define AOIYUME_SIMPLE_BUTTON_H

#include "Entity.h"
#include "ButtonBase.h"

class Collision2DRectComponent;
class LayoutComponent;

class SimpleButton : public ButtonBase
{
	typedef ButtonBase Super;
public:
	enum Child {
		eChild_DebugSquareImage,
		eChild_TextImage,
		eChild_Max
	};
public:
	SimpleButton(const char* pResName);
	virtual ~SimpleButton();

public:
	void SetSize(float fSizeW, float fSizeH) override;
	
	void Select() override;
	void UnSelect() override;

private:
	LayoutComponent*	m_pLayoutComponent;
};

#endif