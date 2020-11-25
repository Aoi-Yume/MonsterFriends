//
// Created by 葵ユメ on 2018/10/08.
//


#include <LayoutComponent.h>
#include <TransformComponent.h>
#include <SceneManager.h>
#include "SceneTitle.h"
#include "Button/SimpleButton.h"
#include "SceneGameMain.h"

SceneBase* SceneTitle::CreateScene()
{
	return new SceneTitle();
}

//------------------------------------------
//------------------------------------------
SceneTitle::SceneTitle()
: SceneBase("title")
, m_nStep(0)
, m_nNextStep(0)
, m_nStepCnt(0)
, m_nEggCnt(0)
, m_nTranslateCnt(0)
, m_pBgImage(nullptr)
, m_pEggImage(nullptr)
, m_pTitleImage(nullptr)
, m_pButtonManager(nullptr)
{
	DEBUG_LOG("Scene Title Constructor");
}

//------------------------------------------
//------------------------------------------
SceneTitle::~SceneTitle()
{
	delete m_pPlayerName;
	delete m_pButtonManager;
	delete m_pTitleImage;
	delete m_pEggImage;
	delete m_pBgImage;
}

//------------------------------------------
//------------------------------------------
void SceneTitle::SceneSetup() {
	Super::SceneSetup();
	DEBUG_LOG("Title Call Setup");
	{
		m_pBgImage = new Entity();
		Entity::CreateLayoutComponent(m_pBgImage, "image/monfri_bg.png");
		auto pLayoutComponent = (LayoutComponent *) m_pBgImage->GetComponent(eComponentKind_Layout);
		pLayoutComponent->SetOrtho(true);
		m_pBgImage->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pEggImage = new Entity();
		Entity::CreateLayoutComponent(m_pEggImage, "image/egg_plane.png");
		auto pLayoutComponent = (LayoutComponent *) m_pEggImage->GetComponent(
				eComponentKind_Layout);
		pLayoutComponent->SetOrtho(true);
		m_pEggImage->SetPosition(0, -80, 0);
		m_pEggImage->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pTitleImage = new Entity();
		Entity::CreateLayoutComponent(m_pTitleImage, "image/title.png");
		auto pLayoutComponent = (LayoutComponent *) m_pTitleImage->GetComponent(
				eComponentKind_Layout);
		pLayoutComponent->SetOrtho(true);
		m_pTitleImage->SetPosition(0, 330.0f, 0);
		m_pTitleImage->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pButtonManager = new ButtonManager();
		std::pair<const char *, VEC3> btnList[] = {
				{"image/online_button.png", VEC3(-500.0f, -400.0f, 0)},
				{"image/local_button.png",  VEC3(500.0f, -400.0f, 0)}
		};
		for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
			auto btn = m_pButtonManager->CreateButton(btnList[i].first);
			btn->SetPosition(btnList[i].second);
		}
	}
	{
		m_pPlayerName = new Entity();
		Entity::CreateTextImageComponent(m_pPlayerName, "", 32);
		auto pTextImage = (TextImageComponent*)(m_pPlayerName->GetComponent(eComponentKind_Layout));
		pTextImage->SetVisible(false);
		pTextImage->SetOrtho(true);
		m_pPlayerName->Update(eGameMessage_Setup, nullptr);
	}
	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void SceneTitle::SceneUpdate() {
	Super::SceneUpdate();
	//DEBUG_LOG("Launcher Call Update");

	m_pEggImage->SetRotate(0, 0, sinf(DEGTORAD(m_nEggCnt)) * 20.0f);
	m_nEggCnt++;

	if(m_nStep == 0){
		if(m_pButtonManager->GetDecide() != -1){
			Engine::GetEngine()->ShowSoftwareKeyboard();
			m_nNextStep = 1;
		}
	}
	else if(m_nStep == 1){
		// 一旦有効になるのを待つ
		if(Engine::GetEngine()->IsShowSofrwareKeyboard()){
			m_nNextStep = 2;
		}
	}
	else if(m_nStep == 2){
		if(!Engine::GetEngine()->IsShowSofrwareKeyboard()){
			m_nNextStep = 3;
		}
	}
	else if(m_nStep == 3){
		const int nSubCnt = m_nStepCnt % 180;
		auto pTextImage = (TextImageComponent *) (m_pPlayerName->GetComponent(eComponentKind_Layout));
		pTextImage->SetVisible(true);
		if(nSubCnt >= 120) {
			pTextImage->SetText("接続中・・・", 5.0f);
		}
		else if(nSubCnt >= 60){
			pTextImage->SetText("接続中・・", 5.0f);
		}
		else{
			pTextImage->SetText("接続中・", 5.0f);
		}
		// TODO 通信接続完了
		if(m_nStepCnt >= 180 * 5) {
			m_nNextStep = 4;
		}
	}
	else if(m_nStep == 4){
		SCENE_MANAGER()->ChangeScene(SceneGameMain::CreateScene());
	}
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
void SceneTitle::SceneFinalize()
{
	Super::SceneFinalize();
	DEBUG_LOG("Title Call Finalize");
}

//------------------------------------------
//------------------------------------------
void SceneTitle::EntityUpdate(GameMessage message, const void* param)
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
		m_pBgImage->Update(message, param);
		m_pEggImage->Update(message, param);
		m_pTitleImage->Update(message, param);
		m_pButtonManager->Update(message, param);
		m_pPlayerName->Update(message, param);
	}
}
