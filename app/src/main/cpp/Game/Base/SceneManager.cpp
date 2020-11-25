//
// Created by 葵ユメ on 2018/09/16.
//


#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneLauncher.h"

static SceneManager* s_pSceneManger = nullptr;

//------------------------------------------
//------------------------------------------
SceneManager* SceneManager::CreateSceneMaanger(SceneBase* pBootScene)
{
	assert(s_pSceneManger == nullptr);
	s_pSceneManger = new SceneManager(pBootScene);
	return s_pSceneManger;
}

SceneManager* SceneManager::GetSceneManager()
{
	return s_pSceneManger;
}

void SceneManager::DestroySceneManager()
{
	delete s_pSceneManger;
	s_pSceneManger = nullptr;
}

//------------------------------------------
//------------------------------------------
SceneManager::SceneManager(SceneBase* pInitalizeScene)
: EntityBase()
, m_nState(eManagerState_None)
, m_nNextState(eManagerState_None)
, m_nNextCallState(eCallState_None)
, m_pReturnSceneName(nullptr)
, m_pInitalizeScene(pInitalizeScene)
, m_pNextScene(nullptr)
{
}

//------------------------------------------
//------------------------------------------
SceneManager::~SceneManager()
{
}

//------------------------------------------
//------------------------------------------
void SceneManager::AddNextCallScene(SceneBase* pScene)
{
	if(isCallState()){ return; }
	setCallState(eCallState_Add, pScene);
	
	auto it = m_slScene.rbegin();
	(*it)->Update(eGameMessage_Destroy, nullptr);
}

//------------------------------------------
//------------------------------------------
void SceneManager::ChangeScene(SceneBase* pScene)
{
	if(isCallState()){ return; }
	setCallState(eCallState_Change, pScene);
	
	auto it = m_slScene.rbegin();
	(*it)->Update(eGameMessage_Destroy, nullptr);
}

//------------------------------------------
//------------------------------------------
void SceneManager::EndScene()
{
	if(isCallState()) { return; }
	setCallState(eCallState_End, nullptr);

	auto it = m_slScene.rbegin();
	(*it)->Update(eGameMessage_Destroy, nullptr);
}

//------------------------------------------
//------------------------------------------
void SceneManager::ReturnScene(const char* pSceneName)
{
	if(isCallState()){ return; }
	setCallState(eCallState_Return, nullptr, pSceneName);
	
	auto it = m_slScene.rbegin();
	(*it)->Update(eGameMessage_Destroy, nullptr);
}

//------------------------------------------
//------------------------------------------
void SceneManager::EntityUpdate(GameMessage message, const void* param)
{
	switch(message){
		case eGameMessage_Setup:
		{
			setupManager();
			break;
		}
		case eGameMessage_Update:
		{
			updateManager();
			break;
		}
		case eGameMessage_Draw:
		{
			auto it = m_slScene.rbegin();
			if(it == m_slScene.rend()){ break; }
			if((*it)->IsActived()) {
				(*it)->Update(eGameMessage_Draw, nullptr);
			}
			break;
		}
		case eGameMessage_Pause:
		{
			break;
		}
		case eGameMessage_Destroy:
		{
			finalizeManager();
			break;
		}
	}
}

//------------------------------------------
//------------------------------------------
void SceneManager::setCallState(int nState, SceneBase* pScene, const char* pReturnSceneName)
{
	m_nNextCallState = nState;
	m_pNextScene = pScene;
	m_pReturnSceneName = pReturnSceneName;
}

//------------------------------------------
//------------------------------------------
void SceneManager::clearCallState()
{
	m_nNextCallState = eCallState_None;
	m_pNextScene = nullptr;
	m_pReturnSceneName = nullptr;
}

//------------------------------------------
//------------------------------------------
void SceneManager::popScene()
{
	auto it = m_slScene.rbegin();
	if(strcmp((*it)->GetSceneName(), "launcher") != 0) {
		delete (*it);
	}
	m_slScene.pop_back();
}

//------------------------------------------
//------------------------------------------
bool SceneManager::isCallState() const
{
	const bool isCall = m_nNextCallState != eCallState_None;
	if(isCall){
		DEBUG_LOG("既に次のシーンが設定されています");
	}
	return isCall;
}

//------------------------------------------
//------------------------------------------
void SceneManager::setupManager()
{
	m_slScene.push_back(m_pInitalizeScene);
	m_nState = eManagerState_Update;
}

//------------------------------------------
//------------------------------------------
void SceneManager::updateManager()
{
	switch(m_nState){
		case eManagerState_Update:
		{
			auto it = m_slScene.rbegin();
			if(it == m_slScene.rend()){ break; }
			if((*it)->IsActiveWait()){
				(*it)->Update(eGameMessage_Setup, nullptr);
			}
			else if((*it)->IsActived()){
				(*it)->Update(eGameMessage_Update, nullptr);
			}
			else if((*it)->IsDeactiveWait()){
				(*it)->Update(eGameMessage_Destroy, nullptr);
			}
			if((*it)->IsDeactived()){
				m_nNextState = eManagerState_Change;
			}
			break;
		}
		case eManagerState_Change:
		{
			if(m_nNextCallState == eCallState_Add){
				DEBUG_LOG("SceneManager::Scene Add");
				m_slScene.push_back(m_pNextScene);
				clearCallState();
				m_nNextState = eManagerState_Update;
			}
			else if(m_nNextCallState == eCallState_Change){
				DEBUG_LOG("SceneManager::Scene Change");
				popScene();
				m_slScene.push_back(m_pNextScene);
				clearCallState();
				m_nNextState = eManagerState_Update;
			}
			else if(m_nNextCallState == eCallState_End){
				DEBUG_LOG("SceneManager::Scene End");
				popScene();
				size_t size = m_slScene.size();
				if(size == 0 ){
					m_pInitalizeScene->Update(eGameMessage_Setup, nullptr);
					m_slScene.push_back(m_pInitalizeScene);
				}
				else if(*m_slScene.rbegin() == m_pInitalizeScene){
					m_pInitalizeScene->Update(eGameMessage_Setup, nullptr);
				}
				clearCallState();
				m_nNextState = eManagerState_Update;
			}
			else if(m_nNextCallState == eCallState_Return){
				DEBUG_LOG("SceneManager::Scene Return");
				size_t size = m_slScene.size();
				while(size > 0){
					const char* pNextPopSceneName = (*m_slScene.rbegin())->GetSceneName();
					if(strcmp(pNextPopSceneName, m_pReturnSceneName) == 0){
						break;
					}
					popScene(); 
					size--;
				}
				if(size == 0){ DEBUG_LOG("Not Find Return Scene"); }
				clearCallState();
				m_nNextState = eManagerState_Update;
			}
			dumpSceneListName();
			break;
		}
	}
	if(m_nNextState != eManagerState_None){
		m_nState = m_nNextState;
		m_nNextState = eManagerState_None;
	}
}

//------------------------------------------
//------------------------------------------
void SceneManager::finalizeManager()
{
}

//------------------------------------------
//------------------------------------------
void SceneManager::dumpSceneListName()
{
	DEBUG_LOG("======= SceneList Dump =======");
	for(const auto& pScene : m_slScene){
		DEBUG_LOG_A("%s", pScene->GetSceneName());
	}
	DEBUG_LOG("=============================");
}