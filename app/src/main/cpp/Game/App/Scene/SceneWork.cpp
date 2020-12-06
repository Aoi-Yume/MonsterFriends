//
// Created by 葵ユメ on 2018/10/08.
//


#include <LayoutComponent.h>
#include <TransformComponent.h>
#include "SceneWork.h"
#include "Button/SimpleButton.h"
#include "SceneGameMain.h"
#include <Character.h>
#include <InformationPlate.h>
#include <MessageWindow/MessageWindow.h>
#include <SceneManager.h>
#include <AppParam.h>

SceneBase* SceneWork::CreateScene()
{
	return new SceneWork();
}

//------------------------------------------
//------------------------------------------
SceneWork::SceneWork()
: SceneBase("work")
, m_nStep(0)
, m_nNextStep(0)
, m_nStepCnt(0)
, m_pBgImage(nullptr)
, m_pChara(nullptr)
, m_pInformationPlate(nullptr)
, m_pMessageWindow(nullptr)
{
	DEBUG_LOG("Scene Work Constructor");
}

//------------------------------------------
//------------------------------------------
SceneWork::~SceneWork()
{
	delete m_pBgImage;
	delete m_pChara;
	delete m_pInformationPlate;
	delete m_pMessageWindow;
}

//------------------------------------------
//------------------------------------------
void SceneWork::SceneSetup() {
	Super::SceneSetup();
	DEBUG_LOG("SceneWork Call Setup");
	{
		m_pBgImage = new Entity();
		Entity::CreateLayoutComponent(m_pBgImage, "image/monfri_bg.png");
		auto pLayoutComponent = (LayoutComponent *) m_pBgImage->GetComponent(eComponentKind_Layout);
		pLayoutComponent->SetOrtho(true);
		m_pBgImage->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pChara = new Character(Character::eCHARA_01);
		m_pChara->Update(eGameMessage_Setup, nullptr);
		// ひとまず画面外に表示
		m_pChara->SetPosition(-5000.0f, -150.0f, 0);
		m_pChara->SetState(Character::eState_Work);
		{
			auto pWorkWood = new Entity();
			Entity::CreateLayoutComponent(pWorkWood, "image/work_wood.png");
			reinterpret_cast<LayoutComponent*>(pWorkWood->GetComponent(eComponentKind_Layout))->SetOrtho(true);
			pWorkWood->Update(eGameMessage_Setup, nullptr);
			pWorkWood->SetPosition(0, 200.0f, 0);
			m_pChara->AddChild(pWorkWood);
		}
	}
	{
		m_pInformationPlate = new InformationPlate();
		m_pInformationPlate->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pMessageWindow = new MessageWindow("image/message_window.png");
		m_pMessageWindow->Update(eGameMessage_Setup, nullptr);
		m_pMessageWindow->SetTextScale(1.5f);
		std::string str = AppParam::Get()->GetCharaName();
		str.append("はよく働いている！");
		m_pMessageWindow->SetDirectMessage(str.c_str());
		m_pMessageWindow->SetVisible(false);
	}
	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void SceneWork::SceneUpdate() {
	Super::SceneUpdate();
	//DEBUG_LOG("Launcher Call Update");
	if(m_nStep == 0){
		if(m_nStepCnt >= 180){
			m_pMessageWindow->SetVisible(true);
			Engine::GetEngine()->ResetTouchEvent();
			m_nNextStep = 1;
		}
	}
	else if(m_nStep == 1){
		if(m_pMessageWindow->IsNextMessage()) {
			Engine::GetEngine()->ResetTouchEvent();
			// TODO キズナ値調整
			m_pMessageWindow->SetDirectMessage("キズナが10増えた！");
			AppParam::Get()->AddKizunaPoint(10);
			m_nNextStep = 2;
		}
	}
	else if(m_nStep == 2){
		if(m_pMessageWindow->IsNextMessage()){
			SCENE_MANAGER()->ChangeScene(SceneGameMain::CreateScene());
		}
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
void SceneWork::SceneFinalize()
{
	Super::SceneFinalize();
	DEBUG_LOG("SceneWork Call Finalize");
}

//------------------------------------------
//------------------------------------------
void SceneWork::EntityUpdate(GameMessage message, const void* param)
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
		m_pInformationPlate->Update(message, param);
		m_pMessageWindow->Update(message, param);
	}
}
