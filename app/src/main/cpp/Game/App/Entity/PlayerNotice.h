//
// Created by 葵ユメ on 2021/03/13.
//


#ifndef AOIYUME_APP_PLAYER_NOTICE_H
#define AOIYUME_APP_PLAYER_NOTICE_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "GameEntity.h"

class PlayerNotice : public GameEntity
{
	typedef GameEntity Super;

public:
	enum NoticeType{
		eNoticeType_CurrentPlayer,
		eNoticeType_Winner,
		eNoticeType_Max
	};

	enum State {
		eState_Idle,
		eState_Open,
		eState_Wait,
		eState_Close,
		eState_Max
	};

public:
	PlayerNotice(NoticeType nType, int nNoticePlayer);
	virtual ~PlayerNotice();

	void GameEntitySetup(const void* param) override;
	void GameEntityUpdate(const void* param) override;
	void EntityUpdate(GameMessage message, const void* param) override;

public:
	void Open(bool bAutoClose, float fDelay = 0.0f);
	void Close();

private:
	NoticeType	m_nType;
	State		m_State;
	int			m_nNoticePlayer;
	float_t 	m_fDelay;
	bool 		m_bAutoClose;
};

#endif