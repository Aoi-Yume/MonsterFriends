//
// Created by 葵ユメ on 2021/03/05.
//


#include <LayoutComponent.h>
#include <AnimationComponent.h>
#include "SceneResult.h"
#include "SceneTitle.h"
#include <EntityManager.h>
#include <Button/ButtonManager.h>
#include <Button/SimpleButton.h>
#include <FadeCtrl.h>
#include <SceneManager.h>
#include <Character.h>
#include <../Net/TransferManager.h>
#include <../Net/TransferGameInfo.h>
#include <../Net/TransferTouchInfo.h>
#include <AppParam.h>
#include <AppItemList.h>

SceneBase* SceneResult::CreateScene()
{
	return new SceneResult();
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
				ChangeState(SceneResult::eState_ResultMain);
			}
		}
	};

	//==========================================
	//==========================================
	class StateResultMain : public StateBase {
		void Begin(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneResult *>(pUserPtr);
			DEBUG_LOG("State Result Begin\n");
			m_nStep = 0;
			m_nNextStep = 0;
			m_nPlayer = 0;
			m_nItemNo = 0;
			m_fTime = 0.0f;
		}

		void Update(void *pUserPtr) override {
			auto p = reinterpret_cast<SceneResult *>(pUserPtr);
			auto child = reinterpret_cast<Entity*>(p->m_CharaInfo[m_nPlayer].pChara->GetChild(0));

			if(m_nStep == 0){
				if(m_fTime <= 0.0f){
					auto anim = reinterpret_cast<AnimationComponent*>(child->GetComponent(eComponentKind_Animation));
					auto layout = reinterpret_cast<LayoutComponent*>(child->GetComponent(eComponentKind_Layout));
					anim->Play("Open");
					layout->SetVisible(true);
				}
				else if(m_fTime >= 0.4f){
					m_nNextStep = 1;
					m_nItemNo = 0;
				}
			}
			else if(m_nStep == 1){
				if(m_fTime <= 0.0f){
					if(m_nItemNo >= p->m_CharaInfo[m_nPlayer].nCristalNum){
						m_nNextStep = 3;
					}
					else {
						auto pIcon = reinterpret_cast<Entity*>(child->GetChild(m_nItemNo));
						auto anim = reinterpret_cast<AnimationComponent *>(pIcon->GetComponent(eComponentKind_Animation));
						anim->Play("Open");
						pIcon->SetVisible(true);
						m_nNextStep = 2;
					}
				}
			}
			else if(m_nStep == 2){
				if(m_fTime >= 0.2f){
					m_nItemNo++;
					m_nNextStep = 1;
				}
			}
			else if(m_nStep == 3){
				m_nPlayer++;
				if(m_nPlayer >= TransferManager::Get()->GetConnectNum() + 1){
					m_nNextStep = 4;
				}
				else {
					m_nNextStep = 0;
					m_nItemNo = 0;
				}
			}
			else if(m_nStep == 4){
				if(m_fTime >= 3.0f){
					ChangeState(SceneResult::eState_FadeOut);
				}
			}

			if(m_nNextStep == m_nStep) {
				m_fTime += Engine::GetEngine()->GetDeltaTime();
			}
			else{
				m_nStep = m_nNextStep;
				m_fTime = 0.0f;
			}
		}

		int m_nStep;
		int m_nNextStep;
		int m_nPlayer;
		int m_nItemNo;
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
				auto pManager = TransferManager::Get();
				if (pManager->IsConnectSucess()) {
					Engine::GetEngine()->DisconnectNearbyAllEndPoint();
					pManager->StopAllTransfer();
					pManager->SetConnectSuccess(false);
				}
				SCENE_MANAGER()->ChangeScene(SceneTitle::CreateScene());
			}
		}
	};
}

//==========================================
//==========================================
SceneResult::SceneResult()
: SceneBase("Reult")
, m_pBgImage(nullptr)
, m_CharaInfo()
, m_pStateManager(nullptr)
, m_aBtnManager()
{
	DEBUG_LOG("Scene Result Constructor");
}

//------------------------------------------
//------------------------------------------
SceneResult::~SceneResult()
{
	delete m_pBgImage;
	for(int i = 0; i < NET_CONNECT_MAX; ++i) {
		delete m_CharaInfo[i].pChara;
	}
	delete m_pStateManager;
	for(auto& it : m_aBtnManager) {
		delete it;
	}
}

//------------------------------------------
//------------------------------------------
void SceneResult::SceneSetup() {
	Super::SceneSetup();
	DEBUG_LOG("Result Call Setup");
	{
		m_pBgImage = new Entity();
		Entity::CreateLayoutComponent(m_pBgImage, "image/monfri_bg.png");
		auto pLayoutComponent = (LayoutComponent *) m_pBgImage->GetComponent(eComponentKind_Layout);
		pLayoutComponent->SetOrtho(true);
		m_pBgImage->Update(eGameMessage_Setup, nullptr);
	}
	{
		const float fScreenX = (float)Engine::GetEngine()->GetScreenInfo().m_nScreenX;
		const float fAddX = fScreenX / (TransferManager::Get()->GetConnectNum() + 2);
		const float fStartX = -fScreenX * 0.5f;
		for(int i = 0; i < NET_CONNECT_MAX; ++i) {
			const bool bVisible = (i < TransferManager::Get()->GetConnectNum() + 1);
			m_CharaInfo[i].pChara = new Character(Character::CHARA_ID::eCHARA_01);

			auto pEntity = new Entity();
			{
				Entity::CreateLayoutComponent(pEntity, "image/hukidasi.png");
				Entity::CreateAnimationComponent(pEntity)->AddAnimation("Open", new SimpleOpenAnimation());
				auto pLayout = reinterpret_cast<LayoutComponent *>(pEntity->GetComponent(eComponentKind_Layout));
				pLayout->SetOrtho(true);
			}
			{
				for(int j = 0; j < 3; ++j) {
					auto icon_Entity = new Entity();
					Entity::CreateLayoutComponent(icon_Entity, "image/icon_kizuna_cristal.png");
					Entity::CreateAnimationComponent(icon_Entity)->AddAnimation("Open", new SimpleOpenAnimation());
					auto pLayout = reinterpret_cast<LayoutComponent *>(icon_Entity->GetComponent(eComponentKind_Layout));
					pLayout->SetOrtho(true);
					pEntity->AddChild(icon_Entity);
				}
			}
			m_CharaInfo[i].pChara->AddChild(pEntity);
			m_CharaInfo[i].pChara->Update(eGameMessage_Setup, nullptr);
			m_CharaInfo[i].pChara->SetVisible(bVisible);
			pEntity->SetVisible(false);
			{
				const int nItemNo = AppItemList::Get()->GetItemNoFromName("キズナクリスタル");
				m_CharaInfo[i].nCristalNum = AppParam::Get()->GetItemNum(i, nItemNo);

				const float fWidth = (128.0f + 8.0f) * 3.0f;
				const float fIconStartX = -fWidth * 0.5f;
				const float fIconAddX = fWidth / (float)(m_CharaInfo[i].nCristalNum + 1.0f);
				for(int j = 0; j < 3; ++j) {
					auto pIcon = reinterpret_cast<Entity *>(m_CharaInfo[i].pChara->GetChild(0)->GetChild(j));
					pIcon->SetPosition(fIconStartX + (float)(j + 1) * fIconAddX, 128, 0);
					pIcon->SetVisible(false);
				}
			}
			pEntity->SetPosition(0, 128, 0);
			m_CharaInfo[i].pChara->SetPosition(fStartX + (float)(i + 1) * fAddX, -200.0f, 0);
		}
	}
	{
		m_pStateManager = new StateManager(eState_Max);
		m_pStateManager->SetUserPtr(this);
		m_pStateManager->CreateState<StateFadeIn>();
		m_pStateManager->CreateState<StateResultMain>();
		m_pStateManager->CreateState<StateFadeOut>();
		m_pStateManager->ChangeState(eState_FadeIn);
	}

	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void SceneResult::SceneSync()
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
void SceneResult::SceneUpdate()
{
	Super::SceneUpdate();
	//DEBUG_LOG("Launcher Call Update");
	m_pStateManager->Update();
}

//------------------------------------------
//------------------------------------------
void SceneResult::SceneFinalize()
{
	Super::SceneFinalize();
	DEBUG_LOG("Result Call Finalize");
}

//------------------------------------------
//------------------------------------------
void SceneResult::EntityUpdate(GameMessage message, const void* param)
{
	SceneBase::EntityUpdate(message, param);

	if(message != eGameMessage_Setup) {
		m_pBgImage->Update(message, param);
		for(int i = 0; i < NET_CONNECT_MAX; ++i) {
			m_CharaInfo[i].pChara->Update(message, param);
		}
		for(auto& it : m_aBtnManager) {
			it->Update(message, param);
		}
	}
}
