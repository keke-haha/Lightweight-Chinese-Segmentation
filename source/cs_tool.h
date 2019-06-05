/*
 * tool.h
 *
 *  Created on: 2019年5月11日
 *      Author: 一步之遥
 */

#ifndef CS_TOOL_H_
#define CS_TOOL_H_

#include <iostream>
#include <vector>

#include "cs_define.h"

namespace cs {

	using std::vector;
	using std::string;
	using std::pair;

	vector <_char> sentence_to_char(const _sentence& sentence);         // 句子转字符
	vector <_char> word_to_char(const _word& word);                     // 词语转字符
	_word char_to_word(const vector <_char>& char_list);                // 字符转词语
	_sentence char_to_sentence(const vector <_char>& char_list);        // 字符转句子

	_char string_to_char(const string& str);                            // string转字符
	_word string_to_word(const string& str);                            // string转词语
	_sentence string_to_sentence(const string& str);                    // string转句子

	string char_to_string(const _char& ch);                             // 字符转化成string
	string word_to_string(const _word& word);                           // 词语转化成string
	string sentence_to_string(const _sentence& sentence);               // 句子转化成string

	uint sentence_char_num(const _sentence & sentence);                 // 句子的字符数
	uint word_char_num(const _word & word);                             // 词语的字符数

	vector <string> split_string(const string& str, const char& ch);    // 根据字符切割字符串
	string remove_string(const string& str, const char& ch);            // 删除字符串中某一字符

	double add(double _value1, double _value2);                         // 特殊加法防爆double
	double sup(double _value1, double _value2);                         // 特殊减法防爆double
	
	charState int_to_charState(int val);                                // int转charState
	charState uint_to_charState(uint val);                              // uint转charState
	int charState_to_int(const charState& val);                         // charState转int
	uint charState_to_uint(const charState& val);                       // charState转uint


	// 使用KMP算法查找

	typedef pair <uint, uint> pair_uint;

	vector <int> get_fail(const vector <_char>& char_list);                                   // 获得fail数组
	vector <pair_uint> find_word(const _word word, const vector <_sentence> sentence_list);   // 使用KMP查词

	uint show_tool_1(uint molecule, uint denominator, uint sum);        // 获得当前百分比值的数量

}


#endif /* CS_TOOL_H_ */
