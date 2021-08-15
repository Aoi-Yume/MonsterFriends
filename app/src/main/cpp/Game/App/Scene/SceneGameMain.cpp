//
// Created by 葵ユメ on 2018/09/19.
//


#include <LayoutComponent.h>
#include "SceneGameMain.h"
#include "SceneResult.h"
#include <EntityManager.h>
#include <Button/ButtonManager.h>
#include <Button/SimpleButton.h>
#include <FadeCtrl.h>
#include <SceneManager.h>
#include <BackGround.h>
#include <Character.h>
#include <InformationPlate.h>
#include <MessageWindow/MessageWindow.h>
#include <../Net/TransferManager.h>
#include <../Net/TransferGameInfo.h>
#include <../Net/TransferTouchInfo.h>
#include <../Net/TransferCommand.h>
#include <AppParam.h>
#include <AppSkillList.h>
#include <Adv.h>
#include <Shop.h>
#include <UseItem.h>
#include <PlayerNotice.h>
#include <Random.h>

SceneBase* SceneGameMain::CreateScene()
{
	return new SceneGameMain();
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
				ChangeState(SceneGameMain::eState_GameMain);

				auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
				p->m_pPlayerNotice->SetVisible(true);
				p->m_pPlayerNotice->Open(true, 1.0f);
				// スキル更新
				SKILL_LIST()->UpdateSkill(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId, AppSkillList::eSkillTiming_SelfTurn);
			}
		}
	};

	//==========================================
	//==========================================
	class StateGameMain : public StateBase {
		enum {
			eBtn_Adv,
			eBtn_Work,
			eBtn_Item,
			eBtn_Max
		};

		void Begin(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_Main];
			pBtn->SetControlPlayerId(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId);
			pBtn->Unlock();
			pBtn->SetVisible(true);
			// プレイヤーIDが更新されていなかった。名前が更新するため呼び出す
			p->m_pInformationPlate->UpdatePlate();
			DEBUG_LOG("State GameMain Begin\n");
		}

		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_Main];

			switch (pBtn->GetDecide()) {
				case eBtn_Adv: {
					//SCENE_MANAGER()->ChangeScene(SceneAdv::CreateScene());
					ChangeState(SceneGameMain::eState_Adv);
					break;
				}
				case eBtn_Work: {
					ChangeState(SceneGameMain::eState_NextPlayer);
					break;
				}
				case eBtn_Item: {
					ChangeState(SceneGameMain::eState_SelectItemUseOrShop);
					break;
				}
				default: {
					break;
				}
			}
		}

		void End(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_Main];
			pBtn->Lock();
			pBtn->Reset();
			pBtn->SetVisible(false);
		}
	};

	//==========================================
	//==========================================
	class StateAdv : public StateBase {
		void Begin(void *pUserPtr) override {
			DEBUG_LOG("StateAdv");
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			p->m_pAdv->Open();
		}

		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			if (p->m_pAdv->IsEnd()) {
				ChangeState(SceneGameMain::eState_GameMain);
			}
		}

		void End(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_Main]->GetButton(0);
			pBtn->Disable();
			pBtn->SetGray(true);
			p->m_pAdv->Close();
		}
	};

	//==========================================
	//==========================================
	class StateUseOrShopSelect : public StateBase {
		enum {
			eBtn_Use,
			eBtn_Shop,
		};

		void Begin(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_ItemOrShop];
			pBtn->SetControlPlayerId(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId);
			pBtn->Unlock();
			pBtn->SetVisible(true);
		}

		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_ItemOrShop];

			switch (pBtn->GetDecide()) {
				case eBtn_Use: {
					ChangeState(SceneGameMain::eState_UseItem);
					break;
				}
				case eBtn_Shop: {
					ChangeState(SceneGameMain::eState_Shop);
					break;
				}
				default: {
					break;
				}
			}
		}

		void End(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_ItemOrShop];
			pBtn->Lock();
			pBtn->Reset();
			pBtn->SetVisible(false);
		}
	};

	//==========================================
	//==========================================
	class StateShop : public StateBase {
		void Begin(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			p->m_pShop->Open();
		}

		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			if (p->m_pShop->IsEnd()) {
				if (AppParam::Get()->IsClear()) {
					ChangeState(SceneGameMain::eState_ClearFademOut);
				} else {
					ChangeState(SceneGameMain::eState_GameMain);
				}
			}
		}

		void End(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			p->m_pShop->Close();
		}
	};

	//==========================================
	//==========================================
	class StateUseItem : public StateBase {
		void Begin(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			p->m_pUseItem->Open();
		}

		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			if (p->m_pUseItem->IsEnd()) {
				ChangeState(SceneGameMain::eState_GameMain);
			}
		}

		void End(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneGameMain *>(pUserPtr);
			p->m_pUseItem->Close();
		}
	};

	//==========================================
	//==========================================
	class StateNextPlayer : public StateBase {
		void Begin(void *pUserPtr) override {
			const int nNum = TransferManager::Get()->GetConnectNum() + 1;
			auto &networkInfo = AppParam::Get()->GetNetworkInfo();
			networkInfo.nCurrentPlayerId = (networkInfo.nCurrentPlayerId + 1) % nNum;
			networkInfo.nCurrentTurn++;
			FADE()->Out();
		}

		void Update(void *pUserPtr) override {
			if (FADE()->IsFadeOutEnd()) {
				SCENE_MANAGER()->ChangeScene(SceneGameMain::CreateScene());
			}
		}
	};

	//==========================================
	//==========================================
	class StateClearFadeOut : public StateBase {
		void Begin(void *pUserPtr) override {
			FADE()->Out();
		}

		void Update(void *pUserPtr) override {
			if (FADE()->IsFadeOutEnd()) {
				SCENE_MANAGER()->ChangeScene(SceneResult::CreateScene());
			}
		}
	};
}

//==========================================
//==========================================
SceneGameMain::SceneGameMain()
: SceneBase("GameMain")
, m_pCounter(nullptr)
, m_pBg(nullptr)
, m_pChara(nullptr)
, m_pShop(nullptr)
, m_pInformationPlate(nullptr)
, m_pMessageWindow(nullptr)
, m_pStateManager(nullptr)
, m_aBtnManager()
{
	DEBUG_LOG("Scene Game Main Constructor");
}

//------------------------------------------
//------------------------------------------
SceneGameMain::~SceneGameMain()
{
	delete m_pCounter;
	delete m_pBg;
	delete m_pPlayerNotice;
	delete m_pChara;
	delete m_pAdv;
	delete m_pShop;
	delete m_pUseItem;
	delete m_pInformationPlate;
	delete m_pMessageWindow;
	delete m_pStateManager;
	for(auto& it : m_aBtnManager) {
		delete it;
	}
}

//------------------------------------------
//------------------------------------------
void SceneGameMain::SceneSetup() {
	Super::SceneSetup();
	DEBUG_LOG("GameMain Call Setup");
	{
		m_pCounter = new Entity();
		Entity::CreateTextImageComponent(m_pCounter, "", 32);
		auto p = (TextImageComponent*)m_pCounter->GetComponent(eComponentKind_Layout);
		p->SetOrtho(true);
		m_pCounter->Update(eGameMessage_Setup, nullptr);
		const float x = 0;
		const float y = 1080.0f * 0.5f - 100;
		m_pCounter->SetPosition(x, y, 0);
	}
	{
		m_pBg = new BackGround();
		m_pBg->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pPlayerNotice = new PlayerNotice(PlayerNotice::eNoticeType_CurrentPlayer, AppParam::Get()->GetNetworkInfo().nCurrentPlayerId);
		m_pPlayerNotice->Update(eGameMessage_Setup, nullptr);
	}
	{
		const int uCharaId = TransferManager::Get()->GetCharaIdFromPlayerId(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId);
		m_pChara = new Character(uCharaId);
		m_pChara->Update(eGameMessage_Setup, nullptr);
		m_pChara->SetPosition(0, -150.0f, 0);
	}
	{
		m_pInformationPlate = new InformationPlate();
		m_pInformationPlate->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pMessageWindow = new MessageWindow("image/message_window.png");
		m_pMessageWindow->Update(eGameMessage_Setup, nullptr);
		m_pMessageWindow->SetTextScale(1.5f);
		m_pMessageWindow->SetDirectMessage("てすと\nててててすと");
		m_pMessageWindow->SetVisible(false);
		m_pMessageWindow->SetDecideCommand(TransferCommand::eCommand_GameMainNextMessage);
		m_pMessageWindow->SetDecideSELabel(SE_LABEL_DECIDE);
	}
	{
		m_pAdv = new Adv();
		m_pAdv->SetCharacter(m_pChara);
		m_pAdv->SetMessageWindow(m_pMessageWindow);
		m_pAdv->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pShop = new Shop();
		m_pShop->SetInformationPlate(m_pInformationPlate);
		m_pShop->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pUseItem = new UseItem();
		m_pUseItem->SetInformationPlate(m_pInformationPlate);
		m_pUseItem->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pStateManager = new StateManager(eState_Max);
		m_pStateManager->SetUserPtr(this);
		m_pStateManager->CreateState<StateFadeIn>();
		m_pStateManager->CreateState<StateGameMain>();
		m_pStateManager->CreateState<StateAdv>();
		m_pStateManager->CreateState<StateUseOrShopSelect>();
		m_pStateManager->CreateState<StateShop>();
		m_pStateManager->CreateState<StateUseItem>();
		m_pStateManager->CreateState<StateNextPlayer>();
		m_pStateManager->CreateState<StateClearFadeOut>();
		m_pStateManager->ChangeState(eState_FadeIn);
	}
	{
		// メインボタン
		{
			auto pBtnManager = new ButtonManager();
			std::tuple<const char*, VEC3, uint8_t> btnList[] = {
					{"image/button_Adv.png",  VEC3(-230.0f, -400.0f, 0), TransferCommand::eCommand_PushAdv },
					{"image/button_next.png", VEC3(0.0f, -400.0f, 0), TransferCommand::eCommand_PushNext },
					{"image/button_item.png", VEC3(230.0f, -400.0f, 0), TransferCommand::eCommand_PushItem},
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				auto pBtn = pBtnManager->CreateButton(std::get<0>(btnList[i]));
				pBtn->SetPosition(std::get<1>(btnList[i]));
				pBtn->SetScale(VEC3(0.8f, 0.8f, 0.8f));
				pBtn->SetDecideCommand(std::get<2>(btnList[i]));
				pBtn->SetSelectSELabel(SE_LABEL_DECIDE);
			}
			pBtnManager->SetVisible(false);
			pBtnManager->Lock();
			m_aBtnManager.emplace_back(pBtnManager);
		}
		// アイテム使用 or ショップボタン
		{
			auto pBtnManager = new ButtonManager();
			const std::tuple<const char *, VEC3, uint8_t> btnList[] = {
					{"image/button_bag.png",  VEC3(-250.0f, -400.0f, 0), TransferCommand::eCommand_PushBag},
					{"image/button_shop.png", VEC3(250.0f, -400.0f, 0), TransferCommand::eCommand_PushShop},
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				auto pBtn = pBtnManager->CreateButton(std::get<0>(btnList[i]));
				pBtn->SetPosition(std::get<1>(btnList[i]));
				pBtn->SetDecideCommand(std::get<2>(btnList[i]));
				pBtn->SetSelectSELabel(SE_LABEL_DECIDE);
			}
			pBtnManager->SetVisible(false);
			pBtnManager->Lock();
			m_aBtnManager.emplace_back(pBtnManager);
		}
	}
	SKILL_LIST()->InitializeSkillTransfer();

#ifdef _AOIYUME_DEBUG_
	if(auto pSoundManager = Engine::GetEngine()->GetSoundManager(); !pSoundManager->IsPlayBGM()){
		pSoundManager->PlayBGM(BGM_LABEL_MAIN, true);
	}
#endif

	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void SceneGameMain::SceneSync()
{
	SceneBase::SceneSync();

	auto pManager = TransferManager::Get();
	if(GetSyncStep() == eSyncStep_UserSync) {
		// ゲーム情報の送受信設定
		auto pInfo = pManager->GetTransfer<TransferGameInfo>(TransferManager::eTransferKind_GameInfo);
		pInfo->SetGameInfoData(&AppParam::Get()->GetNetworkInfo(), sizeof(AppParam::GameNetworkInfo));
		pInfo->SetReceiveCallBack([](void *pData) {
			auto &pParam = AppParam::Get()->GetNetworkInfo();
			pParam = *reinterpret_cast<AppParam::GameNetworkInfo *>(pData);
		});
		pInfo->SetDumpCallBack([]() { AppParam::Get()->DumpNetworkInfo(); });
		pManager->StartTransfer(TransferManager::eTransferKind_GameInfo);
		SetSyncStep(eSyncStep_UserSync + 1);
	}
	else if(GetSyncStep() == eSyncStep_UserSync + 1){
		// ゲーム情報送受信待ち
		auto pTransfer = pManager->GetTransfer<TransferBase>(TransferManager::eTransferKind_GameInfo);
		if(pTransfer->IsEnd()){
			pTransfer->Dump();
			SetSyncStep(eSyncStep_End);
		}
	}
}

//------------------------------------------
//------------------------------------------
void SceneGameMain::SceneUpdate()
{
	Super::SceneUpdate();
	//DEBUG_LOG("Launcher Call Update");
	m_pStateManager->Update();

	auto p = (TextImageComponent*)m_pCounter->GetComponent(eComponentKind_Layout);
	char str[4096];
	std::snprintf(str, sizeof(str), "Time[%.3f]", DELAY_INPUT()->GetCurrentTime());
	p->SetText(str);
	//auto pTouchInput = Engine::GetEngine()->GetTouchInputInfoPtr(0);
	//DEBUG_LOG_A("TouchEvent[%d], X[%.2f], Y[%.2f]\n", pTouchInput->nTouchEvent, pTouchInput->fTouchX, pTouchInput->fTouchY);
}

//------------------------------------------
//------------------------------------------
void SceneGameMain::SceneFinalize()
{
	Super::SceneFinalize();
	DEBUG_LOG("GameMain Call Finalize");
}

//------------------------------------------
//------------------------------------------
void SceneGameMain::EntityUpdate(GameMessage message, const void* param)
{
	SceneBase::EntityUpdate(message, param);

	if(message != eGameMessage_Setup) {
		m_pBg->Update(message, param);
		m_pChara->Update(message, param);
		m_pAdv->Update(message, param);
		m_pShop->Update(message, param);
		m_pUseItem->Update(message, param);
		m_pInformationPlate->Update(message, param);
		m_pMessageWindow->Update(message, param);
		m_pPlayerNotice->Update(message, param);
		m_pCounter->Update(message, param);
		for(auto& it : m_aBtnManager) {
			it->Update(message, param);
		}
	}
}
