//
// Created by 葵ユメ on 2021/01/10.
//


#ifndef AOIYUME_STATE_MANAGER_H
#define AOIYUME_STATE_MANAGER_H

#include <vector>

class StateManager;
class StateBase
{
	friend class StateManager;
public:
	StateBase()
		: m_pStateManager(nullptr)
	{}
	virtual ~StateBase(){}
	
	virtual void Begin(void * pUserPtr) {}
	virtual void Update(void * pUserPtr) {}
	virtual void End(void * pUserPtr) {}

	void ChangeState(int nState);
private:
	StateManager* 	m_pStateManager;
};

class StateManager
{
public:
	StateManager(int nReserve = 10);
	virtual ~StateManager();

	void Update();
	void ChangeState(int nState);
	void SetUserPtr(void* pPtr);

	int GetStateCount() const { return m_nStateCount; }

	template<class T>
	T* CreateState(){
		auto pState = new T;
		((StateBase*)pState)->m_pStateManager = this;
		m_aState.push_back(pState);
		return pState;
	}
	
	template<class T>
	T* GetState(int nState) {
		return (T*)m_aState[nState];
	}
	
private:
	bool m_bStart;
	int m_nState;
	int m_nNextState;
	int m_nStateCount;
	std::vector<StateBase*>	m_aState;
	void* m_pUserPtr;
};

#endif