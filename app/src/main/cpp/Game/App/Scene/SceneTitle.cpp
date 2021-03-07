//
// Created by 葵ユメ on 2018/10/08.
//


#include <LayoutComponent.h>
#include <TransformComponent.h>
#include <SceneManager.h>
#include <AppParam.h>
#include "SceneTitle.h"
#include "Button/SimpleButton.h"
#include "SceneGameMain.h"
#include <../Net/TransferManager.h>
#include <../Net/TransferBase.h>
#include <State.h>
#include <FadeCtrl.h>

SceneBase* SceneTitle::CreateScene()
{
	return new SceneTitle();
}

namespace {
	//==========================================
	//==========================================
	class StateFadeIn : public StateBase {
		void Begin(void *pUserPtr) override {
			FADE()->In();
		}

		void Update(void *pUserPtr) override {
			if (FADE()->IsFadeInEnd()) {
				ChangeState(SceneTitle::eState_WaitPressButton);
			}
		}
	};

	//==========================================
	//==========================================
	class StateWaitPressButton : public StateBase {
		void Begin(void* pUserPtr) override{
			auto p = reinterpret_cast<SceneTitle *>(pUserPtr);
			p->m_pButtonManager->SetVisible(true);
		}
		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle *>(pUserPtr);
			if (p->m_pButtonManager->GetDecide() != -1) {
				ChangeState(SceneTitle::eState_WaitKeyboardEnable);
			}
		}
	};

	//==========================================
	//==========================================
	class StateWaitKeyboardEnable : public StateBase {
		void Begin(void *pUserPtr) override {
			Engine::GetEngine()->ShowSoftwareKeyboard();
		}

		void Update(void *pUsrPtr) override {
			// 一旦有効になるのを待つ
			if (Engine::GetEngine()->IsShowSoftwareKeyboard()) {
				ChangeState(SceneTitle::eState_WaitInput);
			}
		}
	};

	//==========================================
	//==========================================
	class StateWaitInput : public StateBase {
		void Update(void *pUserPtr) override {
			if (!Engine::GetEngine()->IsShowSoftwareKeyboard()) {
				char charName[64];
				Engine::GetEngine()->GetInputText(charName, sizeof(charName));
				AppParam::Get()->SetCharaName(charName);
				if (strcmp(charName, "親") == 0) {
					Engine::GetEngine()->StartNearbyAdvertising(charName);
					TransferManager::Get()->Initialize(true);
					DEBUG_LOG("親NEarby\n");
				} else {
					Engine::GetEngine()->StartNearbyDiscovery(charName);
					TransferManager::Get()->Initialize(false);
					DEBUG_LOG("子NEarby\n");
				}
				TransferManager::Get()->StartTransfer(TransferManager::eTransferKind_Connect);
				ChangeState(SceneTitle::eState_WaitNearbyConnect);
			}
		}
	};

	//==========================================
	//==========================================
	class StateWaitNearbyConnect : public StateBase {
		void Begin(void *pUserPtr) override {
			m_fTime = 0.0f;
		}

		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle *>(pUserPtr);
			auto pTextImage = (TextImageComponent *) (p->m_pPlayerName->GetComponent(eComponentKind_Layout));
			pTextImage->SetVisible(true);
			if (m_fTime >= 3.0f) {
				m_fTime = 0.0f;
			} else if (m_fTime >= 2.0f) {
				pTextImage->SetText("接続中・・・", 5.0f);
			} else if (m_fTime >= 1.0f) {
				pTextImage->SetText("接続中・・", 5.0f);
			} else {
				pTextImage->SetText("接続中・", 5.0f);
			}
			m_fTime += Engine::GetEngine()->GetDeltaTime();

			// 通信接続完了
			auto pTransfer = TransferManager::Get()->GetTransfer<TransferBase>(TransferManager::eTransferKind_Connect);
			if (pTransfer->IsEnd()) {
				pTransfer->Dump();
				Engine::GetEngine()->StopNearbyAdvertising();
				Engine::GetEngine()->StopNearbyDiscovery();
				TransferManager::Get()->StartTransfer(TransferManager::eTransferKind_PlayerId);
				ChangeState(SceneTitle::eState_WaitNearbyTransferPlayerId);
			}
		}

		float m_fTime;
	};

	//==========================================
	//==========================================
	class StateWaitNearbyTransferPlayerId : public StateBase {
		void Begin(void *pUserPtr) override {
			m_fTime = 0.0f;
		}

		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle *>(pUserPtr);
			auto pTextImage = (TextImageComponent *) (p->m_pPlayerName->GetComponent(eComponentKind_Layout));
			pTextImage->SetVisible(true);
			if (m_fTime >= 3.0f) {
				m_fTime = 0.0f;
			} else if (m_fTime >= 2.0f) {
				pTextImage->SetText("接続中・・・", 5.0f);
			} else if (m_fTime >= 1.0f) {
				pTextImage->SetText("接続中・・", 5.0f);
			} else {
				pTextImage->SetText("接続中・", 5.0f);
			}
			m_fTime += Engine::GetEngine()->GetDeltaTime();

			// プレイヤーID通信完了
			auto pTransfer = TransferManager::Get()->GetTransfer<TransferBase>(TransferManager::eTransferKind_PlayerId);
			if (pTransfer->IsEnd()) {
				pTransfer->Dump();
				TransferManager::Get()->SetConnectSuccess(true);
				ChangeState(SceneTitle::eState_FadeOut);
			}
		}

		float m_fTime;
	};

	//==========================================
	//==========================================
	class StateFadeOut : public StateBase {
		void Begin(void *pUserPtr) override {
			FADE()->Out();
		}

		void Update(void *pUserPtr) override {
			if (FADE()->IsFadeOutEnd()) {
				SCENE_MANAGER()->ChangeScene(SceneGameMain::CreateScene());
			}
		}
	};
}

//==========================================
//==========================================
SceneTitle::SceneTitle()
: SceneBase("title")
, m_fEggCnt(0.0f)
, m_pBgImage(nullptr)
, m_pEggImage(nullptr)
, m_pTitleImage(nullptr)
, m_pButtonManager(nullptr)
, m_pStateManager(nullptr)
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
	delete m_pStateManager;
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
		m_pButtonManager->SetVisible(false);
	}
	{
		m_pPlayerName = new Entity();
		Entity::CreateTextImageComponent(m_pPlayerName, "", 32);
		auto pTextImage = (TextImageComponent*)(m_pPlayerName->GetComponent(eComponentKind_Layout));
		pTextImage->SetVisible(false);
		pTextImage->SetOrtho(true);
		m_pPlayerName->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pStateManager = new StateManager(eState_Max);
		m_pStateManager->SetUserPtr(this);
		m_pStateManager->CreateState<StateFadeIn>();
		m_pStateManager->CreateState<StateWaitPressButton>();
		m_pStateManager->CreateState<StateWaitKeyboardEnable>();
		m_pStateManager->CreateState<StateWaitInput>();
		m_pStateManager->CreateState<StateWaitNearbyConnect>();
		m_pStateManager->CreateState<StateWaitNearbyTransferPlayerId>();
		m_pStateManager->CreateState<StateFadeOut>();
		m_pStateManager->ChangeState(eState_FadeIn);
	}
	TransferManager::Get()->ResetConnect();
	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void SceneTitle::SceneUpdate() {
	Super::SceneUpdate();
	//DEBUG_LOG("Launcher Call Update");

	m_pStateManager->Update();

	m_pEggImage->SetRotate(0, 0, sinf(DEGTORAD(m_fEggCnt)) * 20.0f);
	m_fEggCnt += Engine::GetEngine()->GetDeltaTime() * 100.0f;
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
