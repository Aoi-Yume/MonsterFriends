//
// Created by 葵ユメ on 2021/02/21.
//


#include <CameraComponent.h>
#include <AppParam.h>
#include "TransformComponent.h"
#include "LayoutComponent.h"
#include "CollisionComponent.h"
#include "ItemListUI.h"
#include <TransferManager.h>
#include <TransferCommand.h>
#include <Button/ButtonManager.h>
#include <Button/SimpleButton.h>
#include <AppItemList.h>

ItemListUI::ItemListUI()
: GameEntity()
, m_nCurrentItemIdx(0)
, m_nSelectItemNo(-1)
, m_aItemNo()
, m_aButtonManager()
{
	DEBUG_LOG("Create ItemListUI");
}

ItemListUI::~ItemListUI()
{
	for(auto& it : m_aButtonManager){
		delete it;
	}
}

void ItemListUI::GameEntitySetup(const void* param) {
	Super::GameEntitySetup(param);
	const int nCurrentPlayerId = AppParam::Get()->GetUIControlPlayerNo();

	// 表示分までは無効領域を詰めておく
	while(m_aItemNo.size() < eButtonVisibleNum){
		m_aItemNo.emplace_back(-1);
	}

	// 黒幕
	{
		Entity* pEntity = new Entity();
		Entity::CreateLayoutComponent(pEntity, "image/gray_screen.png");
		auto pComponent = (LayoutComponent *) pEntity->GetComponent(eComponentKind_Layout);
		pComponent->SetOrtho(true);
		pComponent->SetVisible(false);
		AddChild(pEntity);
	}
	// アイテムボタン
	{
		auto pBtnManager = new ButtonManager();
		const VEC3 btnList[eButtonVisibleNum + 1] = {
				VEC3(-450.0f, 0, 0),
				VEC3(0.0f, 0, 0),
				VEC3(450.0f, 0, 0),
				VEC3(900.0f, 0, 0),	// アニメ用
		};
		for (int i = 0; i < eButtonVisibleNum; ++i) {
			if(m_aItemNo[i] >= 0) {
				const auto &itemInfo = AppItemList::Get()->GetItemInfo(m_aItemNo[i]);
				auto pBtn = pBtnManager->CreateButton(itemInfo.fileName.c_str());
				pBtn->SetPosition(btnList[i]);
				pBtn->SetDecideCommand(TransferCommand::eCommand_PushItem_1 + i);
				pBtn->SetSelectSELabel(SE_LABEL_DECIDE);
			}
			else{
				auto pBtn = pBtnManager->CreateButton("image/shop_item_none.png");
				pBtn->SetPosition(btnList[i]);
				pBtn->SetDecideCommand(TransferCommand::eCommand_PushItem_1 + i);
				pBtn->SetSelectSELabel(nullptr);
			}
		}
		// アニメ用
		{
			auto pBtn = pBtnManager->CreateButton("image/shop_item_none.png");
			pBtn->SetPosition(btnList[eButtonVisibleNum]);
			//pBtn->SetDecideCommand(TransferCommand::eCommand_PushItem_1 + i);
			pBtn->SetSelectSELabel(nullptr);
		}
		// アニメの追加
		const int nCenter = eButtonVisibleNum / 2;
		for(int i = 0; i < pBtnManager->GetButtonSize(); ++i){
			auto pBtn = pBtnManager->GetButton(i);
			auto pAnim = pBtn->GetComponent<AnimationComponent*>(eComponentKind_Animation);
			std::pair<float, float> scaleL = {1, 1};
			std::pair<float, float> scaleR = {1, 1};
			std::pair<float, float> scaleB = {1, 1};
			std::pair<VEC4, VEC4> colorL = {VEC4(0.8f, 0.8f, 0.8f, 1.0f), VEC4(0.8f, 0.8f, 0.8f, 1.0f)};
			std::pair<VEC4, VEC4> colorR = {VEC4(0.8f, 0.8f, 0.8f, 1.0f), VEC4(0.8f, 0.8f, 0.8f, 1.0f)};
			std::pair<VEC4, VEC4> colorB = {VEC4(0.8f, 0.8f, 0.8f, 1.0f), VEC4(0.8f, 0.8f, 0.8f, 1.0f)};
			if(i == 0) { colorL.second.SetW(0.0f); }								// 一番左側
			else if(i == eButtonVisibleNum - 1){ colorR.second.SetW(0.0f); }		// 一番右側

			// 真ん中の左側
			if(i == nCenter - 1) {
				scaleR.second = 1.1f;
				colorR.second = VEC4::One();
			}
			// 真ん中の右側
			else if(i == nCenter + 1) {
				scaleL.second = 1.1f;
				colorL.second = VEC4::One();
			}
			// 真ん中
			else if(i == nCenter) {
				scaleB = {1.1f, 1.1f};
				scaleL = scaleR = {1.1f, 1.0f};
				colorB = { VEC4::One(), VEC4::One() };
				colorL = colorR= { VEC4::One(), VEC4(0.8f, 0.8f, 0.8f, 1.0f) };
			}

			if(i == eButtonVisibleNum) {
				colorL = colorR = {VEC4(0.8f, 0.8f, 0.8f, 0.0f), VEC4(0.8f, 0.8f, 0.8f, 1.0f) };
				colorB = {VEC4(1, 1, 1, 0), VEC4(1, 1, 1, 0)};
			}

			auto pLA = new LinearAnimation(std::move(scaleL), {}, {VEC3(0, 0, 0), VEC3(-450.0f, 0, 0)}, std::move(colorL));
			auto pRA = new LinearAnimation(std::move(scaleR), {}, {VEC3(0, 0, 0), VEC3(450.0f, 0, 0)}, std::move(colorR));
			auto pBase = new LinearAnimation(std::move(scaleB), {}, {}, std::move(colorB));
			pLA->SetCallBack([=](){ pBtn->Disable(); pAnim->Play("Base");});
			pRA->SetCallBack([=](){ pBtn->Disable(); pAnim->Play("Base");});
			pBase->SetCallBack([=](){ if(i == nCenter){ pBtn->Enable(); } });
			pAnim->AddAnimation("MoveLeft", pLA);
			pAnim->AddAnimation("MoveRight", pRA);
			pAnim->AddAnimation("Base", pBase);
		}
		pBtnManager->SetVisible(false);
		pBtnManager->Lock();
		pBtnManager->SetControlPlayerId(nCurrentPlayerId);
		m_aButtonManager.emplace_back(pBtnManager);
	}
	// ←→ボタン
	{
		auto pBtnManager = new ButtonManager();
		const std::tuple<const char *, VEC3, uint8_t> btnList[] = {
				{ "image/button_left.png", VEC3(-600.0f, 0, 0), TransferCommand::eCommand_PushItemLeft },
				{ "image/button_right.png", VEC3(600.0f, 0, 0), TransferCommand::eCommand_PushItemRight },
		};
		for (int i = 0; i < sizeof(btnList) / sizeof(btnList[0]); ++i) {
			auto pBtn = pBtnManager->CreateButton(std::get<0>(btnList[i]));
			auto pos = std::get<1>(btnList[i]);
			pBtn->SetPosition(pos);
			pBtn->SetDecideCommand(std::get<2>(btnList[i]));
			auto pAnim = pBtn->GetComponent<AnimationComponent *>(eComponentKind_Animation);
			{
				auto pAddAnim = new LinearAnimation({1, 1.2f}, {}, {}, {VEC4::One(), VEC4::One()}, 3.0f);
				pAddAnim->SetCallBack([=]() { pAnim->Play("loopE"); });
				pAnim->AddAnimation("loopS", pAddAnim);
			}
			{
				auto pAddAnim = new LinearAnimation({1.2f, 1.0f}, {}, {}, {VEC4::One(), VEC4::One()}, 3.0f);
				pAddAnim->SetCallBack([=]() { pAnim->Play("loopS"); });
				pAnim->AddAnimation("loopE", pAddAnim);
			}
			{
				pAnim->GetAnimation("Open")->SetCallBack([=]() { pAnim->Play("loopS"); });
			}
			{
				pAnim->GetAnimation("On")->SetCallBack([this, i](){
					const bool bMoveRight = (i == 0 );
					auto pBtnManager = m_aButtonManager[eButtonManager_ItemList];
					for(int btnIdx = 0; btnIdx < pBtnManager->GetButtonSize(); ++btnIdx){
						auto pBtn = pBtnManager->GetButton(btnIdx);
						pBtn->GetComponent<AnimationComponent*>(eComponentKind_Animation)->Play(bMoveRight ? "MoveRight" : "MoveLeft");
					}
					{
						auto pBtn = pBtnManager->GetButton(eButtonVisibleNum);
						VEC3 pos = pBtn->GetPosition();
						pos.SetX( (bMoveRight ? -std::abs(pos.X()) : std::abs(pos.X())) );
						pBtn->SetPosition(pos);
						updateInvisibleItemIcon(bMoveRight);
					}
				});
			}
			{
				pAnim->GetAnimation("Off")->SetCallBack([=](){ pAnim->Play("loopS"); });
			}
		}
		pBtnManager->SetVisible(false);
		pBtnManager->Lock();
		pBtnManager->SetControlPlayerId(nCurrentPlayerId);
		m_aButtonManager.emplace_back(pBtnManager);
	}

	// 子供のEntityに関してはこの関数を抜けた後にSetupが呼ばれるので
	// コンポーネントを参照するアイテム画像更新が呼べない
	// updateItemIcon();
//	updateItemButton();
}

void ItemListUI::GameEntityUpdate(const void* param)
{
	{
		const int nDecide = m_aButtonManager[eButtonManager_LeftRight]->GetDecide();
		if(nDecide >= 0){
			const int nButtonSize = m_aButtonManager[eButtonManager_ItemList]->GetButtonSize();
			bool bAnimEnd = true;
			for(int i = 0; i < nButtonSize; ++i){
				auto pBtn = m_aButtonManager[eButtonManager_ItemList]->GetButton(i);
				auto pAnim = pBtn->GetComponent<AnimationComponent*>(eComponentKind_Animation);
				if(pAnim->IsPlay("MoveLeft") || pAnim->IsPlay("MoveRight") ) {
					bAnimEnd = false;
					break;
				}
			}
			if(bAnimEnd) {
				const int nSize = m_aItemNo.size();
				if (nDecide == 0){
					m_nCurrentItemIdx = (m_nCurrentItemIdx + nSize - 1) % nSize;
				}
				else if(nDecide == 1){
					m_nCurrentItemIdx = (m_nCurrentItemIdx + 1) % nSize;
				}
				m_aButtonManager[eButtonManager_LeftRight]->Reset();
				updateItemIcon();
//				updateItemButton();
			}
		}
	}
	{
		const int nDecide = m_aButtonManager[eButtonManager_ItemList]->GetDecide();
		if(nDecide == 1){
			m_nSelectItemNo = m_aItemNo[m_nCurrentItemIdx];
			if(m_aItemNo[m_nCurrentItemIdx] == -1){
				m_aButtonManager[eButtonManager_ItemList]->Reset();
			}
			else{
				Lock();
			}
		}
	}

	Super::GameEntityUpdate(param);
}

void ItemListUI::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
	if(message != eGameMessage_Setup){
		for(auto& it : m_aButtonManager){
			it->Update(message, param);
		}
	}
}

void ItemListUI::Open()
{
	m_nCurrentItemIdx = 0;
	m_nSelectItemNo = -1;
	m_aButtonManager[eButtonManager_ItemList]->Unlock();
	m_aButtonManager[eButtonManager_ItemList]->SetVisible(true);
	m_aButtonManager[eButtonManager_ItemList]->Reset();
	m_aButtonManager[eButtonManager_LeftRight]->Unlock();
	m_aButtonManager[eButtonManager_LeftRight]->SetVisible(true);
	m_aButtonManager[eButtonManager_LeftRight]->Reset();

	auto p = GetChild(0)->GetComponent<LayoutComponent*>(eComponentKind_Layout);
	p->SetVisible(true);

	// 表示分までは無効領域を詰めておく
	while(m_aItemNo.size() < eButtonVisibleNum){
		m_aItemNo.emplace_back(-1);
	}
	for(int i = 0; i < m_aButtonManager[eButtonManager_ItemList]->GetButtonSize(); ++i){
		auto pBtn = m_aButtonManager[eButtonManager_ItemList]->GetButton(i);
		pBtn->GetComponent<AnimationComponent*>(eComponentKind_Animation)->Play("Base");
	}
	updateItemIcon();
//	updateItemButton();
}

void ItemListUI::Close()
{
	m_aButtonManager[eButtonManager_ItemList]->Lock();
	m_aButtonManager[eButtonManager_ItemList]->SetVisible(false);
	m_aButtonManager[eButtonManager_LeftRight]->Lock();
	m_aButtonManager[eButtonManager_LeftRight]->SetVisible(false);

	auto p = GetChild(0)->GetComponent<LayoutComponent*>(eComponentKind_Layout);
	p->SetVisible(false);
}

void ItemListUI::Lock()
{
	m_aButtonManager[eButtonManager_ItemList]->Lock();
	m_aButtonManager[eButtonManager_LeftRight]->Lock();
}
void ItemListUI::UnLock()
{
	m_aButtonManager[eButtonManager_ItemList]->Unlock();
	m_aButtonManager[eButtonManager_LeftRight]->Unlock();
}
void ItemListUI::Reset()
{
	m_aButtonManager[eButtonManager_ItemList]->Reset();
	m_aButtonManager[eButtonManager_LeftRight]->Reset();
	m_nSelectItemNo = -1;
}

void ItemListUI::AddItemNo(int nNo)
{
	m_aItemNo.emplace_back(nNo);
	// TODO 最初にアイテムを追加する場合はアイテム画像のEntityがセットアップされていない
	// TODO 途中からアイテム管理数を増やしたときはアイテム画像の更新を呼ぶべきだが…。
	// updateItemIcon();
}


void ItemListUI::ChangeItemNo(int nIdx, int nNo)
{
	m_aItemNo[nIdx] = nNo;
	updateItemIcon();
}

void ItemListUI::ClearItemNo()
{
	m_aItemNo.clear();
}

int ItemListUI::GetSelectItemNo() const
{
	return m_nSelectItemNo;
}

int ItemListUI::GetCurrentItemIdx() const
{
	return m_nCurrentItemIdx;
}

void ItemListUI::updateItemButton()
{
	const int nCenter = eButtonVisibleNum / 2;
	auto pBtnManager = m_aButtonManager[eButtonManager_ItemList];
	for(int i = 0; i < eButtonVisibleNum; ++i){
		auto pBtn = pBtnManager->GetButton(i);
		if(i == nCenter){
//			pBtn->SetGray(false);
			pBtn->Enable();
		}
		else{
//			pBtn->SetGray(true);
			pBtn->Disable();
		}
	}
};

void ItemListUI::updateItemIcon()
{
	const int nCenter = eButtonVisibleNum / 2;
	const int nListSize = m_aItemNo.size();

	int nStart = m_nCurrentItemIdx;
	for(int i = 0; i < nCenter; ++i){
		nStart = (nStart + nListSize - 1) % nListSize;
	}
	auto pBtnManager = m_aButtonManager[eButtonManager_ItemList];
	for(int i = 0; i < eButtonVisibleNum; ++i) {
		auto pBtn = pBtnManager->GetButton(i);
		auto p = (LayoutComponent *) pBtn->GetComponent(eComponentKind_Layout);
		if (m_aItemNo[nStart] >= 0) {
			const auto &itemInfo = AppItemList::Get()->GetItemInfo(m_aItemNo[nStart]);
			p->ChangeTexture(itemInfo.fileName.c_str());
			pBtn->SetSelectSELabel(SE_LABEL_DECIDE);
		} else {
			p->ChangeTexture("image/shop_item_none.png");
			pBtn->SetSelectSELabel(nullptr);
		}
		nStart = (nStart + 1) % nListSize;
	}
}

void ItemListUI::updateInvisibleItemIcon(bool bMoveRight)
{
	const int nCenter = eButtonVisibleNum / 2;
	const int nListSize = m_aItemNo.size();
	int nStart = m_nCurrentItemIdx;
	for(int i = 0; i < nCenter; ++i){
		nStart = (nStart + nListSize - 1) % nListSize;
	}
	if(bMoveRight){
		nStart = (nStart + nListSize - 1) % nListSize;
	}
	else{
		nStart = (nStart + eButtonVisibleNum) % nListSize;
	}
	auto pBtnManager = m_aButtonManager[eButtonManager_ItemList];
	auto pBtn = pBtnManager->GetButton(eButtonVisibleNum);
	auto p = (LayoutComponent *) pBtn->GetComponent(eComponentKind_Layout);
	if (m_aItemNo[nStart] >= 0) {
		const auto &itemInfo = AppItemList::Get()->GetItemInfo(m_aItemNo[nStart]);
		p->ChangeTexture(itemInfo.fileName.c_str());
		pBtn->SetSelectSELabel(SE_LABEL_DECIDE);
	} else {
		p->ChangeTexture("image/shop_item_none.png");
		pBtn->SetSelectSELabel(nullptr);
	}
}