//
// Created by 葵ユメ on 2021/01/01.
//


#ifndef AOIYUME_DELAY_INPUT_H
#define AOIYUME_DELAY_INPUT_H

#include <deque>
#include <Singleton.h>
#include <engine_define.h>

struct TouchInputInfo{
	int		nPreTouchEvent;
	int		nTouchEvent;
	int 	nTouchCnt;
	float	fTouchX;
	float	fTouchY;

	TouchInputInfo()
			: nPreTouchEvent(0)
			, nTouchEvent(-1)
			, nTouchCnt(0)
			, fTouchX(0.0f)
			, fTouchY(0.0f)
	{
	}
};

class DelayInput : public Singleton<DelayInput>
{
	USE_SINGLETON(DelayInput);

public:
	struct DelayTouchInfo {
		uint32_t uFrame;
		TouchInputInfo info;

		DelayTouchInfo()
				: uFrame(0U)
				, info()
		{}

		DelayTouchInfo(uint32_t uFrame, const TouchInputInfo& info)
				: uFrame(uFrame)
				, info(info)
		{}
	};

public:
	void Initialize(int nDelay);
	void Update();

	void StartDelayInput();
	void StopDelayInput();
	void ResetDelayInput();

	uint32_t GetCurrentFrame() const;
	void AddTouchInfo(const TouchInputInfo& info, int nPlayer = 0);
	void AddDelayTouchInfo(const DelayTouchInfo& info, int nPlayer = 0);
	void ClearTouchInfo(int nPlayer= 0);
	bool FindDelayTouchInfo(TouchInputInfo& info, int nEvent, int nPlayer = 0) const;
	DelayTouchInfo GetNextDelayTouchInfo(int nPlayer = 0) const;
	bool IsInputEmpty(int nPlayer = 0) const;

private:
	enum {
		eSTATE_START = (1 << 0),
		eSTATE_STOP = (1 << 1) ,
	};

	uint8_t  m_uState;
	uint32_t m_nDelay;
	uint32_t m_uCurrentFrame;
	std::deque<DelayTouchInfo>		m_aTouchInputInfo[NET_CONNECT_MAX];
};

#define DELAY_INPUT() DelayInput::Get()

#endif