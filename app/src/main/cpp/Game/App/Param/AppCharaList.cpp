//
// Created by 葵ユメ on 2021/03/27.
//


#include <Util.h>
#include "AppCharaList.h"
USE_SINGLETON_VARIABLE(AppCharaList);

AppCharaList::AppCharaList()
: Singleton<AppCharaList>()
, m_aCharaInfo()
{
	DEBUG_LOG("Create AppCharaList");
}

AppCharaList::~AppCharaList()
{
	DEBUG_LOG("Destroy AppCharaList");
}

void AppCharaList::Load()
{
	void* pBuffer = nullptr;
	long lSize = 0L;
	Engine::GetEngine()->LoadAsset("param/charaList.csv", &pBuffer, &lSize, true);
	std::string str = (const char*)pBuffer;
	std::vector<std::string>	aSplitText;
	Util::SplitCSVText(aSplitText, str);

	const int nParamNum = 5;
	m_aCharaInfo.resize(aSplitText.size() / nParamNum);
	assert(aSplitText.size() % nParamNum == 0);
	for(int i = 0; i < aSplitText.size(); ++i){
		const int nIdx = i / nParamNum;
		const int nParamNo = i % nParamNum;
		auto& info = m_aCharaInfo[nIdx];
		if(nParamNo == 0){ info.name = aSplitText[i]; }
		else if(nParamNo == 1){ info.fileName = aSplitText[i]; }
		else if(nParamNo == 2){ info.nDice = std::stoi(aSplitText[i]); }
		else if(nParamNo == 3){ info.fDiceOffsetY = std::stof(aSplitText[i]); }
		else if(nParamNo == 4){ info.nGetPoint = std::stoi(aSplitText[i]); }
	}
	free(pBuffer);

	for(int i = 0; i < m_aCharaInfo.size(); ++i) {
		auto& it = m_aCharaInfo[i];
		DEBUG_LOG_A("EnemyNo[%d]：[%s][%s][%d][%.1f][%d]\n",
				i + 1, it.name.c_str(), it.fileName.c_str(), it.nDice, it.fDiceOffsetY, it.nGetPoint);
	}
}

const AppCharaList::CharaInfo& AppCharaList::GetCharaInfo(int nIdx) const
{
	return m_aCharaInfo[nIdx];
}