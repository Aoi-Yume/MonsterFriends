//
// Created by 葵ユメ on 2018/09/16.
//


#ifndef AOIYUME_SCENE_MANAGER_H
#define AOIYUME_SCENE_MANAGER_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include <list>
#include <EntityBase.h>

class SceneBase;

class SceneManager : public EntityBase
{
public:
	enum CallState {
		eCallState_None,
		eCallState_Add,
		eCallState_Change,
		eCallState_End,
		eCallState_Return,
		eCallState_Max
	};
	
	enum ManagerState {
		eManagerState_None,
		eManagerState_Update,
		eManagerState_Change,
		eManagerState_Pop,
		eManagerState_Max
	};
public:
	static SceneManager* CreateSceneMaanger(SceneBase* pBootScene);
	static SceneManager* GetSceneManager();
	static void DestroySceneManager();

public:
	SceneManager(SceneBase* pInitalizeScene);
	virtual ~SceneManager();

	void AddNextCallScene(SceneBase* pScene);
	void ChangeScene(SceneBase* pScene);
	void EndScene();
	void ReturnScene(const char* pSceneName);

protected:
	void EntityUpdate(GameMessage message, const void* param) override;

private:
	void setCallState(int nState, SceneBase* pScene, const char* pReturnSceneName = nullptr);
	void clearCallState();
	void popScene();
	
	bool isCallState() const;
	
	void setupManager();
	void updateManager();
	void finalizeManager();

	void dumpSceneListName();
private:
	int						m_nState;
	int						m_nNextState;
	
	int						m_nNextCallState;
	int 					m_nWait;

	const char*			m_pReturnSceneName;
	SceneBase*				m_pInitalizeScene;
	SceneBase* 				m_pNextScene;
	std::list<SceneBase*>	m_slScene;
};

#define SCENE_MANAGER() SceneManager::GetSceneManager()

#endif