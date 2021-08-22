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
#include <Random.h>
#include <FadeCtrl.h>
#include <Window/MessageWindow.h>
#include <BackGround.h>
#include <Egg.h>
#include <Character.h>
#include <AppCharaList.h>

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
			p->m_aButtonManager[SceneTitle::eBTN_MANAGER_TITLE]->SetVisible(true);
			p->m_aButtonManager[SceneTitle::eBTN_MANAGER_TITLE]->Unlock();
			p->m_aButtonManager[SceneTitle::eBTN_MANAGER_TITLE]->Reset();
		}
		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle *>(pUserPtr);
			const int nDecide = p->m_aButtonManager[SceneTitle::eBTN_MANAGER_TITLE]->GetDecide();
			if (nDecide == SceneTitle::eBTN_LOCAL) {
				p->m_aButtonManager[SceneTitle::eBTN_MANAGER_TITLE]->Lock();
				p->m_aButtonManager[SceneTitle::eBTN_MANAGER_TITLE]->SetVisible(false);
				ChangeState(SceneTitle::eState_BorneStartMonster);
			}
			else if(nDecide == SceneTitle::eBTN_TUTORIAL){
				p->m_aButtonManager[SceneTitle::eBTN_MANAGER_TITLE]->Lock();
				ChangeState(SceneTitle::eState_ShowLicense);
			}
		}
	};

	//==========================================
	//==========================================
	class StateBorneStartMonster : public StateBase {
		void Begin(void* pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle*>(pUserPtr);
			p->m_pEgg->StartBorne();
		}
		void Update(void* pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle*>(pUserPtr);
			if(p->m_pEgg->IsBorneEnd()){ ChangeState(SceneTitle::eState_BorneFadeOut); }
		}
	};

	//==========================================
	//==========================================
	class StateBorneFadeOut : public StateBase {
		void Begin(void *pUserPtr) override {
			FADE()->Out({1, 1, 1});
		}

		void Update(void *pUserPtr) override {
			if (FADE()->IsFadeOutEnd()) {
				auto p = reinterpret_cast<SceneTitle*>(pUserPtr);
				p->m_pEgg->SetVisible(false);
				p->m_pChara->SetVisible(true);
				p->m_pChara->InVisibleDice();
				ChangeState(SceneTitle::eState_BorneFadeIn);
			}
		}
	};

	//==========================================
	//==========================================
	class StateBorneFadeIn : public StateBase {
		void Begin(void *pUserPtr) override {
			FADE()->In({1, 1, 1});
		}

		void Update(void *pUserPtr) override {
			if (FADE()->IsFadeInEnd()) {
				ChangeState(SceneTitle::eState_BorneMessage);
			}
		}
	};

	//==========================================
	//==========================================
	class StateBorneMessage : public StateBase{
		void Begin(void* pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle*>(pUserPtr);
			p->m_pMessageWindow->SetVisible(true);
			p->m_pMessageWindow->SetDirectMessage("モンスターがうまれたね。\n名前をつけてあげよう！");
		}

		void Update(void* pUserPtr) override{
			auto p = reinterpret_cast<SceneTitle*>(pUserPtr);
			if(p->m_pMessageWindow->IsNextMessage()){
				p->m_pMessageWindow->SetVisible(false);
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
				if(strlen(charName) <= 0){
					ChangeState(SceneTitle::eState_WaitKeyboardEnable);
				}
				else {
					AppParam::Get()->SetCharaName(charName);
					ChangeState(SceneTitle::eState_StartNearby);
				}
			}
		}
	};

	//==========================================
	//==========================================
	class StateStartNearby : public StateBase{
		void Begin(void* pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle*>(pUserPtr);
			auto pBtnMgr = p->m_aButtonManager[SceneTitle::eBTN_MANAGER_ROOM];
			pBtnMgr->SetVisible(true);
			pBtnMgr->Reset();
			pBtnMgr->Unlock();
		}
		void Update(void* pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle*>(pUserPtr);
			auto pBtnMgr = p->m_aButtonManager[SceneTitle::eBTN_MANAGER_ROOM];
			const char* pCharaName = AppParam::Get()->GetCharaName();
			const int nDecide = pBtnMgr->GetDecide();
			bool bRet = false;
			if(nDecide == SceneTitle::eBTN_MAKE_ROOM){
				Engine::GetEngine()->StartNearbyAdvertising(pCharaName);
				TransferManager::Get()->Initialize(true);
				DEBUG_LOG("親NEarby\n");
				bRet = true;
			}
			else if(nDecide == SceneTitle::eBTN_JOIN_ROOM){
				Engine::GetEngine()->StartNearbyDiscovery(pCharaName);
				TransferManager::Get()->Initialize(false);
				DEBUG_LOG("子NEarby\n");
				bRet = true;
			}
			if(bRet){
				pBtnMgr->Lock();
				pBtnMgr->SetVisible(false);
				ChangeState(SceneTitle::eState_WaitNearbyConnect);
			}
		}
	};

	//==========================================
	//==========================================
	class StateWaitNearbyConnect : public StateBase {
		void Begin(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle *>(pUserPtr);
			m_fTime = 0.0f;
			m_bConnectStart = false;
			p->m_pMessageWindow->SetVisible(true);
			p->m_pMessageWindow->SetDirectMessage("接続中・");
		}

		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle *>(pUserPtr);
			if (m_fTime >= 3.0f) {
				m_fTime = 0.0f;
			} else if (m_fTime >= 2.0f) {
				p->m_pMessageWindow->SetDirectMessage("接続中・・・");
			} else if (m_fTime >= 1.0f) {
				p->m_pMessageWindow->SetDirectMessage("接続中・・");
			} else {
				p->m_pMessageWindow->SetDirectMessage("接続中・");
			}
			m_fTime += Engine::GetEngine()->GetDeltaTime();


			if(TransferManager::Get()->GetConnectNum() > 0) {
				auto pTransfer = TransferManager::Get()->GetTransfer<TransferBase>(TransferManager::eTransferKind_Connect);
				// 通信開始
				if (!m_bConnectStart) {
					TransferManager::Get()->StartTransfer(TransferManager::eTransferKind_Connect);
					m_bConnectStart = true;
				}
				// 通信接続完了
				else if (pTransfer->IsEnd()) {
					pTransfer->Dump();
					Engine::GetEngine()->StopNearbyAdvertising();
					Engine::GetEngine()->StopNearbyDiscovery();
					TransferManager::Get()->StartTransfer(TransferManager::eTransferKind_PlayerId);
					ChangeState(SceneTitle::eState_WaitNearbyTransferPlayerId);
				}
			}
		}

		bool m_bConnectStart;
		float m_fTime;
	};

	//==========================================
	//==========================================
	class StateWaitNearbyTransferPlayerId : public StateBase {
		void Begin(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle *>(pUserPtr);
			m_fTime = 0.0f;
			p->m_pMessageWindow->SetVisible(true);
			p->m_pMessageWindow->SetDirectMessage("通信中・");
		}

		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneTitle *>(pUserPtr);
			if (m_fTime >= 3.0f) {
				m_fTime = 0.0f;
			} else if (m_fTime >= 2.0f) {
				p->m_pMessageWindow->SetDirectMessage("通信中・・・");
			} else if (m_fTime >= 1.0f) {
				p->m_pMessageWindow->SetDirectMessage("通信中・・");
			} else {
				p->m_pMessageWindow->SetDirectMessage("通信中・");
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
	class StateShowLicense : public StateBase {
		void Begin(void* pUserPtr) override {
			Engine::GetEngine()->ShowLicense();
		}

		void Update(void* pUserPtr) override {
			if(!Engine::GetEngine()->IsShowLicense()){
				ChangeState(SceneTitle::eState_WaitPressButton);
			}
		}
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
, m_pBg(nullptr)
, m_pEgg(nullptr)
, m_pChara(nullptr)
, m_pTitleImage(nullptr)
, m_aButtonManager()
, m_pMessageWindow(nullptr)
, m_pStateManager(nullptr)
{
	DEBUG_LOG("Scene Title Constructor");
}

//------------------------------------------
//------------------------------------------
SceneTitle::~SceneTitle()
{
	for(auto& it : m_aButtonManager){
		delete it;
	}
	delete m_pMessageWindow;
	delete m_pTitleImage;
	delete m_pChara;
	delete m_pEgg;
	delete m_pBg;
	delete m_pStateManager;
}

//------------------------------------------
//------------------------------------------
void SceneTitle::SceneSetup() {
	Super::SceneSetup();
	DEBUG_LOG("Title Call Setup");
	TransferManager::Get()->ResetConnect();
	{
		m_pBg = new BackGround();
		m_pBg->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pEgg = new Egg();
		m_pEgg->Update(eGameMessage_Setup, nullptr);
		m_pEgg->SetPosition(0, -80, 0);
	}
	{
		const int nCharaNo = Random::GetInt(0, AppCharaList::Get()->GetCharaListSize() - 1);
		m_pChara = new Character(nCharaNo);
		m_pChara->Update(eGameMessage_Setup, nullptr);
		m_pChara->SetVisible(false);
		m_pChara->SetPosition(0, -150.0f, 0);
		TransferManager::Get()->SetSelfCharaId(nCharaNo);	// 自分のキャラとしてひとまず設定
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
		{
			ButtonManager *pButtonManager = new ButtonManager();
			std::pair<const char *, VEC3> btnList[] = {
					{"image/playStyle_2.png",    VEC3(0.0f, -400.0f, 0)},
					{"image/button_tutorial.png", VEC3(900.0f, 400.0f, 0)},
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				auto btn = pButtonManager->CreateButton(btnList[i].first);
				btn->SetPosition(btnList[i].second);
				btn->SetSelectSELabel(SE_LABEL_DECIDE);
			}
			pButtonManager->SetVisible(false);
			pButtonManager->Lock();
			m_aButtonManager.push_back(pButtonManager);
		}
		{
			ButtonManager *pButtonManager = new ButtonManager();
			std::pair<const char *, VEC3> btnList[] = {
					{"image/button_make_room.png",    VEC3(-500.0f, -400.0f, 0)},
					{"image/button_join_room.png", VEC3(500.0f, -400.0f, 0)},
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				auto btn = pButtonManager->CreateButton(btnList[i].first);
				btn->SetPosition(btnList[i].second);
				btn->SetSelectSELabel(SE_LABEL_DECIDE);
			}
			pButtonManager->SetVisible(false);
			pButtonManager->Lock();
			m_aButtonManager.push_back(pButtonManager);
		}
	}
	{
		m_pMessageWindow = new MessageWindow("image/message_window.png");
		m_pMessageWindow->Update(eGameMessage_Setup, nullptr);
		m_pMessageWindow->SetVisible(false);
		m_pMessageWindow->SetTextScale(1.5f);
		m_pMessageWindow->SetDecideSELabel(SE_LABEL_DECIDE);
		m_pMessageWindow->SetActive(true);
	}
	{
		m_pStateManager = new StateManager(eState_Max);
		m_pStateManager->SetUserPtr(this);
		m_pStateManager->CreateState<StateFadeIn>();
		m_pStateManager->CreateState<StateWaitPressButton>();
		m_pStateManager->CreateState<StateBorneStartMonster>();
		m_pStateManager->CreateState<StateBorneFadeOut>();
		m_pStateManager->CreateState<StateBorneFadeIn>();
		m_pStateManager->CreateState<StateBorneMessage>();
		m_pStateManager->CreateState<StateWaitKeyboardEnable>();
		m_pStateManager->CreateState<StateWaitInput>();
		m_pStateManager->CreateState<StateStartNearby>();
		m_pStateManager->CreateState<StateWaitNearbyConnect>();
		m_pStateManager->CreateState<StateWaitNearbyTransferPlayerId>();
		m_pStateManager->CreateState<StateShowLicense>();
		m_pStateManager->CreateState<StateFadeOut>();
		m_pStateManager->ChangeState(eState_FadeIn);
	}
	Engine::GetEngine()->GetSoundManager()->PlayBGM(BGM_LABEL_MAIN, true);
	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void SceneTitle::SceneUpdate() {
	Super::SceneUpdate();
	//DEBUG_LOG("Launcher Call Update");


	m_pStateManager->Update();
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
		case eGameMessage_Destroy:
		{
			SceneFinalize();
			break;
		}
		default:
		{
			break;
		}
	}

	if(message != eGameMessage_Setup) {
		m_pBg->Update(message, param);
		m_pEgg->Update(message, param);
		m_pChara->Update(message, param);
		m_pTitleImage->Update(message, param);
		for(auto& it : m_aButtonManager) {
			it->Update(message, param);
		}
		m_pMessageWindow->Update(message, param);
	}
}
