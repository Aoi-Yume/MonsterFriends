//
// Created by 葵ユメ on 2021/10/09.
//


#include <Util.h>
#include "AppShadowList.h"
USE_SINGLETON_VARIABLE(AppShadowList);

AppShadowList::AppShadowList()
: Singleton<AppShadowList>()
, m_aShadowInfo()
{
	DEBUG_LOG("Create AppShadowList");
}

AppShadowList::~AppShadowList()
{
	DEBUG_LOG("Destroy AppShadowList");
}

void AppShadowList::Load()
{
	void* pBuffer = nullptr;
	long lSize = 0L;
	Engine::GetEngine()->LoadAsset("param/shadowList.csv", &pBuffer, &lSize, true);
	std::string str = (const char*)pBuffer;
	std::vector<std::string>	aSplitText;
	Util::SplitCSVText(aSplitText, str);

	const int nParamNum = 4;
	m_aShadowInfo.resize(aSplitText.size() / nParamNum);
	ASSERT(aSplitText.size() % nParamNum == 0);
	for(int i = 0; i < aSplitText.size(); ++i){
		const int nIdx = i / nParamNum;
		const int nParamNo = i % nParamNum;
		auto& info = m_aShadowInfo[nIdx];
		if(nParamNo == 0){ info.name = aSplitText[i]; }
		else if(nParamNo == 1){ info.fileName = aSplitText[i]; }
		else if(nParamNo == 2){ info.offsetX = std::stof(aSplitText[i]); }
		else if(nParamNo == 3){ info.offsetY = std::stof(aSplitText[i]); }
	}
	free(pBuffer);

	for(int i = 0; i < m_aShadowInfo.size(); ++i) {
		auto& it = m_aShadowInfo[i];
		DEBUG_LOG_A("ShadowNo[%d]：[%s][%s][%.1f][%.1f]\n",
				i + 1, it.name.c_str(), it.fileName.c_str(), it.offsetX, it.offsetY);
	}
}

const AppShadowList::ShadowInfo& AppShadowList::GetShadowInfo(int nIdx) const
{
	return m_aShadowInfo[nIdx];
}

const AppShadowList::ShadowInfo& AppShadowList::GetShadowInfo(const char* pName) const
{
	const int nId = GetShadowInfoIdx(pName);
	return m_aShadowInfo[nId];
}

const int AppShadowList::GetShadowInfoIdx(const char* pName) const
{
	for(int i = 0; i < m_aShadowInfo.size(); ++i){
		if(m_aShadowInfo[i].name == pName){
			return i;
		}
	}
	ASSERT_MSG(false, "Not Found ShadowInfo");
	return -1;
}