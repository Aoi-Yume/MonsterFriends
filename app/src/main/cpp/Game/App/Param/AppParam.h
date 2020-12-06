//
// Created by 葵ユメ on 2020/12/06.
//


#ifndef AOIYUME_APP_PARAM_H
#define AOIYUME_APP_PARAM_H

#include "../Engine/Engine.h"
#include <Singleton.h>

class AppParam : public Singleton<AppParam>
{
	friend  class Singleton<AppParam>;

public:
	struct CharacterInfo {
		char charaName[64];
		int nKizunaPoint;
	};

public:
	void SetCharaName(const char* pName);
	const char* GetCharaName() const;

	void AddKizunaPoint(int nAdd);
	int GetKizunaPoint() const;

protected:
	AppParam();
	virtual ~AppParam();

private:
	AppParam(const AppParam&) = delete;
	void operator=(const AppParam&) = delete;

private:
	CharacterInfo m_CharaInfo;
};

#endif