#pragma once
/*
 * alldefine.h
 *
 *  Created on: 2019年5月11日
 *      Author: 一步之遥
 */

#ifndef CS_DEFINE_H_
#define CS_DEFINE_H_

#include <string>

namespace cs {

	enum charFlag {
		_simple = 0,         // 简体汉字
		_traditional = 1,    // 繁体汉字
		_english = 2,        // 英文
		_number = 3,         // 数字
		_symbol = 4,         // 标点符号
		_unknow = 5          // 未知
	};

	enum charState {
		_begin = 0,           /// 词头
		_middle = 1,          /// 词中
		_end = 2,             /// 词尾
		_single = 3,          /// 独立词
		_root = 4             /// 开始
	};

	typedef long long int ll;
	typedef unsigned int uint;
	typedef unsigned long long int ull;
	typedef unsigned char uchar;

	typedef std::string _sentence;       // 句子的存储方式
	typedef std::string _word;           // 词语的存储方式
	typedef unsigned int _char;          // 字符的存储方式

	const _char _char_space = 0;         // 空格

	/// 用unicode和utf-8的转换
	const uint utf8_one_bit = 127;       // 只需要一个UTF-8字节流的最大unicode编码
	const uint utf8_two_bit = 2047;      // 只需要两个UTF-8字节流的最大unicode编码
	const uint utf8_three_bit = 65535;   // 只需要三个UTF-8字节流的最大unicode编码
	const uint utf8_four_bit = 1114111;  // 只需要四个UTF-8字节流的最大unicode编码
	const uint or_val = 0xFFFFFF00;      // 用于添加高位1

	const double inf_min = -1e10;        // 最小值

}



#endif /* CS_DEFINE_H_ */
