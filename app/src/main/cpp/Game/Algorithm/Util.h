//
// Created by 葵ユメ on 2021/02/23.
//


#ifndef AOIYUME_UTIL_H
#define AOIYUME_UTIL_H

#include <vector>
#include <Random.h>

namespace Util {
	void SplitCSVText(std::vector<std::string> &aSplitText, std::string text);
	float Centering(float fWidth, int nNum, int nIdx, float fOffset = 0.0f);

	template <class T>
	void Shuffle(T& data, int nSize){
		for(int i = nSize - 1; i > 0; --i) {
			const int nRandIdx = Random::GetInt(0, i);
			std::swap(data[nSize - 1], data[nRandIdx]);
		}
	}
}

#endif