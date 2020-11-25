//
// Created by 葵ユメ on 2020/11/23.
//


#ifndef AOIYUME_MESSAGE_WINDOW_H
#define AOIYUME_MESSAGE_WINDOW_H

#include "GameEntity.h"

class LayoutComponent;
class TextImageComponent;

class MessageWindow : public GameEntity
{
	typedef GameEntity Super;
public:
	enum Child {
		eChild_BackImage,
		eChild_TextImage,
		eChild_Max
	};

	enum {
		eLINE_MAX = 10
	};

public:
	MessageWindow(const char* pResName);
	virtual ~MessageWindow();

public:
	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	void SetVisible(bool bVisible);

	void SetTextScale(float fScale);
	void SetMessage(const char* pLabel);
	void SetDirectMessage(const char* pText);

	bool IsNextMessage() const;

private:
	void clearNextMessageFlag();
	void clearText();
	void updatePosition();

private:
	bool 			m_bNextMessage;
	int 			m_nNextMessageCnt;
	int 			m_nCurrentUseLine;
	char 			m_cResPath[64];
	float 			m_fTextScale;
	LayoutComponent*	m_pLayoutComponent;
	TextImageComponent*	m_pTextComponent[eLINE_MAX];
};

#endif