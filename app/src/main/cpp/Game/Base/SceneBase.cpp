//
// Created by 葵ユメ on 2018/09/05.
//


#include "SceneBase.h"
#include <SceneManager.h>
#include <LayoutComponent.h>
#include <DelayInput.h>
#include <TransferManager.h>
#include <TransferBase.h>

//------------------------------------------
//------------------------------------------
SceneBase::SceneBase(const char* pSceneName)
: EntityBase()
, m_nSceneState(eSceneState_ActiveWait)
, m_nSyncStep(eSyncStep_None)
, m_SceneName(pSceneName)
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
	return m_SceneName.c_str();
}

//------------------------------------------
//------------------------------------------
void SceneBase::SetSyncStep(int nStep)
{
	m_nSyncStep = nStep;
}

//------------------------------------------
//------------------------------------------
int SceneBase::GetSyncStep() const
{
	return m_nSyncStep;
}

//------------------------------------------
//------------------------------------------
bool SceneBase::IsActiveWait() const
{
	return GetSceneState() == eSceneState_ActiveWait;
}

//------------------------------------------
//------------------------------------------
bool SceneBase::IsSyncWait() const
{
	return GetSceneState() == eSceneState_SyncWait;
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
	if(TransferManager::Get()->IsConnectSucess()){
		m_nSyncStep = eSyncStep_SceneSync;
		m_nSceneState = eSceneState_SyncWait;
	}
	else {
		m_nSceneState = eSceneState_Actived;
	}

	// 継承先でも実装
}

//------------------------------------------
//------------------------------------------
void SceneBase::SceneSync()
{
	// 継承先でも実装
	auto pManager = TransferManager::Get();
	if(m_nSyncStep == eSyncStep_SceneSync){
		DEBUG_LOG("SyncWait\n");
		pManager->StartTransfer(TransferManager::eTransferKind_SyncScene);
		m_nSyncStep = eSyncStep_TouchInfo;
	}
	else if(m_nSyncStep == 1){
		auto pTransfer = pManager->GetTransfer<TransferBase>(TransferManager::eTransferKind_SyncScene);
		if(pTransfer->IsEnd()){
			if(!pManager->IsStartTransfer(TransferManager::eTransferKind_TouchInfo)) {
				pManager->StartTransfer(TransferManager::eTransferKind_TouchInfo);
			}
			DELAY_INPUT()->ResetDelayInput();
			m_nSyncStep = eSyncStep_UserSync;
		}
	}
	else if(m_nSyncStep == eSyncStep_UserSync){
		m_nSyncStep = eSyncStep_End;
	}
}

//------------------------------------------
//------------------------------------------
void SceneBase::SceneUpdate()
{
	// 継承先でも実装

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
bool SceneBase::IsSceneSyncEnd() const
{
	return m_nSyncStep == eSyncStep_End;
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
		case eGameMessage_Sync:
		{
			SceneSync();
			if(IsSceneSyncEnd()){
				m_nSceneState = eSceneState_Actived;
			}
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
