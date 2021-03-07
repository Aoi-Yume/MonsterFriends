//
// Created by 葵ユメ on 2018/09/22.
//


#ifndef AOIYUME_BUTTON_BASE_H
#define AOIYUME_BUTTON_BASE_H

#include "Entity.h"

class Collision2DRectComponent;

class ButtonBase : public Entity
{
	typedef Entity Super;
public:
	enum State{
		eState_UnSelect,
		eState_Select,
		eState_Selected,
		eState_Max
	};
	enum Child {
		eChild_DebugSquareImage,
		eChild_TextImage,
		eChild_Max
	};
public:
	ButtonBase();
	virtual ~ButtonBase();

public:
	virtual void SetSize(float fSizeW, float fSizeH);
	virtual void Select();
	virtual void UnSelect();

	void Enable();
	void Disable();
	bool IsEnable() const;

	void SetVisible(bool bVisible);

	bool IsSelect() const;
	bool IsUnSelect() const;
	bool IsSelected() const;

	void SetGray(bool bGray);
	bool IsGray() const;

protected:
	virtual void ChangeColor();

private:
	bool 	m_bEnable;
	bool 	m_bGray;
	int		m_nState;

	Collision2DRectComponent*		m_pColl2DRectComponent;
};

#endif