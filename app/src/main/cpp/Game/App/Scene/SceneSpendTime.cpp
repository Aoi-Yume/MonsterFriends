//
// Created by 葵ユメ on 2018/10/08.
//


#include <LayoutComponent.h>
#include <TransformComponent.h>
#include "SceneSpendTime.h"
#include "Button/SimpleButton.h"

SceneBase* SceneSpendTime::CreateScene()
{
	return new SceneSpendTime();
}

//------------------------------------------
//------------------------------------------
SceneSpendTime::SceneSpendTime()
: SceneBase("SpendTime")
, m_nStep(0)
, m_nNextStep(0)
, m_nStepCnt(0)
, m_pSceneName(nullptr)
{
	DEBUG_LOG("Scene SpendTime Constructor");
}

//------------------------------------------
//------------------------------------------
SceneSpendTime::~SceneSpendTime()
{
	delete m_pSceneName;
}

//------------------------------------------
//------------------------------------------
void SceneSpendTime::SceneSetup() {
	Super::SceneSetup();
	DEBUG_LOG("SceneSpendTime Call Setup");
	{
		m_pSceneName = new Entity();
		Entity::CreateTextImageComponent(m_pSceneName, "", 32);
		auto pText = (TextImageComponent*)m_pSceneName->GetComponent(eComponentKind_Layout);
		pText->SetOrtho(true);
		m_pSceneName->Update(eGameMessage_Setup, nullptr);
		pText->SetText("SpentTime", 5.0f);
	}
	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void SceneSpendTime::SceneUpdate() {
	Super::SceneUpdate();
	//DEBUG_LOG("Launcher Call Update");

	if(m_nStep != m_nNextStep){
		m_nStep = m_nNextStep;
		m_nStepCnt = 0;
	}
	else{
		m_nStepCnt++;
	}
}

//------------------------------------------
//------------------------------------------
void SceneSpendTime::SceneFinalize()
{
	Super::SceneFinalize();
	DEBUG_LOG("Title Call Finalize");
}

//------------------------------------------
//------------------------------------------
void SceneSpendTime::EntityUpdate(GameMessage message, const void* param)
{
	switch(message){
		case eGameMessage_Setup:
		{
			SceneSetup();
			break;
		}
		case eGameMessage_Update:
		{
			SceneUpdate();
			break;
		}
		case eGameMessage_PostUpdate:
		{
			break;
		}
		case eGameMessage_ChangeCamera:
		{
			break;
		}
		case eGameMessage_Draw:
		{
			break;
		}
		case eGameMessage_Pause:
		{
			break;
		}
		case eGameMessage_Destroy:
		{
			SceneFinalize();
			break;
		}
	}

	if(message != eGameMessage_Setup) {
		m_pSceneName->Update(message, param);
	}
}
