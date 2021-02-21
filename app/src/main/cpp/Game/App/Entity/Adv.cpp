//
// Created by 葵ユメ on 2018/10/08.
//


#include <LayoutComponent.h>
#include <TransformComponent.h>
#include "Adv.h"
#include "Button/SimpleButton.h"
#include <Character.h>
#include <InformationPlate.h>
#include <MessageWindow/MessageWindow.h>
#include <Dice.h>
#include <SceneManager.h>
#include <AppParam.h>
#include <Button/ButtonManager.h>

//------------------------------------------
//------------------------------------------
Adv::Adv()
	: GameEntity()
	, m_nStep(eSTEP_END)
	, m_nNextStep(eSTEP_END)
	, m_nStepCnt(0)
	, m_nSubStep(0)
	, m_nSubStepCnt(0)
	, m_Chara()
	, m_Enemy()
	, m_pBtnManager(nullptr)
	, m_pMessageWindow(nullptr)
{
	DEBUG_LOG("Adv Constructor");
}

//------------------------------------------
//------------------------------------------
Adv::~Adv()
{
	// m_pCharaのpCharaは外部参照
	delete m_Chara.pDice;
	delete m_Enemy.pChara;
	delete m_Enemy.pDice;
	delete m_pBtnManager;
}

//------------------------------------------
//------------------------------------------
void Adv::Open()
{
	{
		m_Chara.pChara->SetPosition(-350.0f, -150.0f, 0);
		m_Chara.pDice->InVisible();
		m_Chara.prePos = m_Chara.pChara->GetPosition();
		m_Chara.pDice->SetPosition(VEC3(-350.0f, 90.0f, 0));
	}
	{
		m_Enemy.pChara->SetPosition(350.0f, -150.0f, 0);
		m_Enemy.pChara->SetScale(VEC3(-1.0f, 1.0f, 1.0f));
		m_Enemy.pChara->SetVisible(true);
		m_Enemy.pDice->InVisible();
		m_Enemy.pDice->SetPosition(VEC3(350.0f, 90.0f, 0));
	}
	{
		auto pBtn = m_pBtnManager->GetButton(0);
		pBtn->Disable();
		pBtn->SetVisible(false);
		pBtn->SetPosition(VEC3(0, -400.0f, 0));
		m_pBtnManager->SetControlPlayerId(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId);
		m_pBtnManager->Reset();
		m_pBtnManager->Unlock();
	}
	{
		m_pMessageWindow->SetTextScale(1.5f);
		m_pMessageWindow->SetDirectMessage("敵のスライムが現れた！");
		m_pMessageWindow->SetVisible(true);
		m_pMessageWindow->SetControlPlayerId(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId);
	}
	m_nStep = eSTEP_APPEAR_ENEMY;
	m_nNextStep = eSTEP_APPEAR_ENEMY;
	m_nStepCnt = 0;
	m_nSubStep = 0;
	m_nSubStepCnt = 0;
}

//------------------------------------------
//------------------------------------------
void Adv::Close()
{
	m_Chara.pChara->SetPosition(m_Chara.prePos);
	m_Chara.pDice->InVisible();
	m_Enemy.pChara->SetVisible(false);
	m_Enemy.pDice->InVisible();
	m_pBtnManager->SetVisible(false);
	m_pBtnManager->Reset();
	m_pBtnManager->Lock();
	m_pMessageWindow->SetTextScale(1.0f);
	m_pMessageWindow->SetVisible(false);
}

//------------------------------------------
//------------------------------------------
bool Adv::IsEnd() const
{
	return m_nStep == eSTEP_END;
}

//------------------------------------------
//------------------------------------------
void Adv::SetCharacter(Character* pChara)
{
	m_Chara.pChara = pChara;
}

//------------------------------------------
//------------------------------------------
void Adv::SetMessageWindow(MessageWindow* pMessageWindow)
{
	m_pMessageWindow = pMessageWindow;
}

//------------------------------------------
//------------------------------------------
void Adv::GameEntitySetup(const void* param) {
	Super::GameEntitySetup(param);
	DEBUG_LOG("Adv Call Setup");

	{
		m_Chara.pDice = new Dice();
		m_Chara.pDice->Update(eGameMessage_Setup, nullptr);
		m_Chara.pDice->InVisible();
	}
	{
		// TODO 敵をランダム設定
		m_Enemy.pChara = new Character(Character::eCHARA_01);
		m_Enemy.pChara->Update(eGameMessage_Setup, nullptr);
		m_Enemy.pChara->SetVisible(false);

		m_Enemy.pDice = new Dice();
		m_Enemy.pDice->Update(eGameMessage_Setup, nullptr);
		m_Enemy.pDice->InVisible();
	}
	{
		m_pBtnManager = new ButtonManager();
		m_pBtnManager->CreateButton("image/button_dice.png");
		m_pBtnManager->SetVisible(false);
		m_pBtnManager->Lock();
	}
	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void Adv::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);
	//DEBUG_LOG("Launcher Call Update");

	switch(m_nStep){
		case eSTEP_APPEAR_ENEMY:{ updateAppearEnemyStep(); break; }
		case eSTEP_BATTLE:{ updateBattleStep(); break; }
		case eSTEP_END:{ /*何もしない*/ break; }
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
void Adv::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);

	if(message != eGameMessage_Setup) {
		m_Enemy.pChara->Update(message, param);
		m_Chara.pDice->Update(message, param);
		m_Enemy.pDice->Update(message, param);
		m_pBtnManager->Update(message, param);
	}
}

void Adv::updateAppearEnemyStep()
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

void Adv::updateBattleStep()
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
				m_Chara.pDice->ResetStopDice();
				m_Enemy.pDice->ResetStopDice();
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
			m_nNextStep = eSTEP_END;
		}
	}

	if(nPrevSubStep == m_nSubStep && m_nNextStep == m_nStep){
		m_nSubStepCnt++;
	}
	else{
		m_nSubStepCnt = 0;
	}
}
