//
// Created by 葵ユメ on 2021/01/11.
//


#include <sstream>
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
	splitCSVText(aSplitText, str);

	const int nParamNum = 4;
	m_aItemInfo.resize(aSplitText.size() / nParamNum);
	for(int i = 0; i < aSplitText.size(); ++i){
		const int nIdx = i / nParamNum;
		const int nParamNo = i % nParamNum;
		auto& info = m_aItemInfo[nIdx];
		if(nParamNo == 0){ info.name = aSplitText[i]; }
		else if(nParamNo == 1){ info.fileName = aSplitText[i]; }
		else if(nParamNo == 2){ info.nCost = atoi(aSplitText[i].c_str()); }
		else if(nParamNo == 3){ info.explain = aSplitText[i]; }
	}
	for(int i = 0; i < m_aItemInfo.size(); ++i) {
		auto& it = m_aItemInfo[i];
		DEBUG_LOG_A("ItemNo[%d]：[%s][%s][%d][%s]\n",
				i + 1, it.name.c_str(), it.fileName.c_str(), it.nCost, it.explain.c_str());
	}
}

const AppItemList::ItemInfo& AppItemList::GetItemInfo(int nIdx) const
{
	return m_aItemInfo[nIdx];
}

void AppItemList::splitCSVText(std::vector<std::string>& aSplitText, std::string text)
{
	while(text.find_first_of('#') != std::string::npos){
		auto start = text.find_first_of('#');
		auto last = text.find_first_of('\n', start);
		text.erase(start, (last - start) + 1);
	}
	while(text.find_first_of(',') != std::string::npos){
		auto pos = text.find_first_of(',');
		text.erase(pos, 1);
	}
	if(text.find_last_of('\n') != std::string::npos){
		auto pos = text.find_last_of('\n');
		text.erase(pos);
	}
	std::stringstream ss;
	ss << text;
	while(!ss.eof()){
		std::string splitText;
		ss >> splitText;
		// BOMコードを除外
		if(splitText.size() == 3){
			if(splitText[0] == 0xEF && splitText[1] == 0xBB && splitText[2] == 0xBF){
				continue;
			}
		}
		if(splitText.size() > 0) {
			aSplitText.emplace_back(splitText);
		}
	}
}