//
// Created by 葵ユメ on 2021/03/07.
//


#ifndef AOIYUME_ANIMATION_COMPONENT_H
#define AOIYUME_ANIMATION_COMPONENT_H

#include <map>
#include "../Engine/Engine.h"
#include "ComponentBase.h"
#include "../../Math/math_Vector.h"

class AnimationBase;
class AnimationComponent : public ComponentBase
{
    typedef ComponentBase Super;
public:
	AnimationComponent(EntityBase* pEntityBase);
	virtual ~AnimationComponent();

	void AddAnimation(const char* pName, AnimationBase* pAnimation);
	void Play(const char* pName, float fSpeed = 1.0f);
	void Stop(const char* pName);
	bool IsEnd(const char* pName) const;
	
	virtual GameMessageResult Update(GameMessage message, const void* param) override;

private:
	void update();
	void destroy();
	
private:
	std::map<const char*, AnimationBase*>		m_aAnimation;
};

class AnimationBase
{
	friend class AnimationComponent;
public:
	enum State {
		eState_None,
		eState_Play,
		eState_Pause,
		eState_Stop,
		eState_End,
		eState_Max
	};

public:
	AnimationBase(float fMaxTime)
		: m_State(eState_None)
		, m_Time(0.0f)
		, m_MaxTime(fMaxTime)
		, m_fSpeed(1.0f)
		, m_pEntityBase(nullptr)
	{}
	virtual ~AnimationBase(){}

public:
	void Play() { m_State = eState_Play; m_Time = 0.0f; };
	void Stop(){ m_State = eState_Stop; m_Time = 0.0f; }
	void Pause(){ m_State = eState_Pause; }

	bool IsPlay() const{ return m_State == eState_Play; }
	bool IsEnd() const { return m_State == eState_End; }
	float GetTime() const{ return m_Time; }
	float GetMaxTime() const{ return m_MaxTime; }
	float GetRatio() const{ ASSERT(m_MaxTime > 0.0f); return m_Time / m_MaxTime; }

	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }

protected:
	virtual void update(const float fDeltaTime){
		if(m_Time >= m_MaxTime){ m_State = eState_End; }
		m_Time = std::min(m_Time + fDeltaTime * m_fSpeed, m_MaxTime);
	}
	EntityBase* GetEntityBase() const{ return m_pEntityBase; }

private:
	void SetEntityBase(EntityBase* pEntityBase){ m_pEntityBase = pEntityBase; };

private:
	State		m_State;
	float		m_Time;
	float		m_MaxTime;
	float_t 	m_fSpeed;
	EntityBase*	m_pEntityBase;
};

class SimpleOpenAnimation : public AnimationBase
{
	typedef AnimationBase Super;
public:
	SimpleOpenAnimation(float fDefaultScale = 1.0f, float fOverScale = 1.2f, float fMaxTime = 0.15f);
	virtual ~SimpleOpenAnimation();

protected:
	void update(const float fDeltaTime) override;

private:
	VEC3 m_BaseScale;
	float m_fDefaultScale;
	float m_fOverScale;
};

class LinearAnimation : public AnimationBase
{
	typedef AnimationBase Super;
public:
	LinearAnimation(
			std::pair<float, float>&& scale,
			std::pair<float, float>&& rotZ,
			std::pair<VEC3, VEC3>&& pos,
			std::pair<float, float>&& alpha,
			float fMaxTime = 0.2f);
	virtual ~LinearAnimation();

protected:
	void update(const float fDeltaTaime) override;

private:
	std::pair<float, float> m_LinearScale;
	std::pair<float, float> m_LinearRotZ;
	std::pair<VEC3, VEC3> m_LinearPos;
	std::pair<float, float> m_LinearAlpha;
};
#endif