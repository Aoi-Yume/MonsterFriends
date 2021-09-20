//
// Created by 葵ユメ on 2018/08/25.
//


#include <CameraComponent.h>
#include <AppParam.h>
#include <AppItemList.h>
#include "InformationPlate.h"
#include <TransferManager.h>

//==========================================
//==========================================
CristalIcon::CristalIcon()
: GameEntity()
{

}

//------------------------------------------
//------------------------------------------
CristalIcon::~CristalIcon()
{

}

//------------------------------------------
//------------------------------------------
void CristalIcon::GameEntitySetup(const void *param)
{
	Super::GameEntitySetup(param);
}

//------------------------------------------
//------------------------------------------
void CristalIcon::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);
}

//------------------------------------------
//------------------------------------------
void CristalIcon::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}

//------------------------------------------
//------------------------------------------
void CristalIcon::Setup()
{
	Entity::CreateTransformComponent(this);
	{
		Entity* pEntity = new Entity();
		CreateLayoutComponent(pEntity, "image/point_cristal_gray.png")->SetOrtho(true);
		AddChild(pEntity);
	}
	{
		Entity* pEntity = new Entity();
		CreateLayoutComponent(pEntity, "image/point_cristal.png")->SetOrtho(true);
		AddChild(pEntity);
		pEntity->SetVisible(false);
	}
}

//------------------------------------------
//------------------------------------------
void CristalIcon::Change(bool bGray)
{
	if(bGray){
		GetChild(eChild_Gray)->SetVisible(true);
		GetChild(eChild_Color)->SetVisible(false);
	}
	else{
		GetChild(eChild_Gray)->SetVisible(false);
		GetChild(eChild_Color)->SetVisible(true);
	}
}

//==========================================
//==========================================
InformationPlate::InformationPlate()
: GameEntity()
, m_nCurrentCristal(0)
{
	DEBUG_LOG("Create InformationPlate");
}

//------------------------------------------
//------------------------------------------
InformationPlate::~InformationPlate()
{
}

//------------------------------------------
//------------------------------------------
void InformationPlate::GameEntitySetup(const void* param)
{
	Super::GameEntitySetup(param);
	const int nCurrentPlayerId = AppParam::Get()->GetNetworkInfo().nCurrentPlayerId;
	{
		Entity::CreateLayoutComponent(this, "image/point_plate.png");
		auto pComponent = (LayoutComponent *) GetComponent(eComponentKind_Layout);
		pComponent->SetOrtho(true);
	}
	{
		Entity *pEntity = new Entity();
		Entity::CreateTextImageComponent(pEntity, "", 32)->SetOrtho(true);
		AddChild(pEntity);
		updateKizunaRemain(nCurrentPlayerId);
	}
	{
		Entity* pEntity = new Entity();
		Entity::CreateTextImageComponent(pEntity, "", 32)->SetOrtho(true);
		AddChild(pEntity);
		updateKizunaPlusPoint(nCurrentPlayerId);
	}
	{
		Entity* pEntity = new Entity();
		Entity::CreateTextImageComponent(pEntity, "", 32)->SetOrtho(true);
		AddChild(pEntity);
		updateBattlePlusPoint(nCurrentPlayerId);
	}
	{
		const int nItemNo = AppItemList::Get()->GetItemNoFromName("キズナクリスタル");
		m_nCurrentCristal = AppParam::Get()->GetItemNum(nCurrentPlayerId, nItemNo);
		for(int i = 0; i < AppParam::Get()->GetClearPoint(); ++i) {
			CristalIcon* pIcon = new CristalIcon();
			pIcon->Setup();
			pIcon->SetPosition(i * 32.0f - 110.0f, -40.0f, 0);
			pIcon->Change(i >= m_nCurrentCristal);
			AddChild(pIcon);
		}
		updateCristalNum(nCurrentPlayerId);
	}
	const float fPosX = -1920.0f * 0.4f;
	const float fPosY = 1080.0f * 0.42f;
	SetPosition(fPosX, fPosY, 0);
}

//------------------------------------------
//------------------------------------------
void InformationPlate::GameEntityUpdate(const void* param)
{
	Super::GameEntityUpdate(param);
}

//------------------------------------------
//------------------------------------------
void InformationPlate::EntityUpdate(GameMessage message, const void* param)
{
	Super::EntityUpdate(message, param);
}

//------------------------------------------
//------------------------------------------
void InformationPlate::UpdatePlate()
{
	const int nCurrentPlayerId = AppParam::Get()->GetNetworkInfo().nCurrentPlayerId;
	updateKizunaRemain(nCurrentPlayerId);
	updateKizunaPlusPoint(nCurrentPlayerId);
	updateBattlePlusPoint(nCurrentPlayerId);
	updateCristalNum(nCurrentPlayerId);
}

//------------------------------------------
//------------------------------------------
void InformationPlate::updateKizunaRemain(int nCurrentPlayerId)
{
	Entity* pChild = GetChild(eChild_Point);
	auto pText = (TextImageComponent *) pChild->GetComponent(eComponentKind_Layout);
	char pointStr[64];
	std::snprintf(pointStr, sizeof(pointStr), "%d", AppParam::Get()->GetKizunaPoint(nCurrentPlayerId));
	pointStr[sizeof(pointStr) - 1] = '\0';
	pText->SetText(pointStr);
	const int nLength = strlen(pointStr);
	// TODO 内部的にセンタリングや左揃え指定出来るようにする(現状多少ずれる)
	pChild->SetPosition(100.0f - ((float)nLength * 0.5f) * 16.0f, 40.0f, 0);
}

//------------------------------------------
//------------------------------------------
void InformationPlate::updateKizunaPlusPoint(int nCurrentPlayerId)
{
	Entity* pChild = GetChild(eChild_PointPlus);
	auto pText = (TextImageComponent *) pChild->GetComponent(eComponentKind_Layout);
	char pointStr[64];
	std::snprintf(pointStr, sizeof(pointStr), "%d", getKizunaPlusPoint(nCurrentPlayerId));
	pointStr[sizeof(pointStr) - 1] = '\0';
	pText->SetText(pointStr);
	const int nLength = strlen(pointStr);
	// TODO 内部的にセンタリングや左揃え指定出来るようにする(現状多少ずれる)
	pChild->SetPosition(-20.0f - ((float)nLength * 0.5f) * 16.0f, 2.0f, 0);
}

//------------------------------------------
//------------------------------------------
void InformationPlate::updateBattlePlusPoint(int nCurrentPlayerId)
{
	Entity* pChild = GetChild(eChild_BattlePlus);
	auto pText = (TextImageComponent *) pChild->GetComponent(eComponentKind_Layout);
	char pointStr[64];
	std::snprintf(pointStr, sizeof(pointStr), "%d", getBattlePlusPoint(nCurrentPlayerId));
	pointStr[sizeof(pointStr) - 1] = '\0';
	pText->SetText(pointStr);
	const int nLength = strlen(pointStr);
	// TODO 内部的にセンタリングや左揃え指定出来るようにする(現状多少ずれる)
	pChild->SetPosition(100.0f - ((float)nLength * 0.5f) * 16.0f, 2.0f, 0);
}

//------------------------------------------
//------------------------------------------
void InformationPlate::updateCristalNum(int nCurrentPlayerId)
{
	const int nItemNo = AppItemList::Get()->GetItemNoFromName("キズナクリスタル");
	const int nCurrentItemNum = AppParam::Get()->GetItemNum(nCurrentPlayerId, nItemNo);
	const int nAddNum = nCurrentItemNum - m_nCurrentCristal;
	for(int i = 0; i < nAddNum; ++i) {
		auto pIcon = GetChild<CristalIcon*>(eChild_Cristal + m_nCurrentCristal + i);
		pIcon->Change(false);
	}
	m_nCurrentCristal = nCurrentItemNum;
}

//------------------------------------------
//------------------------------------------
int InformationPlate::getKizunaPlusPoint(int nCurrentPlayerId) const
{
	const int nSkillNo = SKILL_LIST()->GetSkillNoFromSkillName("キズナプラス");
	return AppParam::Get()->GetUseSkillInfo(nCurrentPlayerId)[nSkillNo].Param;
}

//------------------------------------------
//------------------------------------------
int InformationPlate::getBattlePlusPoint(int nCurrentPlayerId) const
{
	const int nSkillNo = SKILL_LIST()->GetSkillNoFromSkillName("バトルプラス");
	return AppParam::Get()->GetUseSkillInfo(nCurrentPlayerId)[nSkillNo].Param;
}
