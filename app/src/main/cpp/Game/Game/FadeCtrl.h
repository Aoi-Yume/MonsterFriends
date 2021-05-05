//
// Created by 葵ユメ on 2021/02/27.
//


#ifndef AOIYUME_FADE_CTRL_H
#define AOIYUME_FADE_CTRL_H

#include "Entity.h"
#include "../Engine/Engine.h"
#include <Singleton.h>

class FadeCtrl : public Singleton<FadeCtrl>
{
	USE_SINGLETON(FadeCtrl)
	
	enum FadeState{
		eFadeState_None,
		eFadeState_In,
		eFadeState_Out,
		eFadeState_InEnd,
		eFadeState_OutEnd,
	};
	
public:
	void In(const VEC3& color = {0, 0, 0}, float fDuration = 0.25f);
	void Out(const VEC3& color = {0, 0, 0}, float fDuration = 0.25f);

	bool IsFadeInEnd() const;
	bool IsFadeOutEnd() const;

	void Update(GameMessage message, const void* param);

private:
	void fadeUpdate();

private:
	float 		m_fFadeTime;
	float	 	m_fFadeDuration;
	VEC3		m_Color;
	FadeState	m_FadeState;
	Entity*		m_pFadeEntity;
};
#define FADE() FadeCtrl::Get()

#endif