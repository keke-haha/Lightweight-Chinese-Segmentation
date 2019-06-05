/*
 * hmm.cpp
 *
 *  Created on: 2019年5月13日
 *      Author: 一步之遥
 */

#include <cstring>
#include <stack>
#include <cmath>
#include <fstream>
#include "cs_hmm.h"
#include "cs_tool.h"

namespace cs {

	hmmSegmentation::hmmSegmentation() {
		memset(_state_amout, 0, sizeof(_state_amout));
		memset(_init_state_amout, 0, sizeof(_init_state_amout));
		memset(_trans_state_amout, 0, sizeof(_trans_state_amout));
		memset(_state_char_amount, 0, sizeof(_state_char_amount));
		for (int i = 0; i < 4; i++) {
			_init_state_pro[i] = inf_min;
			for (int j = 0; j < 4; j++)
				_trans_state_pro[i][j] = inf_min;
		}
		_char_amout = 0;
		_is_trained = false;
	}

	hmmSegmentation::~hmmSegmentation() {

	}

	void hmmSegmentation::clear() {
		memset(_state_amout, 0, sizeof(_state_amout));
		memset(_init_state_amout, 0, sizeof(_init_state_amout));
		memset(_trans_state_amout, 0, sizeof(_trans_state_amout));
		memset(_state_char_amount, 0, sizeof(_state_char_amount));
		for (int i = 0; i < 4; i++) {
			_init_state_pro[i] = inf_min;
			for (int j = 0; j < 4; j++)
				_trans_state_pro[i][j] = inf_min;
		}
		_char_amout = 0;
		_is_trained = false;
	}

	void hmmSegmentation::train() {
		ull _amout = 0;
		for (int i = 0; i < 4; i++)
			_amout += _init_state_amout[i];
		for(int i = 0; i < 4; i ++) {
			if (_init_state_amout[i] == 0)
				_init_state_pro[i] = inf_min;
			else
				_init_state_pro[i] = std::log(_init_state_amout[i]) - std::log(_amout);
		}

		for (int i = 0; i < 4; i++) {
			_amout = 0;
			for (int j = 0; j < 4; j++)
				_amout += _trans_state_amout[i][j];
			for (int j = 0; j < 4; j++) {
				if (_trans_state_amout[i][j] == 0)
					_trans_state_pro[i][j] = inf_min;
				else
					_trans_state_pro[i][j] = std::log(_trans_state_amout[i][j]) - std::log(_amout);
			}
		}

		_is_trained = true;
	}

	double hmmSegmentation::get_state_char_pro(int flag, _char now_char) {
		uint _amout = _state_char_amount[flag][now_char];
		if (_amout == 0)
			return inf_min;
		else
			return std::log(_amout) - std::log(_state_amout[flag]);
	}

	vector<_word> hmmSegmentation::cut_sentence(const _sentence & sen) {
		vector <_char> char_list = sentence_to_char(sen);
		vector<_word> ret;
		_word tmp_word;
		if(char_list.empty())
			return ret;
		vector <charState> state_list = viterbi(char_list);
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

	vector<charState> hmmSegmentation::viterbi(const vector<_char>& char_list) {
		vector <charState> ret;
		vector <int> last_path[4];
		vector <double> now_pro[4];
		int len = char_list.size(), last_point;
		double rcd_val, tmp_val, char_pro;
		_char now_char;

		for (int i = 0; i < len; i++) {
			now_char = char_list[i];
			if (i == 0) {
				for (int j = 0; j < 4; j++) {
					tmp_val = add(_init_state_pro[j], get_state_char_pro(j, now_char));
					now_pro[j].push_back(tmp_val);
					last_path[j].push_back(-1);
				}
			}
			else {
				for (int q = 0; q < 4; q++) {
					char_pro = get_state_char_pro(q, now_char);
					rcd_val = inf_min;
					last_point = 3;
					for (int p = 0; p < 4; p++) {
						tmp_val = add(now_pro[p][i-1], add(_trans_state_pro[p][q], char_pro));
						if (rcd_val < tmp_val) {
							rcd_val = tmp_val;
							last_point = p;
						}
					}
					now_pro[q].push_back(rcd_val);
					last_path[q].push_back(last_point);
				}
			}
		}

		int pos = len - 1, flag = 0;
		for (int i = 0; i < 4; i++)
			if (now_pro[flag][pos] < now_pro[i][pos])
				flag = i;
		
		std::stack<charState> s;
		while (pos >= 0) {
			switch (flag) {
				case 0: s.push(_begin); break;
				case 1: s.push(_middle); break;
				case 2: s.push(_end); break;
				case 3: s.push(_single); break;
				default: s.push(_single); break;
			}
			flag = last_path[flag][pos--];
		}

		while (!s.empty()) {
			ret.push_back(s.top());
			s.pop();
		}

		return ret;
	}

	void hmmSegmentation::show() {
		using std::cout;
		using std::endl;
		cout << "字符出现总次数:" << _char_amout << endl;
		cout << "Begin 状态出现的次数:" << _state_amout[_begin] << endl;
		cout << "Middle 状态出现的次数:" << _state_amout[_middle] << endl;
		cout << "End 状态出现的次数:" << _state_amout[_end] << endl;
		cout << "Single 状态出现的次数:" << _state_amout[_single] << endl;
		cout << "句头 Begin 状态出现的次数:" << _init_state_amout[_begin] << endl;
		cout << "句头 Middle 状态出现的次数:" << _init_state_amout[_middle] << endl;
		cout << "句头 End 状态出现的次数:" << _init_state_amout[_end] << endl;
		cout << "句头 Single 状态出现的次数:" << _init_state_amout[_single] << endl;

		for(int i = 0; i < 4; i ++) {
			for(int j = 0; j < 4; j ++) {
				if(i == 0) cout << "Begin";
				else if(i == 1) cout << "Middle";
				else if(i == 2) cout << "End";
				else if(i == 3) cout << "Single";

				cout << " 转化成 ";

				if(j == 0) cout << "Begin";
				else if(j == 1) cout << "Middle";
				else if(j == 2) cout << "End";
				else if(j == 3) cout << "Single";

				cout << " 的次数:" << _trans_state_amout[i][j] << endl;
			}
		}

		for(int i = 0; i < 4; i ++) {
			cout << "句头转态为 ";
			if(i == 0) cout << "Begin";
			else if(i == 1) cout << "Middle";
			else if(i == 2) cout << "End";
			else if(i == 3) cout << "Single";
			cout << " 的对数概率:" << _init_state_pro[i] << endl;
		}
		cout << "\tB\tM\tE\tS" << endl;
		for(int i = 0; i < 4; i ++) {
			if(i == 0) cout << "B";
			else if(i == 1) cout << "M";
			else if(i == 2) cout << "E";
			else if(i == 3) cout << "S";
			for(int j = 0; j < 4; j ++) {
				cout << "\t";
				cout << _trans_state_pro[i][j];
			}
			cout << endl;
		}
	}

	bool hmmSegmentation::read_model(const string & read_fp) {
		std::ifstream file_in(read_fp);
		if(!file_in.is_open()) {
			std::cerr << read_fp << " read error!!!" << std::endl;
			return false;
		}

		memset(_state_char_amount, 0, sizeof(_state_char_amount));

		string str;
		file_in >> str;
		if(str == "Initialization:") {
			for(int i = 0; i < 4; i ++) {
				file_in >> _init_state_amout[i];
				_char_amout += _init_state_amout[i];
			}
		} else {
			std::cerr << read_fp << " read error!!!" << std::endl;
			return false;
		}

		file_in >> str;
		if(str == "Transformation:") {
			for(int i = 0; i < 4; i ++) {
				for(int j = 0; j < 4; j ++) {
					file_in >> _trans_state_amout[i][j];
					_char_amout += _trans_state_amout[i][j];
				}
			}
		} else {
			std::cerr << read_fp << " read error!!!" << std::endl;
			return false;
		}

		file_in >> str;
		if(str == "Emission:") {
			for(int i = 0; i < 4; i ++) {

				uint num = 0, val = 0;
				file_in >> str >> num;

				uint rcd = _state_amout[i];

				for(uint j = 0; j < num; j ++) {
					file_in >> str >> val;
					_state_char_amount[i][string_to_char(str)] = val;
					_state_amout[i] += val;
					if(rcd > _state_amout[i]) {
						std::cout << str << " " << val << std::endl;
					}
				}
			}
		} else {
			std::cerr << read_fp << " read error!!!" << std::endl;
			return false;
		}

		_is_trained = false;
		return true;
	}

}
