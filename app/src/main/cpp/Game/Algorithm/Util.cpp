//
// Created by 葵ユメ on 2021/02/23.
//


#include "Util.h"
#include <sstream>

namespace Util {

	void SplitCSVText(std::vector<std::string> &aSplitText, std::string text) {
		while (text.find_first_of('#') != std::string::npos) {
			auto start = text.find_first_of('#');
			auto last = text.find_first_of('\n', start);
			text.erase(start, (last - start) + 1);
		}
		while (text.find_first_of(',') != std::string::npos) {
			auto pos = text.find_first_of(',');
			text.erase(pos, 1);
		}
		if (text.find_last_of('\n') != std::string::npos) {
			auto pos = text.find_last_of('\n');
			text.erase(pos);
		}
		std::stringstream ss;
		ss << text;
		while (!ss.eof()) {
			std::string splitText;
			ss >> splitText;
			// BOMコードを除外
			if (splitText.size() == 3) {
				if (splitText[0] == 0xEF && splitText[1] == 0xBB && splitText[2] == 0xBF) {
					continue;
				}
			}
			if (splitText.size() > 0) {
				aSplitText.emplace_back(splitText);
			}
		}
	}
}