/*
 * ngram.cpp
 *
 *  Created on: 2019年5月15日
 *      Author: 一步之遥
 */

#include <cstdio>
#include <fstream>
#include <cstring>
#include <cmath>

#include "cs_ngram.h"
#include "cs_tool.h"

namespace cs {
	triGram::triGram() {

		std::memset(_legal_trans, false, sizeof(_legal_trans));
		std::memset(_pr, 0.0, sizeof(_pr));
		std::memset(_low_freq, 0, sizeof(_low_freq));

		_legal_trans[_root][_root][_single] = _legal_trans[_root][_root][_begin] = true;
		_legal_trans[_root][_begin][_middle] = _legal_trans[_root][_begin][_end] = true;
		_legal_trans[_root][_single][_begin] = _legal_trans[_root][_single][_single] = true;

		_legal_trans[_begin][_middle][_middle] = _legal_trans[_begin][_middle][_end] = true;
		_legal_trans[_begin][_end][_begin] = _legal_trans[_begin][_end][_single] = true;

		_legal_trans[_middle][_middle][_middle] = _legal_trans[_middle][_middle][_end] = true;
		_legal_trans[_middle][_end][_begin] = _legal_trans[_middle][_end][_single] = true;

		_legal_trans[_end][_begin][_middle] = _legal_trans[_end][_begin][_end] = true;
		_legal_trans[_end][_single][_begin] = _legal_trans[_end][_single][_single] = true;

		_legal_trans[_single][_begin][_middle] = _legal_trans[_single][_begin][_end] = true;
		_legal_trans[_single][_single][_begin] = _legal_trans[_single][_single][_single] = true;


		_total_sum = 0;
		_min_tri_gram_pos = 0;
		_is_trained = false;
		_is_smooth = false;
		_par1 = _par2 = _par3 = 0;
		_threshold = 5;

		/**
		  *  根据一亿字的中文语料统计大概可知
		  * ‘BMES’四种状态的不同字符数大概为6000个
		  *  但是‘BMES’的状态只有 22 种 合法的状态转移，
		  *  因此三元组的总可能数大概为 (6000^3)*22 = 4752000000000
		 */
		_low_freq[0] = 4752000000000ull;
	}

	triGram::~triGram() {
		_state.clear();
		_freq.clear();
	}


	/// 使用了插值平滑法，解决数据稀疏问题，是的数据曲线更平滑
	void triGram::train() {

		if(!_is_smooth) 
			good_turing_smooth();

		if(_is_trained)
			return;

		/// 插值平滑法
		ull tmp_par1 = 0;
		ull tmp_par2 = 0;
		ull tmp_par3 = 0;
		uint now_pos, end_pos;

		/// 因为vecotr里的key_value是有序的，那么从值最小的三元组的下标开始
		now_pos = _min_tri_gram_pos;
		end_pos = _state.size();


		ull tri_val = (1ull << 60) - 1, bi_val = (1ull << 40) - 1, uni_val = (1ull << 20) - 1;

		uint all_sum = end_pos - now_pos;
		uint num = 0, p = show_tool_1(1, 20, all_sum), q = 1;

		printf("开始设置tri-gram模型参数：\n");
		printf("tri-gram模型参数设置工作进度：\n");

		while(now_pos < end_pos) {
			ull tmp_val = _state[now_pos];
			double c3 = div(find(tmp_val & tri_val) - 1, find((tmp_val >> 20) & bi_val) - 1);
			double c2 = div(find(tmp_val & bi_val) - 1, find((tmp_val >> 20) & uni_val) - 1);
			double c1 = div(find(tmp_val & uni_val) - 1, _total_sum - 1);
			if (c3 >= c1 && c3 >= c2)
				tmp_par3 += _freq[now_pos];
			else if(c2 >= c3 && c2 >= c1)
				tmp_par2 += _freq[now_pos];
			else if(c1 >= c3 && c1 > c2)
				tmp_par1 += _freq[now_pos];
			now_pos ++;
			if(++ num == p) {
				for(int i = 1; i <= 20; i ++) {
					if(i <= q) printf("#");
					else printf(" ");
				}
				std::cout << "\t" << (num * 100.0 / all_sum) << "%" << std::endl;
				p = show_tool_1(++ q, 20, all_sum);
			}
		}
		_par1 = div(tmp_par1, tmp_par1 + tmp_par2 + tmp_par3);
		_par2 = div(tmp_par2, tmp_par1 + tmp_par2 + tmp_par3);
		_par3 = div(tmp_par3, tmp_par1 + tmp_par2 + tmp_par3);

		_is_trained = true;

		printf("tri-gram模型参数设置完成！\n");

//		double k1 = _par1, k2 = _par2, k3 = _par3;
//		for(int i = 1; i <= 15; i ++) {
//			std::cout << k1 << std::endl;
//			k1 *= 10;
//			while(k1 >= 1) k1 -= 1;
//		}
//		std::cout << "------------------------" << std::endl;
//		for(int i = 1; i <= 15; i ++) {
//			std::cout << k2 << std::endl;
//			k2 *= 10;
//			while(k2 >= 1) k2 -= 1;
//		}
//		std::cout << "------------------------" << std::endl;
//		for(int i = 1; i <= 15; i ++) {
//			std::cout << k3 << std::endl;
//			k3 *= 10;
//			while(k3 >= 1) k3 -= 1;
//		}
//		std::cout << "------------------------" << std::endl;

	}

	void triGram::good_turing_smooth() {
		if(_is_smooth)
			return;
		/// Good-turing平滑法
		ull now_sum = 0;
		double one = 1.0;
		double r[15];   /// 代表 Good-turing 中的 r*
		for(uint i = 0; i <= _threshold; i ++)
			now_sum += 1ull * _low_freq[i] * i;

		for(uint i = 0; i <= _threshold; i ++) {
			if(i < _threshold)
				r[i] = (i + 1) * _low_freq[i + 1] * one / _low_freq[i];
			else
				r[i] = one * i;
		}

		/// 进行归一化
		double normalized = 0;
		for (uint i = 0; i <= _threshold; i++)
			normalized += _low_freq[i] * r[i];
		for (uint i = 0; i <= _threshold; i ++)
			_pr[i] = (r[i] * now_sum) / normalized;

		_is_smooth = true;
	}

	double triGram::log_prob(uint sta1, _char ch1, uint sta2, _char ch2, uint sta3, _char ch3) {
		if(!_legal_trans[sta1][sta2][sta3])
			return inf_min;
		if(sta1 == 4) sta1 = 0, ch1 = 0;
		if(sta2 == 4) sta2 = 0, ch2 = 0;
		ull now_state = (1ull << 18) * sta1 + ch1;
		now_state = (now_state << 20) + (1ull << 18) * sta2 + ch2;
		now_state = (now_state << 20) + (1ull << 18) * sta3 + ch3;
		ull bi_val = (1ull << 40) - 1, uni_val = (1ull << 20) - 1;
		double uni = _par1 * find_tri(now_state & uni_val);
		double bi = div(_par2 * find_tri(now_state & bi_val), find_tri((now_state >> 20) & uni_val));
		double tri = div(_par3 * find_tri(now_state), find_tri(now_state >> 20));
		if (uni + bi + tri == 0)
			return inf_min;
		else
			return std::log(uni + bi + tri);
	}

	vector <_word> triGram::cut_sentence(const _sentence& sentence) {
		vector <_char> char_list = sentence_to_char(sentence);
		vector <charState> state_list = tag_sentence(char_list);
		vector <_word> ret;
		_word tmp_word;
		for(int i = 0, s = state_list.size(); i <= s; i ++) {
			if(i == s) {
				if(!tmp_word.empty()) ret.push_back(tmp_word);
				continue;
			}
			tmp_word += char_to_string(char_list[i]);
			if(state_list[i] == _end || state_list[i] == _single) {
				ret.push_back(tmp_word);
				tmp_word.clear();
			}
		}
		return ret;
	}

	vector <charState> triGram::tag_sentence(const vector <_char> char_list) {
		vector <charState> ret;
		if (!_is_trained || char_list.size() == 0)
			return ret;
		else if (char_list.size() == 1) {
			ret.push_back(_single);
			return ret;
		}
		int len = char_list.size();
		ret.resize(len, _single);

		vector <double> trans_pro[4][4];
		vector <int> last_point[4][4];

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				trans_pro[i][j].resize(len, inf_min);
				last_point[i][j].resize(len, 3);
				last_point[i][j][0] = -1;
			}
		}

		for (int i = 0; i < 4; i++) {
			charState i_state = int_to_charState(i);
			double tmp_pro = log_prob(_root, 0, _root, 0, i_state, char_list[0]);
			if (trans_pro[_begin][i_state][0] < tmp_pro)
				trans_pro[_begin][i_state][0] = tmp_pro;
		}

		for (int i = 0; i < 4; i++) {
			charState i_state = int_to_charState(i);
			for (int j = 0; j < 4; j++) {
				charState j_state = int_to_charState(j);
				double tmp_pro = log_prob(_root, 0, i_state, char_list[0], j_state, char_list[1]);
				tmp_pro = add(tmp_pro, trans_pro[_begin][i][0]);
				if (trans_pro[i_state][j_state][1] < tmp_pro)
					trans_pro[i_state][j_state][1] = tmp_pro;
			}
		}
		for (int l = 2; l < len; l++) {
			for (int i = 0; i < 4; i++) {
				charState i_state = int_to_charState(i);
				for (int j = 0; j < 4; j++) {
					charState j_state = int_to_charState(j);
					double rcd_pro = trans_pro[i_state][j_state][l - 1];
					if(rcd_pro == inf_min) continue;
					for (int k = 0; k < 4; k++) {
						charState k_state = int_to_charState(k);
						double tmp_pro = log_prob(i_state, char_list[l - 2], j_state, char_list[l - 1], k_state, char_list[l]);
						tmp_pro = add(tmp_pro, rcd_pro);
						if (trans_pro[j][k][l] < tmp_pro) {
							trans_pro[j][k][l] = tmp_pro;
							last_point[j][k][l] = i;
						}
					}
				}
			}
		}

		int first_state = 0, second_state = 0, pos = len - 1, rcd_tmp;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (trans_pro[i][j][pos] > trans_pro[first_state][second_state][pos]) {
					second_state = j;
					first_state = i;
				}
			}
		}
		while (true) {
			ret[pos] = int_to_charState(second_state);
			if(pos > 0)
				rcd_tmp = last_point[first_state][second_state][pos];
			second_state = first_state;
			first_state = rcd_tmp;

			if (-- pos < 0)
				break;
		}
		return ret;
	}

	double triGram::div(ull _value1, ull _value2) {
		if (_value2 == 0)
			return 0.0;
		else
			return _value1 / (double)_value2;
	}

	double triGram::find_tri(const ull & key) {
		if (find(key) == 0)
			return _pr[0];
		else {
			uint tmp_val = find(key);
			if(tmp_val <= _threshold) return _pr[tmp_val];
			else return tmp_val;
		}
	}

	uint triGram::find(const ull & key) {
		int low_side = 0, high_side = _state.size() - 1;
		int want_pos = -1, mid;
		while(low_side <= high_side) {
			mid = (low_side + high_side) >> 1;
			if(_state[mid] == key) {
				want_pos = mid;
				break;
			}
			else if(_state[mid] < key)
				low_side = mid + 1;
			else
				high_side = mid - 1;
		}
		if(~want_pos) return _freq[want_pos];
		else return 0;
	}

	void triGram::clear() {
		_state.clear();
		_freq.clear();
	}

	void triGram::show() {
		std::cout << "参数一：" << _par1 << std::endl;
		std::cout << "参数二：" << _par2 << std::endl;
		std::cout << "参数三：" << _par3 << std::endl;
		std::cout << "总出现次数：" << _total_sum << std::endl;
		std::cout << "元组数：" << _state.size() << std::endl;
		std::cout << "阈值：" << _threshold << std::endl;
		for(uint i = 0; i <= _threshold; i ++)
			std::cout << "出现" << i << "次的元组的可能的频率 " << _pr[i] << std::endl;

	}

	bool triGram::read_model(const string & fp) {
		std::ifstream file_in(fp.c_str());
		if(!file_in.is_open()) {
			std::cerr << fp << " trigram model read error" << std::endl;
			return false;
		}


		/// 初始化
		std::memset(_pr, 0.0, sizeof(_pr));
		std::memset(_low_freq, 0, sizeof(_low_freq));
		_low_freq[0] = 4752000000000ull;
		_state.clear();
		_freq.clear();
		_threshold = 5;
		_min_tri_gram_pos = 0;

		printf("开始读取tri-gram模型：\n");
		printf("tri-gram模型读取进度：\n");

		file_in >> _total_sum;

		uint all_sum;
		file_in >> all_sum;
		file_in >> _par1 >> _par2 >> _par3;

		uint num = 0, p = show_tool_1(1, 20, all_sum), q = 1;

		ull state;
		ull bi_val = (1ull << 40) - 1;
		uint freq, step = 0;

		while(file_in >> state >> freq) {
			step ++; /// 当前下标

			_state.push_back(state);
			_freq.push_back(freq);

			/// 获得低频元组数
			_low_freq[0] --;
			if(freq <= _threshold)
				_low_freq[freq] ++;

			/// 获得最小的三元组的下标值，方便训练
			if(state > bi_val && !_min_tri_gram_pos)
				_min_tri_gram_pos = step;

			if(++ num == p) {
				for(int i = 1; i <= 20; i ++) {
					if(i <= q) printf("#");
					else printf(" ");
				}
				std::cout << "\t" << (num * 100.0 / all_sum) << "%" << std::endl;
				p = show_tool_1(++ q, 20, all_sum);

			}
		}
		printf("tri-gram模型读取完成！\n");
		file_in.close();
		_is_smooth = false;
		_is_trained = true;
		return true;
	}
}
