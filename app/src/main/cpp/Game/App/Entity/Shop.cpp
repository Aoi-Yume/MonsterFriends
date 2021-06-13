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
#include <Random.h>
#include <TransferCommand.h>

Shop::Shop()
	: GameEntity()
	, m_nStep(eStep_End)
	, m_pMessageWindow(nullptr)
	, m_pInformationPlate(nullptr)
	, m_pItemListUI(nullptr)
	, m_aButtonManager()
{
	DEBUG_LOG("Create Shop");
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
	m_pItemListUI->Open();
	m_pMessageWindow->SetActive(true);
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
	m_pMessageWindow->SetActive(false);
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
		auto pComponent = GetChild(i)->GetComponent<LayoutComponent*>(eComponentKind_Layout);
		if(pComponent){
			pComponent->SetVisible(bVisible);
		}
	}
}

void Shop::GameEntitySetup(const void* param) {
	Super::GameEntitySetup(param);

	const int nCurrentPlayerId = AppParam::Get()->GetNetworkInfo().nCurrentPlayerId;
	{
		m_pItemListUI = new ItemListUI();
		setNewItem(0);
		setNewItem(1);
		setNewItem(2);
		AddChild(m_pItemListUI);
	}
	{
		Entity* pEntity = new Entity();
		Entity::CreateLayoutComponent(pEntity, "image/clerk.png");
		auto pComponent = (LayoutComponent *) pEntity->GetComponent(eComponentKind_Layout);
		pComponent->SetOrtho(true);
		const float x = 1920.0f * 0.5f - 100.0f;
		const float y = 1080.0f * 0.5f - 80.0f;
		pEntity->SetPosition(x, -y, 0);
		AddChild(pEntity);
	}
	{
		// 「かう」or「かわない」ボタン
		{
			auto pBtnManager = new ButtonManager();
			const std::tuple<const char *, VEC3, uint8_t> btnList[] = {
					{"image/button_buy.png", VEC3(-250.0f, -400.0f, 0), TransferCommand::eCommand_Buy},
					{"image/button_buyCancel.png", VEC3(250.0f, -400.0f, 0), TransferCommand::eCommand_BuyCancel},
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				auto pBtn = pBtnManager->CreateButton(std::get<0>(btnList[i]));
				pBtn->SetPosition(std::get<1>(btnList[i]));
				pBtn->SetDecideCommand(std::get<2>(btnList[i]));
			}
			pBtnManager->SetVisible(false);
			pBtnManager->Lock();
			pBtnManager->SetControlPlayerId(nCurrentPlayerId);
			m_aButtonManager.emplace_back(pBtnManager);
		}
		// 戻るボタン
		{
			auto pBtnManager = new ButtonManager();
			const std::tuple<const char *, VEC3, uint8_t> btnList[] = {
					{"image/button_back.png", VEC3(700.0f, 300.0f, 0), TransferCommand::eCommand_ShopBack},
			};
			for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
				auto pBtn = pBtnManager->CreateButton(std::get<0>(btnList[i]));
				pBtn->SetPosition(std::get<1>(btnList[i]));
				pBtn->SetDecideCommand(std::get<2>(btnList[i]));
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
		m_pMessageWindow->SetDecideCommand(TransferCommand::eCommand_ShopNextMessage);
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

			// TODO 直度系のアイテムスキルは使い忘れることが多いので買ったら即反映にしたいけど
			// TODO 通信が関わってくるので少々面倒か…。
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
			const int nNum = AppParam::Get()->GetItemNum(nPlayerId, nSelectItemNo);
			if(nNum >= 3){
				AppParam::Get()->SetClear(true);
				m_nStep = eStep_ClearMessage;
			}
		}
		setNewItem(m_pItemListUI->GetCurrentItemIdx());
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

void Shop::setNewItem(int nIdx)
{
	// TODO 変更前と同じアイテムが選ばれるとアイテムが変わったかが分かりづらい
	// TODO 変更前と同じアイテムが選ばれないようにするか、変更時にアニメを追加するか？

	const auto pItemList = AppItemList::Get();
	const int nRandomVal = Random::GetSyncInt(0, 100);
	int nAppearSum = 0;
	for(int i = 0; i < pItemList->GetItemListSize(); ++i) {
		nAppearSum += pItemList->GetItemInfo(i).nAppearVal;
	}
	int nNewItemIdx = -1;
	float fAppearProbability = 0.0f;
	for(int i = 0; i < pItemList->GetItemListSize(); ++i) {
		const float fAppear = (float)pItemList->GetItemInfo(i).nAppearVal / (float)nAppearSum * 100.0f;
		fAppearProbability += roundf(fAppear + 0.99999f);	// 計算誤差を防ぐためにひとまず繰り上げ
		if((float)nRandomVal <= fAppearProbability){
			nNewItemIdx = i;
			break;
		}
	}

	ASSERT_MSG_A(nNewItemIdx >= 0, "nRandomVal[%d], fAppearProbability[%f]", nRandomVal, fAppearProbability);
	if(m_pItemListUI->GetRegisterItemNum() <= nIdx){
		m_pItemListUI->AddItemNo(nNewItemIdx);
	}
	else {
		m_pItemListUI->ChangeItemNo(nIdx, nNewItemIdx);
	}
}