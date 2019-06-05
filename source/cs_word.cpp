/*
 * word.cpp
 *
 *  Created on: 2019年5月15日
 *      Author: 一步之遥
 */

#include <fstream>
#include <algorithm>

#include "cs_word.h"
#include "cs_tool.h"

namespace cs {
	wordDictionary::wordDictionary() {
		_word_size = 0;
		_max_word_length = 10;
	}

	wordDictionary::~wordDictionary() {
		_ac_tree.clear();
	}

	void wordDictionary::initialization(const vector <std::string> file_path_list) {
		std::ifstream file_in;
		vector <string> word_list;
		string word;
		printf("开始词典读取\n");
		for (std::string file_path : file_path_list) {
			file_in.open(file_path.c_str());
			if (!file_in.is_open()) {
				std::cerr << "词典文件 " << file_path << " 打开失败!!!" << std::endl;
				continue;
			}
			while(file_in >> word)
				word_list.push_back(word);
			file_in.close();
		}
		sort(word_list.begin(), word_list.end());
		word_list.erase(unique(word_list.begin(), word_list.end()), word_list.end());
		printf("词典读取结束\n");
		printf("开始建立词表\n");
		uint all_sum = word_list.size();
		uint p = show_tool_1(1, 20, all_sum) - 1, q = 1;
		vector <_char> char_list;
		for(int i = 0, s = word_list.size(); i < s; i ++) {
			char_list = word_to_char(word_list[i]);
			if(char_list.size() <= _max_word_length)
				_ac_tree.push(char_list);
			if(i == p) {
				for(int j = 1; j <= 20; j ++) {
					if(j <= q) printf("#");
					else printf(" ");
				}
				std::cout << "\t" << (p * 100.0 / all_sum) << "%" << std::endl;
				p = show_tool_1(++ q, 20, all_sum) - 1;
			}
		}
		printf("词表建立完成\n");
		_ac_tree.build_fail();
	}

	void wordDictionary::push(const _word& word) {
		_ac_tree.push(word_to_char(word));
	}

	void wordDictionary::push(const vector <_char>& char_list) {
		_ac_tree.push(char_list);
	}

	bool wordDictionary::find(const _word& word) {
		return _ac_tree.exist(word_to_char(word));
	}

	bool wordDictionary::find(const vector <_char>& char_list) {
		return _ac_tree.exist(char_list);
	}

	vector <_word> wordDictionary::dictionary_revise(const vector <_word> word_list) {
		/// 字典修正
		vector <_word> ret;
		vector <_char> char_list;
		for (int i = 0, wls = word_list.size(); i < wls; ) {
			char_list = word_to_char(word_list[i]);
			uint now_node = _ac_tree.substring(char_list);
			if(!now_node) {
				ret.push_back(word_list[i ++]);
				continue;
			}
			_word now_word = word_list[i];
			_word ans_word = now_word;
			uint now_pos = i, ans_pos = i, len = char_list.size();
			while(true) {
				if(++ now_pos >= wls) break;
				now_word += word_list[now_pos];
				char_list = word_to_char(word_list[now_pos]);
				len += char_list.size();
				if(len > _max_word_length) break;
				now_node = _ac_tree.substring(char_list, now_node);
				if(!now_node) break;
				if(len == _ac_tree.last_word(now_node)) {
					ans_word = now_word;
					ans_pos = now_pos;
				}
			}
			ret.push_back(ans_word);
			i = ans_pos + 1;
		}
		return ret;
	}

	vector <_word> wordDictionary::dictionary_segmention(const _sentence& sentence, bool flag) {
		vector <_char> char_list = sentence_to_char(sentence);
		vector <pair_uint> word_pair = _ac_tree.search(char_list, flag);
		vector < _word> ret;
		for (int i = 0, wps = word_pair.size(); i < wps; i++) {
			int st = word_pair[i].first, ed = word_pair[i].second;
			_word tmp_word;
			while (st <= ed) {
				tmp_word += char_to_string(char_list[st]);
				st++;
			}
			ret.push_back(tmp_word);
		}
		return ret;
	}
}
