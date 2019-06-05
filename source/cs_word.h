#pragma once
/*
 * word.h
 *
 *  Created on: 2019年5月15日
 *      Author: 一步之遥
 */

#include <vector>
#include <string>

#include "cs_define.h"
#include "cs_trie.h"

namespace cs {
	class wordDictionary {
	protected:
		acAutomaton _ac_tree;                                                                        // AC自动机用于快速查询
		uint _word_size;                                                                             // 词语数量
		uint _max_word_length;                                                                       // 词语最大长度
		
	public:
		wordDictionary();                                                                            // 构造函数
		~wordDictionary();                                                                           // 析构函数
		void initialization(const std::vector <std::string> file_path_list);                         // 初始化函数
		void push(const _word& word);                                                                // 添加词语
		void push(const vector<_char>& char_list);                                                   // 添加词语(字符)
		bool find(const _word& word);                                                                // 查找词语
		bool find(const vector<_char>& char_list);                                                   // 查找词语(字符)
		std::vector <_word> dictionary_revise(const std::vector <_word> word_list);                  // 词典对分词进行修正
		std::vector <_word> dictionary_segmention(const _sentence& sentence, bool flag = false);     // 利用词典分词

	};
}
