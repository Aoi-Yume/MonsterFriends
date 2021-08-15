//
// Created by 葵ユメ on 2018/10/08.
//


#include <LayoutComponent.h>
#include <TransformComponent.h>
#include "Adv.h"
#include "Button/SimpleButton.h"
#include <Character.h>
#include <InformationPlate.h>
#include <Window/MessageWindow.h>
#include <Dice.h>
#include <SceneManager.h>
#include <AppParam.h>
#include <AppCharaList.h>
#include <Button/ButtonManager.h>
#include <Random.h>
#include <TransferCommand.h>

//------------------------------------------
//------------------------------------------
Adv::Adv()
	: GameEntity()
	, m_nStep(eSTEP_END)
	, m_nNextStep(eSTEP_END)
	, m_nStepCnt(0)
	, m_nSubStep(0)
	, m_nSubStepCnt(0)
	, m_pChara(nullptr)
	, m_pEnemy(nullptr)
	, m_pBtnManager(nullptr)
	, m_pMessageWindow(nullptr)
	, m_diceLoopSE(SoundInvalidHandle)
{
	DEBUG_LOG("Adv Constructor");
}

//------------------------------------------
//------------------------------------------
Adv::~Adv()
{
	// m_pCharaは外部参照
	delete m_pEnemy;
	delete m_pBtnManager;
}

//------------------------------------------
//------------------------------------------
void Adv::Open()
{
	{
		m_pChara->SetPosition(-350.0f, -150.0f, 0);
		m_pChara->InVisibleDice();
//		m_Chara.prePos = m_Chara.pChara->GetPosition();
	}
	{
		m_pEnemy->SetPosition(350.0f, -150.0f, 0);
		m_pEnemy->SetScale(VEC3(-1.0f, 1.0f, 1.0f));
		m_pEnemy->SetVisible(true);
		m_pEnemy->InVisibleDice();
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
		m_pMessageWindow->SetActive(true);
		m_pMessageWindow->SetTextScale(1.5f);
		char message[128];
		std::snprintf(message, sizeof(message), "敵の%sが現れた！", AppCharaList::Get()->GetCharaInfo(m_pEnemy->GetCharaId()).name.c_str());
		m_pMessageWindow->SetDirectMessage(message);
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
//	m_Chara.pChara->SetPosition(m_Chara.prePos);
	m_pChara->InVisibleDice();
	m_pEnemy->SetVisible(false);
	m_pEnemy->InVisibleDice();
	m_pBtnManager->SetVisible(false);
	m_pBtnManager->Reset();
	m_pBtnManager->Lock();
	m_pMessageWindow->SetTextScale(1.0f);
	m_pMessageWindow->SetVisible(false);
	m_pMessageWindow->SetActive(false);
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
	m_pChara = pChara;
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
		const int nEnemyNo = Random::GetSyncInt(0, AppCharaList::Get()->GetCharaListSize() - 1);
		m_pEnemy = new Character(nEnemyNo);
		m_pEnemy->SetCom(true);
		m_pEnemy->Update(eGameMessage_Setup, nullptr);
		m_pEnemy->SetVisible(false);
	}
	{
		m_pBtnManager = new ButtonManager();
		auto pBtn = m_pBtnManager->CreateButton("image/button_Adv.png");
		pBtn->SetDecideCommand(TransferCommand::eCommand_PushStopDice);
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
		m_pEnemy->Update(message, param);
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
		m_pChara->SetUseDice(AppParam::Get()->GetDiceNum(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId));
		m_pChara->ResetStopDice();
		m_pChara->BeginDice();
		const int nEnemyDice = AppCharaList::Get()->GetCharaInfo(m_pEnemy->GetCharaId()).nDice;
		m_pEnemy->SetUseDice(nEnemyDice);
		m_pEnemy->ResetStopDice();
		m_pEnemy->BeginComDice();
		auto pSoundManager = Engine::GetEngine()->GetSoundManager();
		const SoundResourceLabel label = pSoundManager->LoadSE("sound/se/dice_loop.ogg");
		m_diceLoopSE = pSoundManager->PlaySE(label, 1.0f, 1.0f, true);
		m_nSubStep = 1;
	}
	else if(m_nSubStep == 1){
		if(m_pBtnManager->GetDecide() == 0){
			m_pChara->StopDice();
			m_nSubStep = 2;
		}
	}
	else if(m_nSubStep == 2){
		if(!m_pChara->IsStopDiceState()) {
			if (!m_pChara->IsEndDice()) {
				m_pBtnManager->Reset();
				m_nSubStep = 1;
			} else {
				auto pBtn = m_pBtnManager->GetButton(0);
				pBtn->SetVisible(false);
				pBtn->Disable();
				m_pBtnManager->Reset();
				m_nSubStep = 3;
			}
		}
	}
	else if(m_nSubStep == 3){
		if(m_pEnemy->IsEndDice()){
			auto pSoundManager = Engine::GetEngine()->GetSoundManager();
			pSoundManager->StopSE(m_diceLoopSE);
			if(m_nSubStepCnt >= 60){
				const int nCharaDice = m_pChara->GetDiceSumVal();
				const int nEnemyDice = m_pEnemy->GetDiceSumVal();
				char cText[256];
				if(nCharaDice > nEnemyDice) {
					snprintf(cText, sizeof(cText), "%d対%dできみの勝ち！", nCharaDice, nEnemyDice);
				}
				else if(nCharaDice < nEnemyDice){
					snprintf(cText, sizeof(cText), "%d対%dできみの負け！", nCharaDice, nEnemyDice);
				}
				else {
					snprintf(cText, sizeof(cText), "%d対%dで引き分け！　もう一度勝負！", nCharaDice, nEnemyDice);
				}
				cText[sizeof(cText) - 1] = '\0';
				m_pMessageWindow->SetDirectMessage(cText);
				m_pMessageWindow->SetVisible(true);
				m_nSubStep = 4;
			}
		}
	}
	else if(m_nSubStep == 4){
		if(m_pMessageWindow->IsNextMessage()) {
			const int nCharaDice = m_pChara->GetDiceSumVal();
			const int nEnemyDice = m_pEnemy->GetDiceSumVal();
			if(nCharaDice == nEnemyDice) {
				m_nSubStep = 0;
				m_pMessageWindow->SetVisible(false);
			}
			else if (nCharaDice > nEnemyDice){
				m_nSubStep = 5;
			}
			else {
				m_nNextStep = eSTEP_END;
			}
		}
	}
	else if(m_nSubStep == 5){
		const int nGetPoint = AppCharaList::Get()->GetCharaInfo(m_pEnemy->GetCharaId()).nGetPoint;
		char message[128];
		std::snprintf(message, sizeof(message), "キズナが%d増えた！", nGetPoint);
		m_pMessageWindow->SetDirectMessage(message);
		AppParam::Get()->AddKizunaPoint(AppParam::Get()->GetNetworkInfo().nCurrentPlayerId, nGetPoint);
//		Engine::GetEngine()->ResetTouchEvent();
		m_nSubStep = 6;
	}
	else if(m_nSubStep == 6){
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
