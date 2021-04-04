//
// Created by 葵ユメ on 2018/09/08.
//


#include <EntityManager.h>
#include <Entity.h>
#include <SceneLauncher.h>
#include "../Engine/Engine.h"
#include <AppParam.h>
#include <AppItemList.h>
#include <AppSkillList.h>
#include <AppCharaList.h>
#include <SceneManager.h>
#include <DelayInput.h>
#include <FadeCtrl.h>

EXTERN_C

static EntityManager* s_EntityMgr = nullptr;

//--------------------------------------------------------
JNIEXPORT void JNICALL
Java_com_aoiyume_Game_GameMainRender_SurfaceCreate(
        JNIEnv *env, jobject jobj
)
{
    DEBUG_LOG("Call GameMain SurfaceCreate");
	s_EntityMgr = new EntityManager();
	{
		Entity* pCamera = new Entity();
		Entity::CreateCameraComponent(pCamera);
		pCamera->Update(eGameMessage_Setup, nullptr);
		s_EntityMgr->AttachEntity(pCamera);
	}
	{
		SceneManager::CreateSceneMaanger(new SceneLauncher());
		SCENE_MANAGER()->Update(eGameMessage_Setup, nullptr);
		s_EntityMgr->AttachEntity(SCENE_MANAGER());
	}
	{
		AppParam::Initialize();
		AppItemList::Initialize();
		AppItemList::Get()->Load();
		AppSkillList::Initialize();
		AppSkillList::Get()->Load();
		AppCharaList::Initialize();
		AppCharaList::Get()->Load();
		FadeCtrl::Initialize();
	}
	// Engine に解放コールバックを設定する
	auto DestroyCalback = [=](){
		SceneManager::DestroySceneManager();
		AppParam::Destroy();
		AppItemList::Destroy();
		AppSkillList::Destroy();
		AppCharaList::Destroy();
		FadeCtrl::Destroy();
		delete s_EntityMgr;
	};
	Engine::GetEngine()->SetDestrpyCallBack(DestroyCalback);
}
//--------------------------------------------------------
JNIEXPORT void JNICALL
Java_com_aoiyume_Game_GameMainRender_SurfaceChanged(
        JNIEnv *env, jobject jobj,
		jint w, jint h
)
{
	DEBUG_LOG("Call GameMain SurfaceChanged");

	const float aspect = (float) w / (float)h;
	glViewport(0, 0, w, h);
	Engine::GetEngine()->SetScreenInfo(w, h, aspect);

	s_EntityMgr->Update(eGameMessage_ChangeCamera, nullptr);
}
//--------------------------------------------------------
JNIEXPORT void JNICALL
Java_com_aoiyume_Game_GameMainRender_SurfaceDraw(
        JNIEnv *env, jobject jobj
)
{
   	Engine::GetEngine()->SetTimePoint(std::chrono::system_clock::now());

	s_EntityMgr->Update(eGameMessage_Update, nullptr);
	FADE()->Update(eGameMessage_Update, nullptr);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	s_EntityMgr->Update(eGameMessage_Draw, nullptr);
	FADE()->Update(eGameMessage_Draw, nullptr);

	//glDisable(GL_DEPTH_TEST);
	const float deltaTime = Engine::GetEngine()->GetDeltaTime();
	if(deltaTime < 60 * 60 * 24) {
		DELAY_INPUT()->Update(deltaTime);
	}
}

END_EXTERN_C