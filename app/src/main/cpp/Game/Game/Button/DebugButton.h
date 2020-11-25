//
// Created by 葵ユメ on 2018/09/22.
//


#ifndef AOIYUME_DEBUG_BUTTON_H
#define AOIYUME_DEBUG_BUTTON_H

#include "Entity.h"
#include "ButtonBase.h"

class Collision2DRectComponent;
class DebugSquareImageComponent;
class TextImageComponent;

class DebugButton : public ButtonBase
{
	typedef ButtonBase Super;
public:
	enum Child {
		eChild_DebugSquareImage,
		eChild_TextImage,
		eChild_Max
	};
public:
	DebugButton();
	virtual ~DebugButton();

public:
	void SetText(const char* pText);
	void SetFontColor(const VEC4& color);
	void SetBackColor(const VEC4& unSelectColor, const VEC4& selectColor);
	void SetSize(float fSizeW, float fSizeH) override;

	void Select() override;
	void UnSelect() override;

protected:
	virtual void ChangeColor() override;

private:
	VEC4	m_unSelectColor;
	VEC4	m_selectColor;

	DebugSquareImageComponent* 		m_pDebugSqauareComponent;
	TextImageComponent*				m_pTextImageComponent;
};

#endif