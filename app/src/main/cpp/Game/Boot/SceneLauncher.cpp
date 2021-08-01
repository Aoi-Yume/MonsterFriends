//
// Created by 葵ユメ on 2018/09/17.
//


#include <DelayInput.h>
#include <LayoutComponent.h>
#include <SceneManager.h>
#include <Button/ButtonManager.h>
#include <Button/DebugButton.h>
#include <CameraComponent.h>
#include <CollisionComponent.h>
#include <TransformComponent.h>
#include "SceneLauncher.h"
#include "../App/Scene/SceneTitle.h"
#include "../App/Scene/SceneGameMain.h"
#include "../App/Scene/SceneResult.h"

//------------------------------------------
//------------------------------------------
SceneLauncher::SceneLauncher()
: SceneBase("launcher")
, m_pSimpleBtnMgr(nullptr)
{
}

//------------------------------------------
//------------------------------------------
SceneLauncher::~SceneLauncher()
{
	delete m_pSimpleBtnMgr;
}

//------------------------------------------
//------------------------------------------
void SceneLauncher::SceneSetup()
{
	Super::SceneSetup();
	DEBUG_LOG("Launcher Call Setup");
	{
		const float fBtnWidth = 400.0f;
		const float fBtnHeight = 120.0f;
		const float fMarginY = 25.0f;
		const float fScreenX = (float)Engine::GetEngine()->GetScreenInfo().m_nScreenX;
		const float fScreenY = (float)Engine::GetEngine()->GetScreenInfo().m_nScreenY;
		const int nBtnXNum = (int)(fScreenX / fBtnWidth);
		float fBtnX = -fScreenX * 0.5f;
		float fBtnY = fScreenY * 0.5f - fBtnHeight;

		const char* pSceneName[] = {
				"Title",
				"Main",
				"Result",
				NULL
		};
		m_pSimpleBtnMgr = new ButtonManager();
		int nXBtnCnt = 0;
		for(int i = 0; pSceneName[i] != NULL; ++i){
			DebugButton* pBtn = m_pSimpleBtnMgr->CreateDebugButton(pSceneName[i], fBtnWidth, fBtnHeight,
										VEC4(0.8f, 0.8f, 0.8f, 1.0f),
										VEC4(0.0f, 0.0f, 0.0f, 1.0f),
										VEC4(0.5f, 0.5f, 0.0f, 1.0f));
			fBtnX += fBtnWidth;
			nXBtnCnt++;
			if(nXBtnCnt >= nBtnXNum){
				nXBtnCnt = 0;
				fBtnX = -fScreenX * 0.5f + fBtnWidth;
				fBtnY -= (fBtnHeight + fMarginY);
			}
			pBtn->SetPosition(fBtnX, fBtnY, 0.0f);
		}
	}
	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void SceneLauncher::SceneUpdate()
{
	Super::SceneUpdate();
	//DEBUG_LOG("Launcher Call Update");

#if 1
	switch (m_pSimpleBtnMgr->GetDecide()){
		case eButton_Title:{ SCENE_MANAGER()->AddNextCallScene(SceneTitle::CreateScene()); break; }
		case eButton_GameMain:{ SCENE_MANAGER()->AddNextCallScene(SceneGameMain::CreateScene()); break;}
		case eButton_Result:{ SCENE_MANAGER()->AddNextCallScene(SceneResult::CreateScene()); break; }
		default: { 	m_pSimpleBtnMgr->Reset(); break; }
	}
#endif
}

//------------------------------------------
//------------------------------------------
void SceneLauncher::SceneFinalize()
{
	const char* pPath = m_pSimpleBtnMgr->GetDecide() >= 0 ? "sound/se/decide.ogg" : "sound/se/cancel.ogg";
	auto pSoundManager = Engine::GetEngine()->GetSoundManager();
	SoundResourceLabel nHandle = pSoundManager->LoadSE(pPath);
	DEBUG_LOG_A("SE IsLoad:%d", (int)pSoundManager->IsLoadSE(nHandle));
	pSoundManager->PlaySE(nHandle, 1.0f, 1.0f, false);

	Super::SceneFinalize();
	DEBUG_LOG("Launcher Call Finalize");
}

//------------------------------------------
//------------------------------------------
void SceneLauncher::EntityUpdate(GameMessage message, const void* param)
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
		default:{
			break;
		}
	}
	if(message != eGameMessage_Setup) {
		m_pSimpleBtnMgr->Update(message, param);
	}
}
