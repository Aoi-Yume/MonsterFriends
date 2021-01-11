//
// Created by 葵ユメ on 2021/01/10.
//


#include "State.h"

void StateBase::ChangeState(int nState)
{
	m_pStateManager->ChangeState(nState);
}

StateManager::StateManager(int nReserve)
	: m_bStart(true)
	, m_nState(0)
	, m_nNextState(0)
	, m_nStateCount(0)
{

}

StateManager::~StateManager()
{
	for(auto& it : m_aState){
		delete it;
	}
	m_aState.clear();
}

void StateManager::Update()
{
	if(m_bStart){
		m_aState[m_nState]->Begin(m_pUserPtr);
		m_bStart = false;
	}
	else {
		if (m_nNextState != m_nState) {
			m_aState[m_nState]->End(m_pUserPtr);
			m_nState = m_nNextState;
			m_nStateCount = 0;
			m_aState[m_nState]->Begin(m_pUserPtr);
		} else {
			m_aState[m_nState]->Update(m_pUserPtr);
			m_nStateCount++;
		}
	}
}

void StateManager::ChangeState(int nState)
{
	m_nNextState = nState;
}

void StateManager::SetUserPtr(void *pPtr)
{
	m_pUserPtr = pPtr;
}