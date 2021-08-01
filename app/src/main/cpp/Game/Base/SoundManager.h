//
// Created by 葵ユメ on 2021/07/25.
//

#ifndef MONSTERFRIENDS_SOUNDMANAGER_H
#define MONSTERFRIENDS_SOUNDMANAGER_H

#include <unordered_map>
#include <string>

typedef int SoundResourceLabel;
typedef int SoundStreamHandle;

constexpr SoundResourceLabel SoundInvalidLabel = -1;
constexpr SoundStreamHandle SoundInvalidHandle = -1;

class SoundManager
{
	struct SoundInfo
	{
		bool bLoad = false;
		SoundResourceLabel label = SoundInvalidLabel;
	};

public:
	SoundManager();
	~SoundManager();

public:
	void Initialize();
	void Finalize();

	SoundResourceLabel LoadSE(const char* pPath);
	void UnLoadSE(SoundResourceLabel label);
	bool IsLoadSE(SoundResourceLabel label) const;

	SoundStreamHandle PlaySE(SoundResourceLabel label, float fLeftVol, float fRightVol, bool bLoop);
	void StopSE(SoundStreamHandle handle);

public:
	void LoadCompleteCallBack(SoundResourceLabel label, int nStatus);

private:
	bool getSoundInfoFromLabel(SoundResourceLabel label, SoundInfo* pInfo) const;
	bool getSoundPathFromLabel(SoundResourceLabel label, const std::string** pPath) const;

private:
	std::unordered_map<std::string, SoundInfo>	m_SoundMap;
};

#endif //MONSTERFRIENDS_SOUNDMANAGER_H
