//
// Created by 葵ユメ on 2018/09/22.
//


#include <LayoutComponent.h>
#include <sstream>
#include "MessageWindow.h"

MessageWindow::MessageWindow(const char* pResName)
: Super()
, m_bNextMessage(false)
, m_nNextMessageCnt(0)
, m_nCurrentUseLine(0)
, m_cResPath()
, m_fTextScale(1.0f)
, m_pLayoutComponent(nullptr)
, m_pTextComponent()
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
	const float fScrrenY = Engine::GetEngine()->GetScreenInfo().m_nScreenY;
	SetPosition(0, fScrrenY * -0.4f, 0.0f);
}

void MessageWindow::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);

	if(!m_bNextMessage){
		if(Engine::GetEngine()->GetTouchInputInfo().m_nTouchEvent == 0){
			m_bNextMessage = true;
			m_nNextMessageCnt = 0;
		}
	}
	else{
		m_nNextMessageCnt++;
		if(m_nNextMessageCnt >= 10){
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

void MessageWindow::clearNextMessageFlag()
{
	m_bNextMessage = false;
	m_nNextMessageCnt = 0;
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
		auto pChild = reinterpret_cast<Entity*>(GetChild(i));
		pChild->SetPosition(VEC3(0, fCurrentY, 0));
		fCurrentY -= 40.0f * m_fTextScale;
	}
}