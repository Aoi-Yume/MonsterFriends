//
// Created by 葵ユメ on 2021/01/10.
//


#include <CameraComponent.h>
#include <AppParam.h>
#include <AppItemList.h>
#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "CollisionComponent.h"
#include "Shop.h"
#include <Button/ButtonManager.h>
#include <Button/SimpleButton.h>
#include <MessageWindow/MessageWindow.h>
#include <InformationPlate.h>
#include <ItemListUI.h>

Shop::Shop()
	: GameEntity()
	, m_nStep(eStep_End)
	, m_nItemNo()
	, m_pMessageWindow(nullptr)
	, m_pInformationPlate(nullptr)
	, m_pItemListUI(nullptr)
	, m_aButtonManager()
{
	DEBUG_LOG("Create Shop");
	m_nItemNo[0] = 0;
	m_nItemNo[1] = 3;
	m_nItemNo[2] = 2;
}

Shop::~Shop()
{
	for(auto& it : m_aButtonManager) {
		delete it;
	}
	m_aButtonManager.clear();
	delete m_pMessageWindow;
}

void Shop::Open()
{
	SetVisible(true);
	m_aButtonManager[eBtnManager_Back]->Unlock();
	m_aButtonManager[eBtnManager_Back]->SetVisible(true);
	m_pItemListUI->ClearItemNo();
	m_pItemListUI->AddItemNo(m_nItemNo[0]);
	m_pItemListUI->AddItemNo(m_nItemNo[1]);
	m_pItemListUI->AddItemNo(m_nItemNo[2]);
	m_pItemListUI->Open();
	m_nStep = eStep_SelectItemWait;
}

void Shop::Close()
{
	SetVisible(false);
	for(auto& it : m_aButtonManager) {
		it->Lock();
		it->SetVisible(false);
	}
	m_pItemListUI->Close();
	m_pMessageWindow->SetVisible(false);
}

bool Shop::IsEnd() const
{
	return m_nStep == eStep_End;
}

void Shop::SetInformationPlate(InformationPlate *pPlate)
{
	m_pInformationPlate = pPlate;
}

void Shop::SetVisible(bool bVisible)
{
	const int nChildSize = GetChildSize();
	for(int i = 0; i < nChildSize; ++i){
		auto pComponent = GetChild(i)->GetComponent(eComponentKind_Layout);
		if(pComponent){
			reinterpret_cast<LayoutComponent*>(pComponent)->SetVisible(bVisible);
		}
	}
}

void Shop::GameEntitySetup(const void* param) {
	Super::GameEntitySetup(param);

	const int nCurrentPlayerId = AppParam::Get()->GetNetworkInfo().nCurrentPlayerId;
	{
		m_pItemListUI = new ItemListUI();
		AddChild(m_pItemListUI);
	}
	{
		Entity* pEntity = new Entity();
		Entity::CreateLayoutComponent(pEntity, "image/clerk.png");
		auto pComponent = (LayoutComponent *) pEntity->GetComponent(eComponentKind_Layout);
		pComponent->SetOrtho(true);
		const float x = Engine::GetEngine()->GetScreenInfo().m_nScreenX * 0.5f - 100.0f;
		const float y = Engine::GetEngine()->GetScreenInfo().m_nScreenY * 0.5f - 80.0f;
		pEntity->SetPosition(x, -y, 0);
		AddChild(pEntity);
	}
	{
		// 「かう」or「かわない」ボタン
		{
			auto pBtnManager = new ButtonManager();
			const std::pair<const char *, VEC3> btnList[] = {
					{"image/button_buy.png", VEC3(-250.0f, -400.0f, 0)},
					{"image/button_buyCancel.png", VEC3(250.0f, -400.0f, 0)},
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				auto pBtn = pBtnManager->CreateButton(btnList[i].first);
				pBtn->SetPosition(btnList[i].second);
			}
			pBtnManager->SetVisible(false);
			pBtnManager->Lock();
			pBtnManager->SetControlPlayerId(nCurrentPlayerId);
			m_aButtonManager.emplace_back(pBtnManager);
		}
		// 戻るボタン
		{
			auto pBtnManager = new ButtonManager();
			const std::pair<const char *, VEC3> btnList[] = {
					{"image/button_back.png", VEC3(700.0f, 300.0f, 0)},
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				auto pBtn = pBtnManager->CreateButton(btnList[i].first);
				pBtn->SetPosition(btnList[i].second);
			}
			pBtnManager->SetVisible(false);
			pBtnManager->Lock();
			pBtnManager->SetControlPlayerId(nCurrentPlayerId);
			m_aButtonManager.emplace_back(pBtnManager);
		}
	}
	{
		m_pMessageWindow = new MessageWindow("image/message_window.png");
		m_pMessageWindow->Update(eGameMessage_Setup, nullptr);
		m_pMessageWindow->SetTextScale(1.5f);
		m_pMessageWindow->SetControlPlayerId(nCurrentPlayerId);
		m_pMessageWindow->SetDirectMessage("");
		m_pMessageWindow->SetVisible(false);
	}
	SetVisible(false);
}

void Shop::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);
	const int nSelectItemNo = m_pItemListUI->GetSelectItemNo();
	if(m_nStep == eStep_SelectItemWait){
		if(nSelectItemNo >= 0){
			m_aButtonManager[eBtnManager_Back]->SetVisible(false);
			m_aButtonManager[eBtnManager_Back]->Lock();
			m_nStep = eStep_ShowDetail;
		}
	}
	else if(m_nStep == eStep_ShowDetail){
		const auto& itemInfo = AppItemList::Get()->GetItemInfo(nSelectItemNo);
		char str[128] = {};
		snprintf(str, sizeof(str), "【%s】", itemInfo.name.c_str());
		str[sizeof(str) - 1] = '\0';
		m_pMessageWindow->SetDirectMessage(str);
		m_pMessageWindow->SetVisible(true);
		m_pItemListUI->Lock();
		m_nStep = eStep_MessageWait;
	}
	else if(m_nStep == eStep_MessageWait){
		if(m_pMessageWindow->IsNextMessage()){
			m_nStep = eStep_ShowExplain;
		}
	}
	else if(m_nStep == eStep_ShowExplain){
		const auto& itemInfo = AppItemList::Get()->GetItemInfo(nSelectItemNo);
		char str[128] = {};
		snprintf(str, sizeof(str), "%sにゃ\n「%dキズナ」にゃ", itemInfo.explain.c_str(), itemInfo.nCost);
		str[sizeof(str) - 1] = '\0';
		m_pMessageWindow->SetDirectMessage(str);
		m_nStep = eStep_MessageWait2;
	}
	else if(m_nStep == eStep_MessageWait2){
		if(m_pMessageWindow->IsNextMessage()){
			m_pMessageWindow->SetVisible(false);
			m_aButtonManager[eBtnManager_BuyOrCancel]->SetVisible(true);
			m_aButtonManager[eBtnManager_BuyOrCancel]->Unlock();
			m_nStep = eStep_BuyOrCancel;
		}
	}
	else if(m_nStep == eStep_BuyOrCancel){
		const int nDecide2 = m_aButtonManager[eBtnManager_BuyOrCancel]->GetDecide();
		if(nDecide2 == eBtn_Buy){
			m_aButtonManager[eBtnManager_BuyOrCancel]->SetVisible(false);
			m_nStep = eStep_BuyCheck;
		}
		else if (nDecide2 == eBtn_BuyCancel){
			m_nStep = eStep_Reset;
		}
	}
	else if(m_nStep == eStep_BuyCheck){
		const int nPlayerId = AppParam::Get()->GetNetworkInfo().nCurrentPlayerId;
		const int nPlayerKizuna = AppParam::Get()->GetKizunaPoint(nPlayerId);
		const auto& itemInfo = AppItemList::Get()->GetItemInfo(nSelectItemNo);
		if(nPlayerKizuna >= itemInfo.nCost){
			AppParam::Get()->SubKizunaPoint(nPlayerId, itemInfo.nCost);
			AppParam::Get()->AddItem(nPlayerId, nSelectItemNo, 1);
			m_pMessageWindow->SetDirectMessage("まいどありにゃ");
		}
		else{
			m_pMessageWindow->SetDirectMessage("キズナが足りないにゃ");
		}
		m_pMessageWindow->SetVisible(true);
		m_nStep = eStep_BuyAfterMessageWait;
	}
	else if(m_nStep == eStep_BuyAfterMessageWait){
		if(m_pMessageWindow->IsNextMessage()){
			m_nStep = eStep_ClearCheck;
		}
	}
	else if(m_nStep == eStep_ClearCheck){
		const int nPlayerId = AppParam::Get()->GetNetworkInfo().nCurrentPlayerId;
		const auto& itemInfo = AppItemList::Get()->GetItemInfo(nSelectItemNo);
		m_nStep = eStep_Reset;
		if(itemInfo.name == "キズナクリスタル"){
			if(AppParam::Get()->GetItemNum(nPlayerId, nSelectItemNo) >= 3){
				AppParam::Get()->SetClear(true);
				m_nStep = eStep_ClearMessage;
			}
		}
	}
	else if(m_nStep == eStep_ClearMessage){
		m_pMessageWindow->SetDirectMessage(
				"クリスタルが３つあるにゃ！すごいにゃ！\n"
						"みんなで今回の結果をみるにゃ！");
		m_pMessageWindow->SetVisible(true);
		m_nStep = eStep_ClearMessageWait;
	}
	else if(m_nStep == eStep_ClearMessageWait){
		if(m_pMessageWindow->IsNextMessage()){
			m_nStep = eStep_Reset;
		}
	}
	else if(m_nStep == eStep_Reset){
		if(m_pInformationPlate){
			m_pInformationPlate->UpdatePlate();
		}
		m_aButtonManager[eBtnManager_BuyOrCancel]->SetVisible(false);
		m_aButtonManager[eBtnManager_BuyOrCancel]->Reset();
		m_aButtonManager[eBtnManager_BuyOrCancel]->Lock();
		m_pItemListUI->Reset();
		m_pItemListUI->UnLock();
		m_aButtonManager[eBtnManager_Back]->Reset();
		m_aButtonManager[eBtnManager_Back]->Unlock();
		m_aButtonManager[eBtnManager_Back]->SetVisible(true);
		m_pMessageWindow->SetVisible(false);
		m_nStep = (AppParam::Get()->IsClear() ? eStep_End : eStep_SelectItemWait);
	}
	else if(m_nStep == eStep_BackMessage){
		m_pMessageWindow->SetDirectMessage("バイバイにゃ");
		m_pMessageWindow->SetVisible(true);
		m_nStep = eStep_BackMessage_Wait;
	}
	else if(m_nStep == eStep_BackMessage_Wait){
		if(m_pMessageWindow->IsNextMessage()){
			m_nStep = eStep_End;
		}
	}

	if(m_aButtonManager[eBtnManager_Back]->GetDecide() == eBtn_Back){
		for(auto& it : m_aButtonManager){
			it->Reset();
			it->Lock();
		}
		m_nStep = eStep_BackMessage;
	}
}

void Shop::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
	for(auto& it : m_aButtonManager) {
		it->Update(message, param);
	}
	m_pMessageWindow->Update(message, param);
}