//
// Created by 葵ユメ on 2021/01/01.
//


#include "DelayInput.h"
#include "../aoiyume_define.h"

USE_SINGLETON_VARIABLE(DelayInput)

DelayInput::DelayInput()
	: m_uState(0U)
	, m_fDelay(0.0f)
	, m_fCurrentTime(0.0f)
	, m_FLastDeltaTime(0.0f)
	, m_aTouchInputInfo()
{
}

DelayInput::~DelayInput()
{
}

void DelayInput::Initialize(float fDelay)
{
	m_fDelay = fDelay;
}

void DelayInput::Update(float fDeltaTime)
{
	if((m_uState & eSTATE_START) == 0U){ return; }
	if((m_uState & eSTATE_STOP) != 0U){ return; }

	for(auto& it : m_aTouchInputInfo){
		while(!it.empty()){
			if(it.front().fTime < m_fCurrentTime){
				it.pop_front();
			}
			else{
				break;
			}
		}
	}
	m_fCurrentTime += fDeltaTime;
	m_FLastDeltaTime = fDeltaTime;
}

void DelayInput::StartDelayInput()
{
	m_uState |= eSTATE_START;
	m_uState &= ~eSTATE_STOP;
}
void DelayInput::StopDelayInput()
{
	m_uState |= eSTATE_STOP;
}
void DelayInput::ResetDelayInput()
{
	m_fCurrentTime = 0;
	for(int i = 0; i < NET_CONNECT_MAX; ++i) {
		ClearTouchInfo(i);
	}
}

float DelayInput::GetCurrentTime() const
{
	return m_fCurrentTime;
}
void DelayInput::AddTouchInfo(const TouchInputInfo& info, int nPlayer)
{
	m_aTouchInputInfo[nPlayer].emplace_back(DelayTouchInfo(m_fCurrentTime + m_fDelay, info));
}
void DelayInput::AddDelayTouchInfo(const DelayTouchInfo& info, int nPlayer)
{
	float  fTime = std::max(info.fTime, m_fCurrentTime + (m_FLastDeltaTime * 1.2f));
	m_aTouchInputInfo[nPlayer].emplace_back(fTime, info.info);
	std::sort(m_aTouchInputInfo[nPlayer].begin(), m_aTouchInputInfo[nPlayer].end(),
			[](const DelayTouchInfo& a, const DelayTouchInfo& b){ return a.fTime < b.fTime; });
}
void DelayInput::ClearTouchInfo(int nPlayer)
{
	m_aTouchInputInfo[nPlayer].clear();
}

bool DelayInput::FindDelayTouchInfo(TouchInputInfo& info, int nEvent, int nPlayer) const
{
	if(!m_aTouchInputInfo[nPlayer].empty()) {
		for(auto& it : m_aTouchInputInfo[nPlayer]){
			if(it.fTime < m_fCurrentTime){
				if(it.info.nTouchEvent == nEvent){
					info = it.info;
					return true;
				}
			}
		}
	}
	return false;
}

DelayInput::DelayTouchInfo DelayInput::GetNextDelayTouchInfo(int nPlayer) const
{
	if(!m_aTouchInputInfo[nPlayer].empty()){
		return m_aTouchInputInfo[nPlayer].front();
	}
	return {};
}

bool DelayInput::IsInputEmpty(int nPlayer) const
{
	return m_aTouchInputInfo[nPlayer].empty();
}