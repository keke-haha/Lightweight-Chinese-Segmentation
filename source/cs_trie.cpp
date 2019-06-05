/*
 * cs_trie.cpp
 *
 *  Created on: 2019年5月12日
 *      Author: 一步之遥
 */
#include <iostream>
#include <queue>
#include <algorithm>

#include "cs_trie.h"
#include "cs_tool.h"

namespace cs {

	/*-------------------------------------------------------------------------------------------------*/
	/*------------------------------------------- tire字典树 -------------------------------------------*/
	/*-------------------------------------------------------------------------------------------------*/

	trie::trie(uint max_word_length) {
		if(max_word_length > 20)
			max_word_length = 20;
		_tree.reserve(65536);
		_tree.push_back(trie_node());
		_capacity = 1;
		_size = 0;
		_max_word_length = max_word_length;
	}

	trie::~trie() {
		_tree.clear();
	}

	uint trie::size() {
		return _size;
	}

	uint trie::capacity() {
		return _capacity;
	}

	bool trie::exist(const vector <trie_char> char_list) {
		return find(char_list) != 0;
	}

	uint trie::find(const vector <trie_char> char_list) {
		if(char_list.size() > _max_word_length)
			return 0;
		uint now_node = 0;
		for (trie_char now_char : char_list) {
			uint nex_node = _tree[now_node].find(now_char);
			if (nex_node)
				now_node = nex_node;
			else
				break;
		}
		if (_tree[now_node]._exist && _tree[now_node]._dep == (uint)char_list.size())
			return now_node;
		else
			return 0;
	}

	bool trie::push(const vector <trie_char> char_list) {
		if(char_list.size() > _max_word_length)
			return false;
		uint now_node = 0;
		for (trie_char now_char : char_list) {
			uint nex_node = _tree[now_node].find(now_char);
			if (!nex_node) {
				_tree.push_back(trie_node(false, _tree[now_node]._dep + 1));
				_tree[now_node].push(now_char, _capacity);
				nex_node = _capacity ++;
			}
			now_node = nex_node;
		}
		if (_tree[now_node]._exist)
			return false;
		else {
			_tree[now_node]._exist = true;
			_size++;
			return true;
		}
	}

	bool trie::erase(const vector <trie_char> char_list) {
		if(char_list.size() > _max_word_length)
			return false;
		uint now_node = find(char_list);
		if (!now_node)
			return false;
		else {
			_tree[now_node]._exist = false;
			return true;
		}
	}

	void trie::clear() {
		_tree.clear();
		_size = 0;
		_capacity = 1;
		_tree.push_back(trie_node());
	}

	/*-------------------------------------------------------------------------------------------------*/
	/*-------------------------------------------- AC自动机 --------------------------------------------*/
	/*-------------------------------------------------------------------------------------------------*/

	using std::queue;

	acAutomaton::acAutomaton(uint max_word_length) {
		if(max_word_length > 20)
			max_word_length = 20;
		_tree.reserve(65536);
		_tree.push_back(trie_node());
		_capacity = 1;
		_size = 0;
		_is_build = false;
		_max_word_length = max_word_length;
	}

	acAutomaton::~acAutomaton() {
		_tree.clear();
		_fail.clear();
	}

	uint acAutomaton::size() {
		return _size;
	}

	uint acAutomaton::capacity() {
		return _capacity;
	}

	bool acAutomaton::is_fail_build() {
		return _is_build;
	}

	bool acAutomaton::exist(const vector <trie_char> char_list) {
		return find(char_list) != 0;
	}

	uint acAutomaton::find(const vector <trie_char> char_list) {
		if(char_list.size() > _max_word_length || char_list.empty())
			return 0;
		uint now_node = 0, flag = 1;
		for (trie_char now_char : char_list) {
			uint nex_node = _tree[now_node].find(now_char);
			if (nex_node)
				now_node = nex_node;
			else {
				flag = 0;
				break;
			}
		}
		if (_tree[now_node]._exist && flag)
			return now_node;
		else
			return 0;
	}

	bool acAutomaton::push(const vector <trie_char> char_list) {
		if(char_list.size() > _max_word_length)
			return false;
		uint now_node = 0;
		for (trie_char now_char : char_list) {
			uint nex_node = _tree[now_node].find(now_char);
			if (nex_node == 0) {
				_tree.push_back(trie_node(false, _tree[now_node]._dep + 1));
				_tree[now_node].push(now_char, _capacity);
				nex_node = _capacity ++;
			}
			now_node = nex_node;
		}
		if (_tree[now_node]._exist)
			return false;
		else {
			_tree[now_node]._exist = true;
			_size++;
			return true;
		}
		_is_build = false;
	}

	bool acAutomaton::erase(const vector <trie_char> char_list) {
		if(char_list.size() > _max_word_length)
			return false;
		uint now_node = find(char_list);
		if (!now_node)
			return false;
		else {
			_tree[now_node]._exist = false;
			return true;
		}
	}

	void acAutomaton::clear() {
		_tree.clear();
		_fail.clear();
		_tree.push_back(trie_node());
		_capacity = 1;
		_size = 0;
		_is_build = false;
	}

	void acAutomaton::build_fail() {
		uint _root = 0, now_pos, fail_pos, find_pos;
		_fail.clear();
		_last_word.clear();
		_fail.resize(_capacity, _root);
		_last_word.resize(_capacity, _root);
		trie_node& now_node = _tree[_root];

		queue <uint> node_que;
		vector <pair_uint>::iterator it_now, it_end;
		it_now = now_node._nex.begin();
		it_end = now_node._nex.end();
		while (it_now != it_end) {
			node_que.push(it_now->second);
			it_now ++;
		}

		while (!node_que.empty()) {
			now_pos = node_que.front();
			node_que.pop();
			it_now = _tree[now_pos]._nex.begin();
			it_end = _tree[now_pos]._nex.end();
			if(_tree[now_pos]._exist)
				_last_word[now_pos] = _tree[now_pos]._dep;
			while (it_now != it_end) {
				_last_word[it_now->second] = _last_word[now_pos];
				node_que.push(it_now->second);
				fail_pos = now_pos;
				while (fail_pos != _root) {
					fail_pos = _fail[fail_pos];
					find_pos = _tree[fail_pos].find(it_now->first);
					if (find_pos) {
						_fail[it_now->second] = find_pos;
						break;
					}
				}
				it_now ++;
			}
		}
		_is_build = true;
	}

	vector <pair_uint> acAutomaton::search(const vector <trie_char> char_list, bool flag) {
		if (!_is_build) {
			vector <pair_uint> ret;
			return ret;
		}

		if (!flag)
			return search_quick(char_list);
		else
			return search_all(char_list);
	}

	vector <pair_uint> acAutomaton::search_quick(const vector <trie_char> char_list) {
		vector <pair_uint> ret;
		uint now_pos = 0, tmp_pos = 0, list_pos = 0, len = 0, st, ed;
		for(trie_char tmp_char : char_list) {
			if(_tree[now_pos].find(tmp_char)) tmp_pos = _tree[now_pos].find(tmp_char);
			else {
				while(now_pos && (_tree[now_pos]._dep > len || !_tree[now_pos].find(tmp_char)))
					now_pos = _fail[now_pos];
				tmp_pos = _tree[now_pos].find(tmp_char);
			}
			if(tmp_pos)
				now_pos = tmp_pos;
			if(_last_word[now_pos]) {
				st = list_pos + 1 - _tree[now_pos]._dep, ed = st + _last_word[now_pos] - 1;
				if(ret.empty()) {
					if(st) ret.push_back(pair_uint(0, st - 1));
					ret.push_back(pair_uint(st, ed));
				} else {
					pair_uint tmp_pair = (*(ret.rbegin()));
					if(st <= tmp_pair.first && ed > tmp_pair.second) {
						tmp_pair.first = st;
						tmp_pair.second = ed;
						(*(ret.rbegin())) = tmp_pair;
					} else if(tmp_pair.second < st){
						if(tmp_pair.second + 1 < st)
							ret.push_back(pair_uint(tmp_pair.second + 1, st - 1));
						ret.push_back(pair_uint(st, ed));
					}
				}
			}
			len = _tree[now_pos]._dep - _last_word[now_pos];
			list_pos ++;
		}
		if(len) ret.push_back(pair_uint(list_pos - len, list_pos - 1));
		return ret;
	}

	vector <pair_uint> acAutomaton::search_all(const vector <trie_char> char_list) {
		vector <pair_uint> ret;
		uint list_len = char_list.size();
		for(uint i = 0; i < list_len; i ++) {
			uint nex_pos, now_pos = 0, len = 0, pos = i;
			while(true) {
				nex_pos = _tree[now_pos].find(char_list[pos]);
				if(!nex_pos)
					break;
				now_pos = nex_pos;
				len ++;
				if(_tree[now_pos]._exist)
					ret.push_back(pair_uint(i, pos));
				if(len == _max_word_length || ++ pos >= list_len)
					break;
			}
		}
		return ret;
	}

	uint acAutomaton::substring(const vector <trie_char> char_list, uint start_node) {
		if(char_list.empty())
			return 0;
		uint now_node = start_node, flag = 1;
		for (trie_char now_char : char_list) {
			uint nex_node = _tree[now_node].find(now_char);
			if (nex_node)
				now_node = nex_node;
			else {
				flag = 0;
				break;
			}
		}
		return flag ? now_node: 0;
	}

	uint acAutomaton::last_word(uint pos) {
		if(!_is_build) return 0;
		else return _last_word[pos];
	}

//	void acAutomaton::show() {
//		std::cout << _capacity << std::endl;
//		for(int i = 0; i < _capacity; i ++) {
//			std::cout << "---------------------分割线---------------------" << std::endl;
//			std::cout << "第" << i << "个点的信息如下:" << std::endl;
//			_tree[i].show();
//			std::cout << "---------------------分割线---------------------" << std::endl;
//		}
//		if(!_is_build) return;
//		for(int i = 0; i < _capacity; i ++) {
//			std::cout << "第" << i << "个节点的fail指向: " << _fail[i] << " 和lastWord值 :" << _last_word[i] << std::endl;
//		}
//		std::cout << "---------------------分割线---------------------" << std::endl;
//	}

}


