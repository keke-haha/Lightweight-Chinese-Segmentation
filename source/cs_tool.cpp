/*
 * tool.cpp
 *
 *  Created on: 2019年5月11日
 *      Author: 一步之遥
 */

#include "cs_define.h"
#include "cs_tool.h"

namespace cs {

	vector <_char> sentence_to_char(const _sentence & sentence) {
		vector <_char> ret;
		if (sentence.size() == 0 || sentence == "") return ret;

		int pos = 0, len = sentence.size(), unicode_len;
		_char now_ch, tmp_val;
		while (pos < len) {
			now_ch = sentence[pos] & 0xFF;

			if (now_ch >= 240)
				unicode_len = 4;
			else if (now_ch >= 224)
				unicode_len = 3;
			else if (now_ch >= 192)
				unicode_len = 2;
			else
				unicode_len = 1;

			if (unicode_len == 1)
				tmp_val = now_ch;
			else if (unicode_len == 2)
				tmp_val = ((now_ch & 0x1F) << 6) + (sentence[pos + 1] & 0x3F);
			else if (unicode_len == 3)
				tmp_val = ((now_ch & 0x0F) << 12) + ((sentence[pos + 1] & 0x3F) << 6) + (sentence[pos + 2] & 0x3F);
			else
				tmp_val = ((now_ch & 0x07) << 18) + ((sentence[pos + 1] & 0x3F) << 12) + ((sentence[pos + 2] & 0x3F) << 6) + (sentence[pos + 3] & 0x3F);

			ret.push_back(tmp_val);
			pos += unicode_len;
		}
		return ret;
	}

	vector <_char> word_to_char(const _word & word) {
		vector <_char> ret;
		if (word.size() == 0 || word == "") return ret;

		int pos = 0, len = word.size(), unicode_len;
		_char now_char, tmp_val;
		while (pos < len) {
			now_char = word[pos] & 0xFF;

			if (now_char >= 240)
				unicode_len = 4;
			else if (now_char >= 224)
				unicode_len = 3;
			else if (now_char >= 192)
				unicode_len = 2;
			else
				unicode_len = 1;

			if (unicode_len == 1)
				tmp_val = now_char;
			else if (unicode_len == 2)
				tmp_val = ((now_char & 0x1F) << 6) + (word[pos + 1] & 0x3F);
			else if (unicode_len == 3)
				tmp_val = ((now_char & 0x0F) << 12) + ((word[pos + 1] & 0x3F) << 6) + (word[pos + 2] & 0x3F);
			else
				tmp_val = ((now_char & 0x07) << 18) + ((word[pos + 1] & 0x3F) << 12) + ((word[pos + 2] & 0x3F) << 6) + (word[pos + 3] & 0x3F);

			ret.push_back(tmp_val);
			pos += unicode_len;
		}
		return ret;
	}

	_sentence char_to_sentence(const vector <_char> & char_list) {
		_sentence ret;
		uchar ch;
		for (_char val : char_list) {
			if (val <= utf8_one_bit) {
				ch = val;
				ret += ch;
			}
			else if (val <= utf8_two_bit) {
				ch = or_val | (0xC0) | (val / 64);
				ret += ch;
				ch = or_val | (0x80) | (val % 64);
				ret += ch;
			}
			else if (val <= utf8_three_bit) {
				ch = or_val | (0xE0) | (val / 4096);
				val %= 4096;
				ret += ch;
				ch = or_val | (0x80) | (val / 64);
				ret += ch;
				ch = or_val | (0x80) | (val % 64);
				ret += ch;
			}
			else {
				ch = or_val | (0xF0) | (val / 262144);
				val %= 262144;
				ret += ch;
				ch = or_val | (0x80) | (val / 4096);
				val %= 4096;
				ret += ch;
				ch = or_val | (0x80) | (val / 64);
				ret += ch;
				ch = or_val | (0x80) | (val % 64);
				ret += ch;
			}
		}
		return ret;
	}

	_word char_to_word(const vector <_char> & char_list) {
		_word ret;
		uchar ch;
		for (_char val : char_list) {
			if (val <= utf8_one_bit) {
				ch = val;
				ret += ch;
			}
			else if (val <= utf8_two_bit) {
				ch = or_val | (0xC0) | (val / 64);
				ret += ch;
				ch = or_val | (0x80) | (val % 64);
				ret += ch;
			}
			else if (val <= utf8_three_bit) {
				ch = or_val | (0xE0) | (val / 4096);
				val %= 4096;
				ret += ch;
				ch = or_val | (0x80) | (val / 64);
				ret += ch;
				ch = or_val | (0x80) | (val % 64);
				ret += ch;
			}
			else {
				ch = or_val | (0xF0) | (val / 262144);
				val %= 262144;
				ret += ch;
				ch = or_val | (0x80) | (val / 4096);
				val %= 4096;
				ret += ch;
				ch = or_val | (0x80) | (val / 64);
				ret += ch;
				ch = or_val | (0x80) | (val % 64);
				ret += ch;
			}
		}
		return ret;
	}

	_char string_to_char(const string & str) {
		_char ret = 0;
		if (str.size() == 1)
			ret = str[0];
		else if (str.size() == 2)
			ret = (str[0] & 0x1F) * 64 + (str[1] & 0x3F);
		else if (str.size() == 3)
			ret = ((str[0] & 0x0F) << 12) + ((str[1] & 0x3F) << 6) + (str[2] & 0x3F);
		else
			ret = ((str[0] & 0x07) << 18) + ((str[1] & 0x3F) << 12) + ((str[2] & 0x3F) << 6) + (str[3] & 0x3F);
		return ret;
	}

	_word string_to_word(const string & str) {
		return str;
	}

	_sentence string_to_sentence(const string & str) {
		return str;
	}

	string char_to_string(const _char & ch) {
		string ret;
		_char val = ch;
		uchar uch;
		if (val <= utf8_one_bit) {
			uch = val;
			ret += uch;
		}
		else if (ch <= utf8_two_bit) {
			uch = or_val | (0xC0) | (val / 64);
			ret += uch;
			uch = or_val | (0x80) | (val % 64);
			ret += uch;
		}
		else if (val <= utf8_three_bit) {
			uch = or_val | (0xE0) | (val / 4096);
			val %= 4096;
			ret += uch;
			uch = or_val | (0x80) | (val / 64);
			ret += uch;
			uch = or_val | (0x80) | (val % 64);
			ret += uch;
		}
		else {
			uch = or_val | (0xF0) | (val / 262144);
			val %= 262144;
			ret += uch;
			uch = or_val | (0x80) | (val / 4096);
			val %= 4096;
			ret += uch;
			uch = or_val | (0x80) | (val / 64);
			ret += uch;
			uch = or_val | (0x80) | (val % 64);
			ret += uch;
		}
		return ret;
	}

	string word_to_string(const _word & word) {
		return word;
	}

	string sentence_to_string(const _sentence & sentence) {
		return sentence;
	}

	uint sentence_char_num(const _sentence & sentence) {
		uint ret = 0;
		if (sentence.size() == 0 || sentence == "") return ret;
		int pos = 0, len = sentence.size(), unicode_len;
		_char now_ch;
		while(pos < len) {
			now_ch = sentence[pos] & 0xFF;

			if (now_ch >= 240)
				unicode_len = 4;
			else if (now_ch >= 224)
				unicode_len = 3;
			else if (now_ch >= 192)
				unicode_len = 2;
			else
				unicode_len = 1;

			ret ++;
			pos += unicode_len;
		}
		return ret;
	}

	uint word_char_num(const _word & word) {
		uint ret = 0;
		if (word.empty() || word == "") return ret;
		int pos = 0, len = word.size(), unicode_len;
		_char now_ch;
		while(pos < len) {
			now_ch = word[pos] & 0xFF;

			if (now_ch >= 240)
				unicode_len = 4;
			else if (now_ch >= 224)
				unicode_len = 3;
			else if (now_ch >= 192)
				unicode_len = 2;
			else
				unicode_len = 1;

			ret ++;
			pos += unicode_len;
		}
		return ret;
	}


	vector <string> split_string(const string & str, const char& ch) {
		vector <string> ret;
		if (str.size() == 0 || str == "") return ret;
		int flag = -1;
		char cr = 13;    /// 回车键
		for (int i = 0, len = str.size(); i < len; i++) {
			if (str[i] == ch || str[i] == cr) {
				if (~flag)
					ret.push_back(str.substr(flag, i - flag));
				flag = -1;
			}
			else if (flag == -1)
				flag = i;
		}
		if (~flag)
			ret.push_back(str.substr(flag, str.size() - flag));
		return ret;
	}

	string remove_string(const string & str, const char& ch) {
		string ret;
		if (str.size() == 0 || str == "") return ret;

		const char* now_str = str.c_str();
		for (int i = 0; now_str[i] != '\0'; i++)
			if (now_str[i] != ch)
				ret += now_str[i];
		return ret;
	}

	double add(double _value1, double _value2) {
		if (_value1 == inf_min || _value2 == inf_min)
			return inf_min;
		else
			return _value1 + _value2;
	}

	double sup(double _value1, double _value2) {
		if (_value1 == inf_min || _value2 == inf_min)
			return inf_min;
		else
			return _value1 - _value2;
	}

	charState int_to_charState(int val) {
		val %= 4;
		if (val == 0)
			return _begin;
		else if (val == 1)
			return _middle;
		else if (val == 2)
			return _end;
		else
			return _single;
	}

	charState uint_to_charState(uint val) {
		val %= 4;
		if (val == 0)
			return _begin;
		else if (val == 1)
			return _middle;
		else if (val == 2)
			return _end;
		else
			return _single;
	}

	int charState_to_int(const charState& val) {
		if (val == _begin)
			return 0;
		else if (val == _middle)
			return 1;
		else if (val == _end)
			return 2;
		else
			return 3;
	}

	uint charState_to_uint(const charState& val) {
		if (val == _begin)
			return 0;
		else if (val == _middle)
			return 1;
		else if (val == _end)
			return 2;
		else
			return 3;
	}

	vector <int> get_fail(const vector <_char>& char_list) {
		int len = char_list.size();
		vector <int> ret;
		ret.push_back(0);
		if (len >= 1) ret.push_back(0);
		
		for (int i = 1, tmp; i < len; i++) {
			tmp = ret[i];
			while (tmp && char_list[i] != char_list[tmp])
				tmp = ret[tmp];
			ret.push_back(char_list[i] == char_list[tmp] ? tmp + 1 : 0);
		}
		return ret;
	}

	vector <pair_uint> find_word(const _word word, const vector <_sentence> sentence_list) {
		vector <_char> word_char = word_to_char(word);
		vector <int> fail = get_fail(word_char);
		vector <pair_uint> ret;
		for (int i = 0, fail_pos = 0, sentence_size = sentence_list.size(); i < sentence_size; i++) {
			vector <_char> char_list = sentence_to_char(sentence_list[i]);
			for (int j = 0, char_size = char_list.size(); j < char_size; j++) {
				while (fail_pos && word_char[fail_pos] != char_list[j])
					fail_pos = fail[fail_pos];
				if (word_char[fail_pos] == char_list[j])
					fail_pos++;
				if (fail_pos == (int)fail.size()) {
					fail_pos = 0;
					ret.push_back(pair<uint, uint>(i + 1, j + 1));
				}
			}
		}
		return ret;
	}

	uint show_tool_1(uint molecule, uint denominator, uint sum) {
		if(molecule == denominator) return sum;
		uint ret = 1ull * sum * molecule / denominator;
		return ret;
	}

}



