#pragma once
/*
 * cs_char.h
 *
 *  Created on: 2019年5月11日
 *      Author: 一步之遥
 */

#ifndef CS_CHAR_H_
#define CS_CHAR_H_

#include <map>
#include "cs_define.h"
using std::map;

namespace cs {

	class charDictionary {
	public:
		charDictionary();
		~charDictionary();
		void add_char_info(const _char& ch, const charFlag& flag);               // 添加字符信息

		void add_trad_simp_relationship(
				const _char& _ch1,                                               // 添加繁简转化关系;
				const _char& _ch2,                                               // flag = false 添加简体转繁体关系
				bool flag = false);                                              // flag = true  添加繁体转简体关系

		void add_full_half_relationship(const _char& full, const _char& half);   // 添加全角半角转化关系
		bool is_legal_char(const _char& ch);                                     // 判断字符合法
		bool is_chinese(const _char& ch);                                        // 判断中文
		bool is_simp(const _char& ch);                                           // 判断简体
		bool is_trad(const _char& ch);                                           // 判断繁体
		bool is_english(const _char& ch);                                        // 判断英文
		bool is_number(const _char& ch);                                         // 判断数字
		bool is_symbol(const _char& ch);                                         // 判断标点符号
		bool is_full(const _char& ch);                                           // 判断全角字符
		bool is_half(const _char& ch);                                           // 判断半角字符
		_char simp_to_trad(const _char& simp);                                   // 简体转化成繁体
		_char trad_to_simp(const _char& trad);                                   // 繁体转化成简体
		_char half_to_full(const _char& half);                                   // 半角转化成全角
		_char full_to_half(const _char& full);                                   // 全角转化成半角

	private:
		map <_char, charFlag> charInfo;                                          // 存储所有字符信息
		map <_char, _char> tradToSimp;                                           // 存储繁转简
		map <_char, _char> simpToTrad;                                           // 存储简转繁
		map <_char, _char> fullToHalf;                                           //全角转半角
		map <_char, _char> halfToFull;                                           //半角转全角
	};

}



#endif /* CS_CHAR_H_ */
