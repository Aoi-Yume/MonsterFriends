//
// Created by 葵ユメ on 2018/09/05.
//


#ifndef AOIYUME_SCENE_BASE_H
#define AOIYUME_SCENE_BASE_H

#include "../Engine/Engine.h"
#include "entity_define.h"
#include "EntityBase.h"

class ComponentBase;

class SceneBase : public EntityBase
{
public:
	enum SceneState{
		eSceneState_None,
		eSceneState_ActiveWait,
		eSceneState_SyncWait,
		eSceneState_Actived,
		eSceneState_DeactiveWait,
		eSceneState_Deactived,
		eSceneState_Max
	};

	enum SyncStep{
		eSyncStep_None = -1,
		eSyncStep_SceneSync,
		eSyncStep_TouchInfo,
		eSyncStep_UserSync,
		eSyncStep_End = 100
	};

public:
	SceneBase(const char* pSceneName);
	virtual ~SceneBase();
	
public:
	int GetSceneState() const;
	const char* GetSceneName() const;

	void SetSyncStep(int nStep);
	int GetSyncStep() const;
	
	bool IsActiveWait() const;
	bool IsSyncWait() const;
	bool IsActived() const;
	bool IsDeactiveWait() const;
	bool IsDeactived() const;

	bool IsSceneSyncEnd() const;

protected:
	virtual void SceneSetup();
	virtual void SceneSync();
	virtual void SceneUpdate();
	virtual void SceneFinalize();

	void EntityUpdate(GameMessage message, const void* param) override;
private:
	int				m_nSceneState;
	int 			m_nSyncStep;
	std::string		m_SceneName;
};

#endif