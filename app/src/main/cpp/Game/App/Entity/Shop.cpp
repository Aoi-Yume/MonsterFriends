﻿//
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

Shop::Shop()
	: GameEntity()
	, m_nStep(0)
	, m_nItemNo()
	, m_pMessageWindow(nullptr)
	, m_pInformationPlate(nullptr)
	, m_aButtonManager()
{
	DEBUG_LOG("Create Shop");
	m_nItemNo[0] = 0;
	m_nItemNo[1] = 1;
	m_nItemNo[2] = 2;
}

Shop::~Shop()
{
	for(auto& it : m_aButtonManager) {
		delete it;
	}
	m_aButtonManager.clear();
}

void Shop::Open()
{
	SetVisible(true);
	m_aButtonManager[eBtnManager_Item]->Unlock();
	m_aButtonManager[eBtnManager_Item]->SetVisible(true);
	m_nStep = eStep_SelectItemWait;
}

void Shop::Close()
{
	SetVisible(false);
	for(auto& it : m_aButtonManager) {
		it->Lock();
		it->SetVisible(false);
	}
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
		Entity* pEntity = new Entity();
		Entity::CreateLayoutComponent(pEntity, "image/gray_screen.png");
		auto pComponent = (LayoutComponent *) pEntity->GetComponent(eComponentKind_Layout);
		pComponent->SetOrtho(true);
		AddChild(pEntity);
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
		// アイテムボタン
		{
			auto pBtnManager = new ButtonManager();
			const VEC3 btnList[] = {
					VEC3(-450.0f, 0, 0),
					VEC3(0.0f, 0, 0),
					VEC3(450.0f, 0, 0),
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				const auto& itemInfo = AppItemList::Get()->GetItemInfo(m_nItemNo[i]);
				auto pBtn = pBtnManager->CreateButton(itemInfo.fileName.c_str());
				pBtn->SetPosition(btnList[i]);
			}
			pBtnManager->SetVisible(false);
			pBtnManager->Lock();
			pBtnManager->SetControlPlayerId(nCurrentPlayerId);
			m_aButtonManager.emplace_back(pBtnManager);
		}
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
	}
	{
		m_pMessageWindow = new MessageWindow("image/message_window.png");
		m_pMessageWindow->Update(eGameMessage_Setup, nullptr);
		m_pMessageWindow->SetTextScale(1.5f);
		m_pMessageWindow->SetControlPlayerId(nCurrentPlayerId);
		m_pMessageWindow->SetDirectMessage("きれいな「ごみ」にゃ");
		m_pMessageWindow->SetVisible(false);
	}
	SetVisible(false);
}

void Shop::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);
	const int nDecide = m_aButtonManager[eBtnManager_Item]->GetDecide();
	if(m_nStep == eStep_SelectItemWait){
		if(nDecide >= eBtn_Item_01 && nDecide <= eBtn_Item_03){
			m_nStep = eStep_ShowDetail;
		}
	}
	else if(m_nStep == eStep_ShowDetail){
		const auto& itemInfo = AppItemList::Get()->GetItemInfo(m_nItemNo[nDecide]);
		char str[128] = {};
		snprintf(str, sizeof(str), "%s\n「%dキズナ」にゃ", itemInfo.explain.c_str(), itemInfo.nCost);
		str[sizeof(str) - 1] = '\0';
		m_pMessageWindow->SetDirectMessage(str);
		m_pMessageWindow->SetVisible(true);
		m_aButtonManager[eBtnManager_Item]->Lock();
		m_nStep = eStep_MessageWait;
	}
	else if(m_nStep == eStep_MessageWait){
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
		const auto& itemInfo = AppItemList::Get()->GetItemInfo(m_nItemNo[nDecide]);
		if(nPlayerKizuna >= itemInfo.nCost){
			AppParam::Get()->SubKizunaPoint(nPlayerId, itemInfo.nCost);
			AppParam::Get()->AddItem(nPlayerId, m_nItemNo[nDecide], 1);
			m_pMessageWindow->SetDirectMessage("まいどありにゃ");
		}
		else{
			m_pMessageWindow->SetDirectMessage("キズナが足りないにゃ");
		}
		m_pMessageWindow->SetVisible(true);
		m_nStep = eSteo_BuyAfterMessageWait;
	}
	else if(m_nStep == eSteo_BuyAfterMessageWait){
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
		m_aButtonManager[eBtnManager_Item]->Reset();
		m_aButtonManager[eBtnManager_Item]->Unlock();
		m_pMessageWindow->SetVisible(false);
		m_nStep = eStep_SelectItemWait;
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