//
// Created by 葵ユメ on 2021/02/21.
//


#include <CameraComponent.h>
#include <AppParam.h>
#include <AppItemList.h>
#include <AppSkillList.h>
#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "CollisionComponent.h"
#include "UseItem.h"
#include <Button/ButtonManager.h>
#include <Button/SimpleButton.h>
#include <MessageWindow/MessageWindow.h>
#include <InformationPlate.h>
#include <ItemListUI.h>

#include <TransferManager.h>
#include <TransferSkillInfo.h>

UseItem::UseItem()
	: GameEntity()
	, m_nStep(0)
	, m_pMessageWindow(nullptr)
	, m_pInformationPlate(nullptr)
	, m_pItemListUI(nullptr)
	, m_aButtonManager()
{
	DEBUG_LOG("Create UseItem");
}

UseItem::~UseItem()
{
	for(auto& it : m_aButtonManager) {
		delete it;
	}
	m_aButtonManager.clear();
	delete m_pMessageWindow;
}

void UseItem::Open()
{
	SetVisible(true);
	updateItemList();
	m_pItemListUI->Open();
	m_aButtonManager[eBtnManager_Back]->Unlock();
	m_aButtonManager[eBtnManager_Back]->SetVisible(true);
	m_aButtonManager[eBtnManager_Back]->Reset();
	m_nStep = eStep_SelectItemWait;
}

void UseItem::Close()
{
	SetVisible(false);
	for(auto& it : m_aButtonManager) {
		it->Lock();
		it->SetVisible(false);
	}
	m_pItemListUI->Close();
	m_pMessageWindow->SetVisible(false);
}

bool UseItem::IsEnd() const
{
	return m_nStep == eStep_End;
}

void UseItem::GameEntitySetup(const void* param) {
	Super::GameEntitySetup(param);

	const int nCurrentPlayerId = AppParam::Get()->GetNetworkInfo().nCurrentPlayerId;
	{
		m_pItemListUI = new ItemListUI();
		AddChild(m_pItemListUI);
	}
	{
		// 「つかう」or「やめる」ボタン
		{
			auto pBtnManager = new ButtonManager();
			const std::pair<const char *, VEC3> btnList[] = {
					{"image/button_use.png", VEC3(-250.0f, -400.0f, 0)},
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

void UseItem::GameEntityUpdate(const void* param)
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
		snprintf(str, sizeof(str), "%s", itemInfo.explain.c_str());
		str[sizeof(str) - 1] = '\0';
		m_pMessageWindow->SetDirectMessage(str);
		m_nStep = eStep_MessageWait2;
	}
	else if(m_nStep == eStep_MessageWait2){
		if(m_pMessageWindow->IsNextMessage()){
			m_pMessageWindow->SetVisible(false);
			m_aButtonManager[eBtnManager_UseOrCancel]->SetVisible(true);
			m_aButtonManager[eBtnManager_UseOrCancel]->Unlock();
			m_nStep = eStep_UseOrCancel;
		}
	}
	else if(m_nStep == eStep_UseOrCancel){
		const int nDecide2 = m_aButtonManager[eBtnManager_UseOrCancel]->GetDecide();
		if(nDecide2 == eBtn_Use){
			m_aButtonManager[eBtnManager_UseOrCancel]->SetVisible(false);
			m_nStep = eStep_Use;
		}
		else if (nDecide2 == eBtn_UseCancel){
			m_nStep = eStep_Reset;
		}
	}
	else if(m_nStep == eStep_Use){
		const int nPlayer = AppParam::Get()->GetNetworkInfo().nCurrentPlayerId;
		AppParam::Get()->SubItem(nPlayer, nSelectItemNo, 1);
		const char* pSkillName = AppItemList::Get()->GetItemInfo(nSelectItemNo).skillName.c_str();
		const int nSkillNo = SKILL_LIST()->GetSkillNoFromSkillName(pSkillName);
		SKILL_LIST()->BeginItemSkill(nPlayer, nSkillNo);
		m_nStep = eStep_UseWait;
	}
	else if(m_nStep == eStep_UseWait){
		if(SKILL_LIST()->IsEndItemSkill()){
			TransferManager::Get()->GetTransfer<TransferSkillInfo>(TransferManager::eTramsferKind_SkillInfo)->Dump();
			m_nStep = eStep_Reset;
		}
	}
	else if(m_nStep == eStep_Reset){
		if(m_pInformationPlate){
			m_pInformationPlate->UpdatePlate();
		}
		m_aButtonManager[eBtnManager_UseOrCancel]->SetVisible(false);
		m_aButtonManager[eBtnManager_UseOrCancel]->Reset();
		m_aButtonManager[eBtnManager_UseOrCancel]->Lock();
		// UIを更新するため一度閉じて開きなおす
		{
			m_pItemListUI->Close();
			updateItemList();
			m_pItemListUI->Open();
			m_pItemListUI->Reset();
			m_pItemListUI->UnLock();
		}
		m_aButtonManager[eBtnManager_Back]->Reset();
		m_aButtonManager[eBtnManager_Back]->Unlock();
		m_aButtonManager[eBtnManager_Back]->SetVisible(true);
		m_pMessageWindow->SetVisible(false);
		m_nStep = eStep_SelectItemWait;
	}

	if(m_aButtonManager[eBtnManager_Back]->GetDecide() == eBtn_Back){
		for(auto& it : m_aButtonManager){
			it->Reset();
			it->Lock();
		}
		m_nStep = eStep_End;
	}
}

void UseItem::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
	for(auto& it : m_aButtonManager) {
		it->Update(message, param);
	}
	m_pMessageWindow->Update(message, param);
}

void UseItem::SetInformationPlate(InformationPlate *pPlate)
{
	m_pInformationPlate = pPlate;
}

void UseItem::SetVisible(bool bVisible)
{
	const int nChildSize = GetChildSize();
	for(int i = 0; i < nChildSize; ++i){
		auto pComponent = GetChild(i)->GetComponent(eComponentKind_Layout);
		if(pComponent){
			reinterpret_cast<LayoutComponent*>(pComponent)->SetVisible(bVisible);
		}
	}
}

void UseItem::updateItemList()
{
	m_pItemListUI->ClearItemNo();
	const int nPlayer = AppParam::Get()->GetNetworkInfo().nCurrentPlayerId;
	for(int i = 0; i < AppParam::eItemKind_Max; ++i){
		if(AppParam::Get()->GetItemNum(nPlayer, i) > 0){
			m_pItemListUI->AddItemNo(i);
		}
	}
}
