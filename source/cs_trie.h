#pragma once
/*
 * trie.h
 *
 *  Created on: 2019年5月11日
 *      Author: 一步之遥
 */

#ifndef CS_TRIE_H_
#define CS_TRIE_H_

#include <iostream>
#include <map>
#include <vector>

#include "cs_define.h"
#include "cs_tool.h"

namespace cs {

	using std::map;
	using std::vector;
	using std::pair;

	typedef _char trie_char;
	typedef pair <uint, uint> pair_uint;

	// 字典树节点
	struct trie_node {
		bool _exist;                                                // 到达该点的单词是否存在
		uint _dep;                                                  // 字典树深度
		vector <pair_uint> _nex;                                    // 下一节点下标

		trie_node(bool is_exist = false, uint dep = 0) {            // 构造
			_exist = is_exist;
			_dep = dep;
		}

		~trie_node() {                                              // 析构
			_nex.clear();
		}

		void push(const trie_char val, const uint address) {        // 添加下一节点下标
			_nex.push_back(pair_uint(val, address));
		}

		uint find(const trie_char& one_char) {                      // 查询下一节点下标
			int l = 0, r = _nex.size() - 1, mid;
			uint ans = 0;
			while(l <= r) {
				mid = (l + r) >> 1;
				if(_nex[mid].first == one_char) {
					ans = _nex[mid].second;
					break;
				} else if(_nex[mid].first < one_char)
					l = mid + 1;
				else
					r = mid - 1;
			}
			return ans;
		}

//		void show() {                                               // 展示函数
//			using std::cout;
//			using std::endl;
//			cout << "该点单词是否存在 : ";
//			if(_exist) cout << "存在" << endl;
//			else cout << "不存在" << endl;
//			cout << "该点深度 : " << _dep << endl;
//			for(int i = 0, s = _nex.size(); i < s; i ++) {
//				cout << char_to_string(_nex[i].first) << " : " << _nex[i].second << endl;
//			}
//		}
	};

	// 字典树
	class trie {
	protected:
		vector <trie_node> _tree;                                   // 数组存字典树
		uint _capacity;                                             // 节点数
		uint _size;                                                 // 单词数
		uint _max_word_length;                                      // 单词最长数
	public:
		trie(uint max_word_length = 10);                            // 构造
		~trie();                                                    // 析造
		uint size();                                                // 词语数量
		uint capacity();                                            // 节点数
		bool exist(const vector <trie_char> char_list);             // 判断是否存在
		uint find(const vector <trie_char> char_list);              // 查找
		bool push(const vector <trie_char> char_list);              // 添加
		bool erase(const vector <trie_char> char_list);             // 删除
		void clear();                                               // 清除
	};

	// AC自动机
	class acAutomaton {
	protected:
		vector <trie_node> _tree;                                   // 数组存字典树
		vector <uint> _fail;                                        // 数组fail指针
		vector <uint> _last_word;                                   // 字典树某点之前的词语
		uint _capacity;                                             // 节点数
		uint _size;                                                 // 单词数
		uint _max_word_length;                                      // 单词最长数，最长为20
		bool _is_build;                                             // fail指针是否构建
	public:
		acAutomaton(uint max_word_length = 10);                     // 构造
		~acAutomaton();                                             // 析造
		uint size();                                                // 词语数量
		uint capacity();                                            // 节点数
		bool is_fail_build();                                       // 构造fail指针
		bool exist(const vector <trie_char> char_list);             // 判断是否存在
		uint find(const vector <trie_char> char_list);              // 查找
		bool push(const vector <trie_char> char_list);              // 添加
		bool erase(const vector <trie_char> char_list);             // 删除
		void clear();                                               // 清除
		void build_fail();                                          // 构造fail指针

		vector <pair_uint> search(const vector <trie_char> char_list, bool flag = false);   // 特殊查找
		vector <pair_uint> search_all(const vector <trie_char> char_list);                  // 全查找
		vector <pair_uint> search_quick(const vector <trie_char> char_list);                // 快速查找
		uint substring(const vector <trie_char> char_list, uint start_node = 0);            // 子串查找
		uint last_word(uint pos);                                                           // 获得某点_last_word的值

//		void show();                                                                        // 展示函数
	};

}



#endif /* CS_TRIE_H_ */
