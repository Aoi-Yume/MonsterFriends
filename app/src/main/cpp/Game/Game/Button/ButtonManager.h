//
// Created by 葵ユメ on 2018/10/06.
//


#ifndef AOIYUME_BUTTON_MANAGER_H
#define AOIYUME_BUTTON_MANAGER_H

#include "Entity.h"

class ButtonBase;
class DebugButton;
class SimpleButton;
class SimpleCUrsor;

class ButtonManager
{
public:
	ButtonManager();
	virtual ~ButtonManager();

public:
	SimpleButton*	CreateButton(const char* pResName);

	DebugButton*	CreateDebugButton(
			const char* pText, float fSizeW, float fSizeH,
			const VEC4& fontCol, const VEC4& unSelectCol, const VEC4& selectCol);

	ButtonBase* GetButton(int nIdx);

	void Lock();
	void Unlock();
	bool IsLock() const;

	void SetVisible(bool bVisible);

	void Reset();
	void SetControlPlayerId(int nPlayerId = -1);
	int GetControlPlayerId() const;

	int GetDecide() const;

	void Update(GameMessage message, const void* param);

	size_t GetButtonSize() const{ return m_aButtons.size(); }

private:
	bool		m_bLock;
	int 		m_nSelectNo;
	int 		m_nDecideNo;
	float 		m_fDecideDelay;
	int 		m_nControlPlayerId;
	Entity*		m_pCursorEntity;
	std::vector<ButtonBase*>	m_aButtons;
};

#endif