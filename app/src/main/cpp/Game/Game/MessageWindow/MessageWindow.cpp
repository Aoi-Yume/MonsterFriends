//
// Created by 葵ユメ on 2018/09/22.
//


#include <LayoutComponent.h>
#include <sstream>
#include <DelayInput.h>
#include <TransferManager.h>
#include <TransferCommand.h>
#include "MessageWindow.h"

MessageWindow::MessageWindow(const char* pResName)
: Super()
, m_bActive(false)
, m_bSetMessage(false)
, m_bNextMessage(false)
, m_nCurrentUseLine(0)
, m_nControlPlayerId(-1)
, m_cResPath()
, m_fTextScale(1.0f)
, m_fLastInputTime(0.0f)
, m_uDecideCommand(TransferCommand::eCommand_GameMainNextMessage)
, m_pLayoutComponent(nullptr)
, m_pTextComponent()
, m_decideSELabel(SoundInvalidLabel)
{
	snprintf(m_cResPath, sizeof(m_cResPath), "%s", pResName);
	m_cResPath[sizeof(m_cResPath) - 1] = '\0';
}

MessageWindow::~MessageWindow()
{
}


void MessageWindow::GameEntitySetup(const void* param)
{
	Super::GameEntitySetup(param);
	{
		DEBUG_LOG_A("ResName:%s\n", m_cResPath);
		Entity::CreateLayoutComponent(this, m_cResPath);

		m_pLayoutComponent = (LayoutComponent *) GetComponent(eComponentKind_Layout);
		m_pLayoutComponent->SetOrtho(true);
	}
	for(int i = 0; i < eLINE_MAX; ++i) {
		auto pChild = new Entity();
		Entity::CreateTextImageComponent(pChild, "", 32);
		AddChild(pChild);

		m_pTextComponent[i] = (TextImageComponent*) pChild->GetComponent(eComponentKind_Layout);
		m_pTextComponent[i]->SetOrtho(true);
		pChild->SetPosition(VEC3(0, i * -40, 0));
	}
	SetPosition(0, 1080.0f * -0.4f, 0.0f);
}

void MessageWindow::GameEntityUpdate(const void* param)
{
	if(!m_bActive){ return; }
	Super::GameEntityUpdate(param);
	if(!m_bSetMessage){ return; }

	auto pTransferManager = TransferManager::Get();

	if(!m_bNextMessage){
		{
			DelayTouchInfo info = {};
			const bool bFind = Engine::GetEngine()->FindDelayTouchInfo(info, eTouchEvent_DOWN, m_nControlPlayerId);
			if (bFind) {
				m_fLastInputTime = info.fTime;
				if (!pTransferManager->IsConnectSucess()) {
					m_bNextMessage = true;
				} else if (pTransferManager->GetSelfConnect().nPlayerId == m_nControlPlayerId) {
					m_bNextMessage = true;
					pTransferManager->GetTransfer<TransferCommand>(TransferManager::eTransferKind_Command)->SetSendCommand((TransferCommand::CommandKind)m_uDecideCommand);
				}
			}
		}
		if(pTransferManager->IsConnectSucess()) {
			auto pCommand = pTransferManager->GetTransfer<TransferCommand>(TransferManager::eTransferKind_Command);
			if(pCommand->IsCommandEmpty()){ return; }

			const auto& command = pCommand->GetCommand();
			if(command.nPlayerId == m_nControlPlayerId && command.uCommand == m_uDecideCommand) {
				m_bNextMessage = true;
				pCommand->PopFrontCommand();
			}
		}
		if(m_bNextMessage && m_decideSELabel != SoundInvalidLabel){
			Engine::GetEngine()->GetSoundManager()->PlaySE(m_decideSELabel, 1.0f, 1.0f, false);
		}
	}
	else{
		DelayTouchInfo info = {};
		const bool bFind = Engine::GetEngine()->FindDelayTouchInfo(info, eTouchEvent_UP, m_nControlPlayerId);
		if(bFind){
			clearNextMessageFlag();
		}
	}
}

void MessageWindow::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}

void MessageWindow::SetVisible(bool bVisible)
{
	m_pLayoutComponent->SetVisible(bVisible);
	for(int i = 0; i < eLINE_MAX; ++i) {
		m_pTextComponent[i]->SetVisible(bVisible);
	}
}

void MessageWindow::SetControlPlayerId(int nPlayerId)
{
	m_nControlPlayerId = nPlayerId;
}

void MessageWindow::SetTextScale(float fScale)
{
	m_fTextScale = fScale;
}

void MessageWindow::SetMessage(const char *pLabel)
{

	//Engine::GetEngine()->L
}

void MessageWindow::SetDirectMessage(const char *pText)
{
	clearText();
	m_bSetMessage = true;
	m_nCurrentUseLine = eLINE_MAX;

	std::stringstream ss;
	ss.clear(std::ios_base::goodbit);
	ss.str("");
	ss << pText;
	for(int i = 0; i < eLINE_MAX; ++i) {
		if(ss.eof()){
			m_nCurrentUseLine = i;
			break;
		}
		std::string s;
		ss >> s;
		m_pTextComponent[i]->SetText(s.c_str(), m_fTextScale);
	}
	updatePosition();
}

bool MessageWindow::IsNextMessage() const
{
	return m_bNextMessage;
}

void MessageWindow::SetDecideSELabel(const char *pLabel)
{
	if(pLabel) {
		m_decideSELabel = Engine::GetEngine()->GetSoundManager()->LoadSE(pLabel);
	}
	else{
		m_decideSELabel = SoundInvalidLabel;
	}
}

void MessageWindow::clearNextMessageFlag()
{
	m_bNextMessage = false;
	m_bSetMessage = false;
}

void MessageWindow::clearText()
{
	clearNextMessageFlag();

	for(auto& it : m_pTextComponent){
		it->SetText("");
	}
	m_nCurrentUseLine = 0;
}

void MessageWindow::updatePosition()
{
	float fCurrentY = (float)(m_nCurrentUseLine / 2) * 40.0f * m_fTextScale;
	for(int i = 0; i < m_nCurrentUseLine; ++i){
		GetChild(i)->SetPosition(VEC3(0, fCurrentY, 0));
		fCurrentY -= 40.0f * m_fTextScale;
	}
}