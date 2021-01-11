//
// Created by 葵ユメ on 2021/01/04.
//


#ifndef AOIYUME_RANDOM_H
#define AOIYUME_RANDOM_H

#include <random>

class Random
{
public:
	static void Initialize();

	static void SetSeed(unsigned int uSeed);
	static void SetSyncSeed(unsigned int uSeed);

	static int GetInt();
	//! @note nMin以上nMax以下を返す
	static int GetInt(int nMin, int nMax);
	//! @note fMin以上fMax以下を返す
	static float GetFloat(float fMin, float fMax);

	static int GetSyncInt();
	//! @note nMin以上nMax以下を返す
	static int GetSyncInt(int nMin, int nMax);
	//! @note fMin以上fMax以下を返す
	static float GetSyncFloat(float fMin, float fMax);

	static unsigned int GetSeed();
	static unsigned int GetSyncSeed();

	template<typename T>
	static void Shuffle(T& rArray, int nNum)
	{
		for (int i = 0; i < nNum; ++i) {
			const int idx = GetInt(0, nNum - i - 1) + i;
			std::swap(rArray[i], rArray[idx]);
		}
	}

	template<typename T>
	static void SyncShuffle(T& pArray, int nNum)
	{
		for (int i = 0; i < nNum; ++i) {
			const int idx = GetSyncInt(0, nNum - i - 1) + i;
			std::swap(pArray[i], pArray[idx]);
		}
	}
};


#endif