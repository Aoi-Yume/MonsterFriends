//
// Created by 葵ユメ on 2021/01/01.
//


#include "DelayInput.h"
USE_SINGLETON_VARIABLE(DelayInput)

DelayInput::DelayInput()
	: m_uState(0U)
	, m_nDelay(0U)
	, m_uCurrentFrame(0U)
	, m_aTouchInputInfo()
{
}

DelayInput::~DelayInput()
{
}

void DelayInput::Initialize(int nDelay)
{
	m_nDelay = nDelay;
}

void DelayInput::Update()
{
	if((m_uState & eSTATE_START) == 0U){ return; }
	if((m_uState & eSTATE_STOP) != 0U){ return; }

	for(auto& it : m_aTouchInputInfo){
		while(!it.empty()){
			if(it.front().uFrame < m_uCurrentFrame){
				it.pop_front();
			}
			else{
				break;
			}
		}
	}
	m_uCurrentFrame++;
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
	m_uCurrentFrame = 0;
	for(int i = 0; i < NET_CONNECT_MAX; ++i) {
		ClearTouchInfo(i);
	}
}

uint32_t DelayInput::GetCurrentFrame() const
{
	return m_uCurrentFrame;
}
void DelayInput::AddTouchInfo(const TouchInputInfo& info, int nPlayer)
{
	m_aTouchInputInfo[nPlayer].emplace_back(DelayTouchInfo(m_uCurrentFrame + m_nDelay, info));
}
void DelayInput::AddDelayTouchInfo(const DelayTouchInfo& info, int nPlayer)
{
	uint32_t  uFrane = std::max(info.uFrame, m_uCurrentFrame + 3);
	m_aTouchInputInfo[nPlayer].emplace_back(uFrane, info.info);
	std::sort(m_aTouchInputInfo[nPlayer].begin(), m_aTouchInputInfo[nPlayer].end(),
			[](const DelayTouchInfo& a, const DelayTouchInfo& b){ return a.uFrame < b.uFrame; });
}
void DelayInput::ClearTouchInfo(int nPlayer)
{
	m_aTouchInputInfo[nPlayer].clear();
}

bool DelayInput::FindDelayTouchInfo(TouchInputInfo& info, int nEvent, int nPlayer) const
{
	if(!m_aTouchInputInfo[nPlayer].empty()) {
		for(auto& it : m_aTouchInputInfo[nPlayer]){
			if(it.uFrame < m_uCurrentFrame){
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