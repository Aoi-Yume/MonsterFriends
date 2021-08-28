//
// Created by 葵ユメ on 2018/11/18.
//


#include "GameEntity.h"

GameEntity::GameEntity()
: Entity()
, m_bActive(true)
, m_nState(0)
, m_nNextState(0)
, m_nStateCnt(0)
, m_fStateTime(0.0f)
{
}

GameEntity::~GameEntity()
{
}

void GameEntity::GameEntitySetup(const void* param)
{
	(void)param;
}

void GameEntity::GameEntityUpdate(const void* param)
{
	(void)param;
	StateUpdate();
}

void GameEntity::EntityUpdate(GameMessage message, const void* param)
{
	if(message == eGameMessage_Setup){
		GameEntitySetup(param);
	}
	else if(message == eGameMessage_Update){
		if(!IsActive()){ return; }
		GameEntityUpdate(param);
	}
	else{
		Entity::EntityUpdate(message, param);
	}
}

void GameEntity::SetState(int nState)
{
	m_nState = nState;
	m_nNextState = nState;
	m_nStateCnt = 0;
}

void GameEntity::SetNextState(int nState)
{
	m_nNextState = nState;
}

void GameEntity::StateUpdate()
{
	if(m_nNextState != m_nState){
		m_nState = m_nNextState;
		m_nStateCnt = 0;
		m_fStateTime = 0.0f;
	}
	else{
		m_nStateCnt++;
		m_fStateTime += Engine::GetEngine()->GetDeltaTime();
	}
}
	
int	GameEntity::GetState() const
{
	return m_nState;
}

int GameEntity::GetNextState() const
{
	return m_nNextState;
}

int GameEntity::GetStateCount() const
{
	return m_nStateCnt;
}

float GameEntity::GetStateTime() const
{
	return m_fStateTime;
}