//
// Created by 葵ユメ on 2018/10/08.
//


#include <LayoutComponent.h>
#include <TransformComponent.h>
#include "SceneAdv.h"
#include "Button/SimpleButton.h"
#include "SceneGameMain.h"
#include <Character.h>
#include <InformationPlate.h>
#include <MessageWindow/MessageWindow.h>
#include <Dice.h>
#include <SceneManager.h>
#include <AppParam.h>

SceneBase* SceneAdv::CreateScene()
{
	return new SceneAdv();
}

//------------------------------------------
//------------------------------------------
SceneAdv::SceneAdv()
: SceneBase("Adv")
, m_nStep(0)
, m_nNextStep(0)
, m_nStepCnt(0)
, m_nSubStep(0)
, m_nSubStepCnt(0)
, m_pBgImage(nullptr)
, m_Chara()
, m_Enemy()
, m_pBtnManager(nullptr)
, m_pInformationPlate(nullptr)
, m_pMessageWindow(nullptr)
{
	DEBUG_LOG("Scene Adv Constructor");
}

//------------------------------------------
//------------------------------------------
SceneAdv::~SceneAdv()
{
	delete m_pBgImage;
	delete m_pBtnManager;
	delete m_pInformationPlate;
	delete m_pMessageWindow;
}

//------------------------------------------
//------------------------------------------
void SceneAdv::SceneSetup() {
	Super::SceneSetup();
	DEBUG_LOG("Adv Call Setup");
	{
		m_pBgImage = new Entity();
		Entity::CreateLayoutComponent(m_pBgImage, "image/monfri_bg.png");
		auto pLayoutComponent = (LayoutComponent *) m_pBgImage->GetComponent(eComponentKind_Layout);
		pLayoutComponent->SetOrtho(true);
		m_pBgImage->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_Chara.pChara = new Character(Character::eCHARA_01);
		m_Chara.pChara->Update(eGameMessage_Setup, nullptr);
		m_Chara.pChara->SetPosition(-350.0f, -150.0f, 0);
		m_Chara.pDice = new Dice();
		m_Chara.pDice->Update(eGameMessage_Setup, nullptr);
		m_Chara.pDice->InVisible();
		m_Chara.pDice->SetPosition(VEC3(-350.0f, 90.0f, 0));
	}
	{
		// TODO 敵をランダム設定
		m_Enemy.pChara = new Character(Character::eCHARA_01);
		m_Enemy.pChara->Update(eGameMessage_Setup, nullptr);
		m_Enemy.pChara->SetPosition(350.0f, -150.0f, 0);
		m_Enemy.pChara->SetScale(VEC3(-1.0f, 1.0f, 1.0f));
		m_Enemy.pDice = new Dice();
		m_Enemy.pDice->Update(eGameMessage_Setup, nullptr);
		m_Enemy.pDice->InVisible();
		m_Enemy.pDice->SetPosition(VEC3(350.0f, 90.0f, 0));
	}
	{
		m_pBtnManager = new ButtonManager();
		auto pBtn = m_pBtnManager->CreateButton("image/button_dice.png");
		pBtn->Disable();
		pBtn->SetVisible(false);
		pBtn->SetPosition(VEC3(0, -400.0f, 0));
		m_pBtnManager->SetControlPlayerId(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId);
	}
	{
		m_pInformationPlate = new InformationPlate();
		m_pInformationPlate->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pMessageWindow = new MessageWindow("image/message_window.png");
		m_pMessageWindow->Update(eGameMessage_Setup, nullptr);
		m_pMessageWindow->SetTextScale(1.5f);
		m_pMessageWindow->SetDirectMessage("敵のスライムが現れた！");
		m_pMessageWindow->SetVisible(true);
		m_pMessageWindow->SetControlPlayerId(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId);
	}
	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void SceneAdv::SceneUpdate() {
	Super::SceneUpdate();
	//DEBUG_LOG("Launcher Call Update");

	switch(m_nStep){
		case eSTEP_APPEAR_ENEMY:{ updateAppearEnemyStep(); break; }
		case eSTEP_BATTLE:{ updateBattleStep(); break; }
		default:{ break; }
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
void SceneAdv::SceneFinalize()
{
	Super::SceneFinalize();
	DEBUG_LOG("Title Call Finalize");
}

//------------------------------------------
//------------------------------------------
void SceneAdv::EntityUpdate(GameMessage message, const void* param)
{
	SceneBase::EntityUpdate(message, param);

	if(message != eGameMessage_Setup) {
		m_pBgImage->Update(message, param);
		m_Chara.pChara->Update(message, param);
		m_Enemy.pChara->Update(message, param);
		m_Chara.pDice->Update(message, param);
		m_Enemy.pDice->Update(message, param);
		m_pBtnManager->Update(message, param);
		m_pInformationPlate->Update(message, param);
		m_pMessageWindow->Update(message, param);
	}
}

void SceneAdv::updateAppearEnemyStep()
{
	const int nPrevSubStep = m_nSubStep;
	if(m_nSubStep == 0){
		if(m_pMessageWindow->IsNextMessage()){
			m_nSubStep = 1;
		}
	}
	else if(m_nSubStep == 1){
		m_pMessageWindow->SetDirectMessage("サイコロで勝負！");
//		Engine::GetEngine()->ResetTouchEvent();
		m_nSubStep = 2;
	}
	else if(m_nSubStep == 2){
		if(m_pMessageWindow->IsNextMessage()) {
			m_pMessageWindow->SetVisible(false);
			m_nSubStep = 0;
			m_nNextStep = eSTEP_BATTLE;
		}
	}

	if(nPrevSubStep == m_nSubStep && m_nNextStep == m_nStep){
		m_nSubStepCnt++;
	}
	else{
		m_nSubStepCnt = 0;
	}
}

void SceneAdv::updateBattleStep()
{
	const int nPrevSubStep = m_nSubStep;
	if(m_nSubStep == 0){
		auto pBtn = m_pBtnManager->GetButton(0);
		pBtn->SetVisible(true);
		pBtn->Enable();
		m_pBtnManager->Reset();
//		Engine::GetEngine()->ResetTouchEvent();
		m_Chara.pDice->SetVisible(0, true);
		m_Chara.pDice->StartDice();
		m_Enemy.pDice->SetVisible(0, true);
		m_Enemy.pDice->StartDice();
		m_nSubStep = 1;
	}
	else if(m_nSubStep == 1){
		if(m_pBtnManager->GetDecide() == 0){
			auto pBtn = m_pBtnManager->GetButton(0);
			pBtn->SetVisible(false);
			pBtn->Disable();
			m_pBtnManager->Reset();
			m_Chara.pDice->StopDice();
			m_Enemy.pDice->StopDice();
			m_nSubStep = 2;
		}
	}
	else if(m_nSubStep == 2){
		if(m_nSubStepCnt >= 60){
			const auto chDice = m_Chara.pDice->GetDice();
			const auto eneDice = m_Enemy.pDice->GetDice();
			char cText[256];
			if(chDice > eneDice) {
				snprintf(cText, sizeof(cText), "%d対%dできみの勝ち！", chDice, eneDice);
			}
			else if(chDice < eneDice){
				snprintf(cText, sizeof(cText), "%d対%dできみの負け！", chDice, eneDice);
			}
			else {
				snprintf(cText, sizeof(cText), "%d対%dで引き分け！　もう一度勝負！", chDice, eneDice);
			}
			cText[sizeof(cText) - 1] = '\0';
			m_pMessageWindow->SetDirectMessage(cText);
			m_pMessageWindow->SetVisible(true);
//			Engine::GetEngine()->ResetTouchEvent();
			m_nSubStep = 3;
		}
	}
	else if(m_nSubStep == 3){
		if(m_pMessageWindow->IsNextMessage()) {
			if(m_Chara.pDice->GetDice() == m_Enemy.pDice->GetDice()) {
				m_nSubStep = 0;
				m_pMessageWindow->SetVisible(false);
			}
			else {
				m_nSubStep = 4;
			}
		}
	}
	else if(m_nSubStep == 4){
		// TODO キズナ値調整
		m_pMessageWindow->SetDirectMessage("キズナが10増えた！");
		AppParam::Get()->AddKizunaPoint(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId, 10);
//		Engine::GetEngine()->ResetTouchEvent();
		m_nSubStep = 5;
	}
	else if(m_nSubStep == 5){
		if(m_pMessageWindow->IsNextMessage()) {
			SCENE_MANAGER()->ChangeScene(SceneGameMain::CreateScene());
		}
	}

	if(nPrevSubStep == m_nSubStep && m_nNextStep == m_nStep){
		m_nSubStepCnt++;
	}
	else{
		m_nSubStepCnt = 0;
	}
}
