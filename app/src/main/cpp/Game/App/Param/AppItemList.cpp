//
// Created by 葵ユメ on 2021/01/11.
//


#include <Util.h>
#include "AppItemList.h"
USE_SINGLETON_VARIABLE(AppItemList);

AppItemList::AppItemList()
: Singleton<AppItemList>()
, m_aItemInfo()
{
	DEBUG_LOG("Create AppItemList");
}

AppItemList::~AppItemList()
{
	DEBUG_LOG("Destroy AppItemList");
}

void AppItemList::Load()
{
	void* pBuffer = nullptr;
	long lSize = 0L;
	Engine::GetEngine()->LoadAsset("param/itemList.csv", &pBuffer, &lSize, true);
	std::string str = (const char*)pBuffer;
	std::vector<std::string>	aSplitText;
	Util::SplitCSVText(aSplitText, str);

	const int nParamNum = 7;
	m_aItemInfo.resize(aSplitText.size() / nParamNum);
	ASSERT(aSplitText.size() % nParamNum == 0);
	for(int i = 0; i < aSplitText.size(); ++i){
		const int nIdx = i / nParamNum;
		const int nParamNo = i % nParamNum;
		auto& info = m_aItemInfo[nIdx];
		if(nParamNo == 0){ info.name = aSplitText[i]; }
		if(nParamNo == 1){ info.useType = aSplitText[i]; }
		else if(nParamNo == 2){ info.fileName = aSplitText[i]; }
		else if(nParamNo == 3){ info.nCost = std::stoi(aSplitText[i]); }
		else if(nParamNo == 4){ info.explain = aSplitText[i]; }
		else if(nParamNo == 5){ info.skillName = aSplitText[i]; }
		else if(nParamNo == 6){ info.nAppearVal = std::stoi(aSplitText[i]); }
	}
	free(pBuffer);

	for(int i = 0; i < m_aItemInfo.size(); ++i) {
		auto& it = m_aItemInfo[i];
		DEBUG_LOG_A("ItemNo[%d]：[%s][%s][%s][%d][%s][%s][%d]\n",
				i + 1, it.name.c_str(), it.useType.c_str(), it.fileName.c_str(), it.nCost,
				it.explain.c_str(), it.skillName.c_str(), it.nAppearVal);
	}
}

const AppItemList::ItemInfo& AppItemList::GetItemInfo(int nIdx) const
{
	return m_aItemInfo[nIdx];
}

int AppItemList::GetItemNoFromName(const char* pName) const
{
	for(int i = 0; auto& it : m_aItemInfo){
		if(std::strcmp(it.name.c_str(), pName) == 0){
			return i;
		}
		i++;
	}
	return -1;
}

bool AppItemList::IsUsePossible(int nIdx, int nPlayerId) const
{
	const std::string& useType = m_aItemInfo[nIdx].useType;
	if(useType == "可能"){ return true; }
	if(useType == "不可"){ return false; }
	return false;
}