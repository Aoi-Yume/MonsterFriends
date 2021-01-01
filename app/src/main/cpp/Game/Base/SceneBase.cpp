//
// Created by 葵ユメ on 2018/09/05.
//


#include "SceneBase.h"
#include <SceneManager.h>
#include <LayoutComponent.h>
#include <DelayInput.h>

//------------------------------------------
//------------------------------------------
SceneBase::SceneBase(const char* pSceneName)
: EntityBase()
, m_nSceneState(eSceneState_ActiveWait)
, m_pSceneName(pSceneName)
{
}

//------------------------------------------
//------------------------------------------
SceneBase::~SceneBase()
{
}

//------------------------------------------
//------------------------------------------
int SceneBase::GetSceneState() const
{
	return m_nSceneState;
}

//------------------------------------------
//------------------------------------------
const char* SceneBase::GetSceneName() const
{
	return m_pSceneName;
}

//------------------------------------------
//------------------------------------------
bool SceneBase::IsActiveWait() const
{
	return GetSceneState() == eSceneState_ActiveWait;
}

//------------------------------------------
//------------------------------------------
bool SceneBase::IsActived() const
{
	return GetSceneState() == eSceneState_Actived;
}

//------------------------------------------
//------------------------------------------
bool SceneBase::IsDeactiveWait() const
{
	return GetSceneState() == eSceneState_DeactiveWait;
}

//------------------------------------------
//------------------------------------------
bool SceneBase::IsDeactived() const
{
	return GetSceneState() == eSceneState_Deactived;
}

//------------------------------------------
//------------------------------------------
void SceneBase::SceneSetup()
{
	m_nSceneState = eSceneState_Actived;

	// 継承先で実装
}

//------------------------------------------
//------------------------------------------
void SceneBase::SceneUpdate()
{
	// 継承先で実装

	TouchInputInfo touchInfo = {};
	const bool bFind = Engine::GetEngine()->FindDelayTouchInfo(touchInfo,eTouchEvent_DOWN);
	if(bFind){
		if(	touchInfo.fTouchX <= 20 &&
			touchInfo.fTouchY <= 20){
			// TODO Launcher以外のシーンを削除するようにする
			SCENE_MANAGER()->EndScene();
		}
	}
}

//------------------------------------------
//------------------------------------------
void SceneBase::SceneFinalize()
{
	m_nSceneState = eSceneState_Deactived;
	// 継承先で実装
}

//------------------------------------------
//------------------------------------------
void SceneBase::EntityUpdate(GameMessage message, const void* param)
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
}
