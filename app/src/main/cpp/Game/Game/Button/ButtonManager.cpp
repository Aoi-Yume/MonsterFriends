//
// Created by 葵ユメ on 2018/09/22.
//


#include <CollisionComponent.h>
#include <DelayInput.h>
#include <TransferManager.h>
#include <TransferCommand.h>
#include "SimpleButton.h"
#include "SimpleCursor.h"
#include "DebugButton.h"
#include "ButtonManager.h"

//-----------------------------------------
//-----------------------------------------
ButtonManager::ButtonManager()
: m_bLock(false)
, m_nSelectNo(-1)
, m_nDecideNo(-1)
, m_fDecideDelay(0.0f)
, m_nControlPlayerId(-1)
, m_pCursorEntity(nullptr)
{
	m_aButtons.clear();
	m_pCursorEntity = new SimpleCursor("", this);
	m_pCursorEntity->Update(eGameMessage_Setup, nullptr);
}

//-----------------------------------------
//-----------------------------------------
ButtonManager::~ButtonManager()
{
	const size_t size = m_aButtons.size();
	for(int i = 0; i < size; ++i){
		delete m_aButtons.at(i);
	}
	delete m_pCursorEntity;
	m_aButtons.clear();
}

//-----------------------------------------
//-----------------------------------------
SimpleButton* ButtonManager::CreateButton(const char* pResName)
{
	SimpleButton* pButton = new SimpleButton(pResName);
	pButton->Update(eGameMessage_Setup, nullptr);

	m_aButtons.push_back(pButton);
	return pButton;
}

//-----------------------------------------
//-----------------------------------------
DebugButton* ButtonManager::CreateDebugButton(
		const char* pText, float fSizeW, float fSizeH,
		const VEC4& fontCol, const VEC4& unSelectCol, const VEC4& selectCol)
{
	DebugButton* pButton = new DebugButton();
	pButton->SetText(pText);
	pButton->SetSize(fSizeW, fSizeH);
	pButton->SetFontColor(fontCol);
	pButton->SetBackColor(unSelectCol, selectCol);
	pButton->Update(eGameMessage_Setup, nullptr);

	m_aButtons.push_back(pButton);
	return pButton;
}

//-----------------------------------------
//-----------------------------------------
ButtonBase* ButtonManager::GetButton(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_aButtons.size());
	return m_aButtons[nIdx];
}

//-----------------------------------------
//-----------------------------------------	
void ButtonManager::Lock()
{
	m_bLock = true;
}
	
//-----------------------------------------
//-----------------------------------------
void ButtonManager::Unlock()
{
	m_bLock = false;
}
	
//-----------------------------------------
//-----------------------------------------
bool ButtonManager::IsLock() const
{
	return m_bLock;
}

//-----------------------------------------
//-----------------------------------------
void ButtonManager::SetVisible(bool bVisible)
{
	for(auto& it : m_aButtons){
		it->SetVisible(bVisible);
	}
}

//-----------------------------------------
//-----------------------------------------
void ButtonManager::Reset()
{
	m_nSelectNo = -1;
	m_nDecideNo = -1;
	m_fDecideDelay = 0.0f;
	Update(eGameMessage_Update, nullptr);
}

//-----------------------------------------
//-----------------------------------------
void ButtonManager::SetControlPlayerId(int nPlayerId)
{
	m_nControlPlayerId = nPlayerId;
}

//-----------------------------------------
//-----------------------------------------
int ButtonManager::GetControlPlayerId() const
{
	return m_nControlPlayerId;
}

//-----------------------------------------
//-----------------------------------------
int ButtonManager::GetDecide() const
{
	return (m_fDecideDelay > 0.0f ? -1 : m_nDecideNo);
}

//-----------------------------------------
//-----------------------------------------	
void ButtonManager::Update(GameMessage message, const void* param)
{
	const size_t size = m_aButtons.size();
	if(message != eGameMessage_Setup) {
		m_pCursorEntity->Update(message, param);
		for (int i = 0; i < size; ++i) {
			m_aButtons.at(i)->Update(message, param);
		}
	}
	if(message != eGameMessage_Update){ return; }

	m_fDecideDelay = std::max(m_fDecideDelay - Engine::GetEngine()->GetDeltaTime(), 0.0f);
	if(m_bLock){ return; }
	if(!m_pCursorEntity){ return; }
	if(m_nDecideNo != -1){ return; }

	auto pCursorCollision = (CollisionComponent*)m_pCursorEntity->GetComponent(eComponentKind_Collision);
	if(!pCursorCollision){ return; }

	const float fDecideDelay = 0.15f;
	for(int i = 0; i < size; ++i){
		ButtonBase* pButton = m_aButtons.at(i);
		if(!pButton->IsEnable()){ continue; }

		auto pTransferManager = TransferManager::Get();
		// 通信時操作側処理
		{
			auto pBtnCollision = (CollisionComponent*)pButton->GetComponent(eComponentKind_Collision);
			TouchInputInfo info = {};
			const bool bFind = Engine::GetEngine()->FindDelayTouchInfo(info, eTouchEvent_DOWN, m_nControlPlayerId);
			if (pCursorCollision->IsIntersect(pBtnCollision)) {
				if (bFind) {
					if(!pTransferManager->IsConnectSucess()) {
						m_nDecideNo = i;
						pButton->Select();
						m_fDecideDelay = fDecideDelay;
						return;
					}
					else if(pTransferManager->GetSelfConnect().nPlayerId == m_nControlPlayerId){
						m_nDecideNo = i;
						pButton->Select();
						m_fDecideDelay = fDecideDelay;
						pTransferManager->GetTransfer<TransferCommand>(TransferManager::eTransferKind_Command)->SetSendCommand((TransferCommand::CommandKind)pButton->GetDecideCommand());
						return;
					}
				}
			}
		}
		// 通信時コマンド受付
		{
			if(!pTransferManager->IsConnectSucess()) { continue; }
			auto pCommand = pTransferManager->GetTransfer<TransferCommand>(TransferManager::eTransferKind_Command);;
			if(pCommand->IsCommandEmpty()){ continue; }

			const auto& command = pCommand->GetCommand();
			if(command.nPlayerId == m_nControlPlayerId && command.uCommand == pButton->GetDecideCommand()) {
				m_nDecideNo = i;
				pButton->Select();
				pCommand->PopFrontCommand();
				m_fDecideDelay = fDecideDelay;
				return;
			}
		}
	}
	for(int i = 0; i < size; ++i){
		ButtonBase* pButton = m_aButtons.at(i);
		if(m_nSelectNo != i){
			pButton->UnSelect();
		}
	}
}