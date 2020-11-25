//
// Created by 葵ユメ on 2018/10/08.
//


#include <LayoutComponent.h>
#include <TransformComponent.h>
#include "SceneAdv.h"
#include "Button/SimpleButton.h"
#include <InformationPlate.h>
#include <MessageWindow/MessageWindow.h>
#include <Dice.h>

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
, m_pChara(nullptr)
, m_pBtnManager(nullptr)
, m_pInformationPlate(nullptr)
, m_pMessageWindow(nullptr)
, m_pDice(nullptr)
{
	DEBUG_LOG("Scene Adv Constructor");
}

//------------------------------------------
//------------------------------------------
SceneAdv::~SceneAdv()
{
	delete m_pBgImage;
	delete m_pChara;
	delete m_pBtnManager;
	delete m_pInformationPlate;
	delete m_pMessageWindow;
	delete m_pDice;
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
		m_pChara = new Entity();
		Entity::CreateLayoutComponent(m_pChara, "image/chara_01.png");
		auto pLayoutComponent = (LayoutComponent *) m_pChara->GetComponent(eComponentKind_Layout);
		pLayoutComponent->SetOrtho(true);
		m_pChara->Update(eGameMessage_Setup, nullptr);
		m_pChara->SetPosition(0, -150.0f, 0);
	}
	{
		m_pBtnManager = new ButtonManager();
		auto pBtn = m_pBtnManager->CreateButton("image/button_dice.png");
		pBtn->Disable();
		pBtn->SetVisible(false);
		pBtn->SetPosition(VEC3(0, -400.0f, 0));
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
	}
	{
		m_pDice = new Dice();
		m_pDice->Update(eGameMessage_Setup, nullptr);
		m_pDice->InVisible();
		m_pDice->SetPosition(VEC3(0, 90.0f, 0));
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
		m_pChara->Update(message, param);
		m_pBtnManager->Update(message, param);
		m_pInformationPlate->Update(message, param);
		m_pMessageWindow->Update(message, param);
		m_pDice->Update(message, param);
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
		Engine::GetEngine()->ResetTouchEvent();
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
		Engine::GetEngine()->ResetTouchEvent();
		m_pDice->SetVisible(0, true);
		m_pDice->StartDice();
		m_nSubStep = 1;
	}
	else if(m_nSubStep == 1){
		if(m_pBtnManager->GetDecide() == 0){
			auto pBtn = m_pBtnManager->GetButton(0);
			pBtn->SetVisible(false);
			pBtn->Disable();
			m_pBtnManager->Reset();
			m_pDice->StopDice();
			m_nSubStep = 2;
		}
	}
	else if(m_nSubStep == 2){
	}

	if(nPrevSubStep == m_nSubStep && m_nNextStep == m_nStep){
		m_nSubStepCnt++;
	}
	else{
		m_nSubStepCnt = 0;
	}
}
