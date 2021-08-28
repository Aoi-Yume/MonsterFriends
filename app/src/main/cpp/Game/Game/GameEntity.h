//
// Created by 葵ユメ on 2018/11/18
//


#ifndef AOIYUME_GAME_ENTITY_H
#define AOIYUME_GAME_ENTITY_H

#include "Entity.h"

class ComponentBase;

class GameEntity : public Entity
{
public:
	GameEntity();
	virtual ~GameEntity();

public:
	virtual void GameEntitySetup(const void* param);
	virtual void GameEntityUpdate(const void* param);
	virtual void EntityUpdate(GameMessage message, const void* param) override;
	
public:
	void SetActive(bool bActive){ m_bActive = bActive; }
	bool IsActive() const { return m_bActive; }
	void SetState(int nState);
	void SetNextState(int nState);
	void StateUpdate();
	
	int	GetState() const;
	int GetNextState() const;
	int GetStateCount() const;
	float GetStateTime() const;

private:
	bool 	m_bActive;
	int		m_nState;
	int		m_nNextState;
	int		m_nStateCnt;
	float	m_fStateTime;
};

#endif