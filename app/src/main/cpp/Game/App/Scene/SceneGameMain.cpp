//
// Created by 葵ユメ on 2018/09/19.
//


#include <LayoutComponent.h>
#include "SceneGameMain.h"
#include "SceneSpendTime.h"
#include "SceneWork.h"
#include <EntityManager.h>
#include <Button/ButtonManager.h>
#include <Button/SimpleButton.h>
#include <SceneManager.h>
#include <Character.h>
#include <InformationPlate.h>
#include <MessageWindow/MessageWindow.h>
#include <../Net/TransferManager.h>
#include <../Net/TransferGameInfo.h>
#include <../Net/TransferTouchInfo.h>
#include <AppParam.h>
#include <AppSkillList.h>
#include <Adv.h>
#include <Shop.h>
#include <UseItem.h>
#include <Random.h>

SceneBase* SceneGameMain::CreateScene()
{
	return new SceneGameMain();
}

//==========================================
//==========================================
void StateGameMain::Begin(void* pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_Main];
	pBtn->SetControlPlayerId(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId);
	pBtn->Unlock();
	pBtn->SetVisible(true);
	// プレイヤーIDが更新されていなかった。名前が更新するため呼び出す
	p->m_pInformationPlate->UpdatePlate();
	DEBUG_LOG("State GameMain Begin\n");
}
void StateGameMain::Update(void* pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_Main];

	switch (pBtn->GetDecide()){
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
		default:{ break; }
	}
}
void StateGameMain::End(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_Main];
	pBtn->Lock();
	pBtn->Reset();
	pBtn->SetVisible(false);
}

//==========================================
//==========================================
void StateAdv::Begin(void *pUserPtr)
{
	DEBUG_LOG("StateAdv");
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	p->m_pAdv->Open();
}
void StateAdv::Update(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	if(p->m_pAdv->IsEnd()) {
		ChangeState(SceneGameMain::eState_GameMain);
	}
}
void StateAdv::End(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_Main]->GetButton(0);
	pBtn->Disable();
	pBtn->SetGray(true);
	p->m_pAdv->Close();
}

//==========================================
//==========================================
void StateUseOrShopSelect::Begin(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_ItemOrShop];
	pBtn->SetControlPlayerId(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId);
	pBtn->Unlock();
	pBtn->SetVisible(true);
}
void StateUseOrShopSelect::Update(void* pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_ItemOrShop];

	switch (pBtn->GetDecide()){
		case eBtn_Use: {
			ChangeState(SceneGameMain::eState_UseItem);
			break;
		}
		case eBtn_Shop: {
			ChangeState(SceneGameMain::eState_Shop);
			break;
		}
		default:{ break; }
	}
}
void StateUseOrShopSelect::End(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	auto pBtn = p->m_aBtnManager[SceneGameMain::eBtnKind_ItemOrShop];
	pBtn->Lock();
	pBtn->Reset();
	pBtn->SetVisible(false);
}

//==========================================
//==========================================
void StateShop::Begin(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	p->m_pShop->Open();
}
void StateShop::Update(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	if(p->m_pShop->IsEnd()) {
		ChangeState(SceneGameMain::eState_GameMain);
	}
}
void StateShop::End(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	p->m_pShop->Close();
}

//==========================================
//==========================================
void StateUseItem::Begin(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	p->m_pUseItem->Open();
}
void StateUseItem::Update(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	if(p->m_pUseItem->IsEnd()) {
		ChangeState(SceneGameMain::eState_GameMain);
	}
}
void StateUseItem::End(void *pUserPtr)
{
	auto p = reinterpret_cast<SceneGameMain*>(pUserPtr);
	p->m_pUseItem->Close();
}

//==========================================
//==========================================
void StateNextPlayer::Begin(void *pUserPtr)
{
	const int nNum = TransferManager::Get()->GetConnectNum() + 1;
	auto& networkInfo = AppParam::Get()->GetNetworkInfo();
	networkInfo.nCurrentPlayerId = (networkInfo.nCurrentPlayerId + 1) % nNum;
	networkInfo.nCurrentTurn++;
	SCENE_MANAGER()->ChangeScene(SceneGameMain::CreateScene());
}

//==========================================
//==========================================
SceneGameMain::SceneGameMain()
: SceneBase("GameMain")
, m_pCounter(nullptr)
, m_pBgImage(nullptr)
, m_pChara(nullptr)
, m_pShop(nullptr)
, m_pInformationPlate(nullptr)
, m_pMessageWindow(nullptr)
, m_aBtnManager()
{
	DEBUG_LOG("Scene Game Main Constructor");
}

//------------------------------------------
//------------------------------------------
SceneGameMain::~SceneGameMain()
{
	delete m_pCounter;
	delete m_pBgImage;
	delete m_pChara;
	delete m_pAdv;
	delete m_pShop;
	delete m_pUseItem;
	delete m_pInformationPlate;
	delete m_pMessageWindow;
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
		float x = -Engine::GetEngine()->GetScreenInfo().m_nScreenX * 0.5 + 100;
		float y = Engine::GetEngine()->GetScreenInfo().m_nScreenY * 0.5f - 100;
		m_pCounter->SetPosition(x, y, 0);
	}
	{
		m_pBgImage = new Entity();
		Entity::CreateLayoutComponent(m_pBgImage, "image/monfri_bg.png");
		auto pLayoutComponent = (LayoutComponent *) m_pBgImage->GetComponent(eComponentKind_Layout);
		pLayoutComponent->SetOrtho(true);
		m_pBgImage->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pChara = new Character(Character::CHARA_ID::eCHARA_01);
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
		m_pStateManager->CreateState<StateGameMain>();
		m_pStateManager->CreateState<StateAdv>();
		m_pStateManager->CreateState<StateUseOrShopSelect>();
		m_pStateManager->CreateState<StateShop>();
		m_pStateManager->CreateState<StateUseItem>();
		m_pStateManager->CreateState<StateNextPlayer>();
	}
	{
		// メインボタン
		{
			auto pBtnManager = new ButtonManager();
			const std::pair<const char *, VEC3> btnList[] = {
					{"image/button_Adv.png",  VEC3(-250.0f, -400.0f, 0)},
					{"image/button_Work.png", VEC3(0.0f, -400.0f, 0)},
					{"image/button_item.png", VEC3(250.0f, -400.0f, 0)},
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				auto pBtn = pBtnManager->CreateButton(btnList[i].first);
				pBtn->SetPosition(btnList[i].second);
			}
			pBtnManager->SetVisible(false);
			pBtnManager->Lock();
			m_aBtnManager.emplace_back(pBtnManager);
		}
		// アイテム使用 or ショップボタン
		{
			auto pBtnManager = new ButtonManager();
			const std::pair<const char *, VEC3> btnList[] = {
					{"image/button_use.png",  VEC3(-250.0f, -400.0f, 0)},
					{"image/button_shop.png", VEC3(250.0f, -400.0f, 0)},
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				auto pBtn = pBtnManager->CreateButton(btnList[i].first);
				pBtn->SetPosition(btnList[i].second);
			}
			pBtnManager->SetVisible(false);
			pBtnManager->Lock();
			m_aBtnManager.emplace_back(pBtnManager);
		}
	}
	auto pManager = TransferManager::Get();
	if(pManager->IsConnectSucess()) {
		if(!pManager->IsStartTransfer(TransferManager::eTransferKind_TouchInfo)) {
			pManager->StartTransfer(TransferManager::eTransferKind_TouchInfo);
		}
	}
	SKILL_LIST()->InitializeSkillTransfer();

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
		auto pInfo = pManager->GetTransfer<TransferGameInfo>(
				TransferManager::eTransferKind_GameInfo);
		pInfo->SetGameInfoData(&AppParam::Get()->GetNetworkInfo(),
							   sizeof(AppParam::GameNetworkInfo));
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
	char str[128];
	std::snprintf(str, sizeof(str), "%d", DELAY_INPUT()->GetCurrentFrame());
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
		m_pBgImage->Update(message, param);
		m_pChara->Update(message, param);
		m_pAdv->Update(message, param);
		m_pShop->Update(message, param);
		m_pUseItem->Update(message, param);
		m_pInformationPlate->Update(message, param);
		m_pMessageWindow->Update(message, param);
		m_pCounter->Update(message, param);
		for(auto& it : m_aBtnManager) {
			it->Update(message, param);
		}
	}
}
