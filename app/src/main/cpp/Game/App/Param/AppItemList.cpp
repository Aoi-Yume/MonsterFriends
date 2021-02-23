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

	const int nParamNum = 5;
	m_aItemInfo.resize(aSplitText.size() / nParamNum);
	for(int i = 0; i < aSplitText.size(); ++i){
		const int nIdx = i / nParamNum;
		const int nParamNo = i % nParamNum;
		auto& info = m_aItemInfo[nIdx];
		if(nParamNo == 0){ info.name = aSplitText[i]; }
		else if(nParamNo == 1){ info.fileName = aSplitText[i]; }
		else if(nParamNo == 2){ info.nCost = atoi(aSplitText[i].c_str()); }
		else if(nParamNo == 3){ info.explain = aSplitText[i]; }
		else if(nParamNo == 4){ info.skillName = aSplitText[i]; }
	}
	for(int i = 0; i < m_aItemInfo.size(); ++i) {
		auto& it = m_aItemInfo[i];
		DEBUG_LOG_A("ItemNo[%d]：[%s][%s][%d][%s][%s]\n",
				i + 1, it.name.c_str(), it.fileName.c_str(), it.nCost,
				it.explain.c_str(), it.skillName.c_str());
	}
}

const AppItemList::ItemInfo& AppItemList::GetItemInfo(int nIdx) const
{
	return m_aItemInfo[nIdx];
}