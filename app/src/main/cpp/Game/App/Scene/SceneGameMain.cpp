//
// Created by 葵ユメ on 2018/09/19.
//


#include <LayoutComponent.h>
#include "SceneGameMain.h"
#include "SceneAdv.h"
#include "SceneSpendTime.h"
#include "SceneWork.h"
#include <EntityManager.h>
#include <Button/ButtonManager.h>
#include <Button/SimpleButton.h>
#include <SceneManager.h>
#include <InformationPlate.h>
#include <MessageWindow/MessageWindow.h>

SceneBase* SceneGameMain::CreateScene()
{
	return new SceneGameMain();
}

//------------------------------------------
//------------------------------------------
SceneGameMain::SceneGameMain()
: SceneBase("GameMain")
, m_pBgImage(nullptr)
, m_pChara(nullptr)
, m_pBtnManager(nullptr)
, m_pInformationPlate(nullptr)
, m_pMessageWindow(nullptr)
{
	DEBUG_LOG("Scene Game Main Constructor");
}

//------------------------------------------
//------------------------------------------
SceneGameMain::~SceneGameMain()
{
	delete m_pBgImage;
	delete m_pChara;
	delete m_pBtnManager;
	delete m_pInformationPlate;
	delete m_pMessageWindow;
}

//------------------------------------------
//------------------------------------------
void SceneGameMain::SceneSetup() {
	Super::SceneSetup();
	DEBUG_LOG("GameMain Call Setup");
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
		m_pInformationPlate = new InformationPlate();
		m_pInformationPlate->Update(eGameMessage_Setup, nullptr);
	}
	{
		m_pBtnManager = new ButtonManager();
		const std::pair<const char *, VEC3> btnList[] = {
				{"image/button_Adv.png",       VEC3(-250.0f, -400.0f, 0)},
				{"image/button_SpendTime.png", VEC3(0.0f, -400.0f, 0)},
				{"image/button_Work.png",      VEC3(250.0f, -400.0f, 0)},
		};
		for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
			auto pBtn = m_pBtnManager->CreateButton(btnList[i].first);
			pBtn->SetPosition(btnList[i].second);
		}
	}
	{
		m_pMessageWindow = new MessageWindow("image/message_window.png");
		m_pMessageWindow->Update(eGameMessage_Setup, nullptr);
		m_pMessageWindow->SetTextScale(1.5f);
		m_pMessageWindow->SetDirectMessage("てすと\nててててすと");
		m_pMessageWindow->SetVisible(false);
	}
	DEBUG_LOG("Setup End");
}

//------------------------------------------
//------------------------------------------
void SceneGameMain::SceneUpdate()
{
	Super::SceneUpdate();
	//DEBUG_LOG("Launcher Call Update");

	switch (m_pBtnManager->GetDecide()){
		case eBtn_Adv: { SCENE_MANAGER()->ChangeScene(SceneAdv::CreateScene()); break; }
		case eBtn_SpendTime: { SCENE_MANAGER()->ChangeScene(SceneSpendTime::CreateScene()); break; }
		case eBtn_Work: { SCENE_MANAGER()->ChangeScene(SceneWork::CreateScene()); break; }
		default:{ break; }
	}
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
		m_pBtnManager->Update(message, param);
		m_pMessageWindow->Update(message, param);
	}
}
