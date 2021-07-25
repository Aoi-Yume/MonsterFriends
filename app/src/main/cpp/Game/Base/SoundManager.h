//
// Created by 葵ユメ on 2021/07/25.
//

#ifndef MONSTERFRIENDS_SOUNDMANAGER_H
#define MONSTERFRIENDS_SOUNDMANAGER_H

#include <unordered_map>
#include <string>

typedef int SoundHandle;

class SoundManager
{
	struct SoundInfo
	{
		bool bLoad = false;
		SoundHandle Handle = -1;
	};

public:
	SoundManager();
	~SoundManager();

public:
	void Initialize();
	void Finalize();

	SoundHandle LoadSE(const char* pPath);
	void UnLoadSE(SoundHandle handle);
	bool IsLoadSE(SoundHandle handle) const;

	void PlaySE(SoundHandle handle, float fLeftVol, float fRightVol, bool bLoop);
	void StopSE(SoundHandle handle);

public:
	void LoadCompleteCallBack(SoundHandle handle, int nStatus);

private:
	bool getSoundInfoFromHandle(SoundHandle handle, SoundInfo* pInfo) const;
	bool getSoundPathFromHandle(SoundHandle handle, const std::string** pPath) const;

private:
	std::unordered_map<std::string, SoundInfo>	m_SoundMap;
};

#endif //MONSTERFRIENDS_SOUNDMANAGER_H
