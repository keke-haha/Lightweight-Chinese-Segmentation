/*
 * cs_char.cpp
 *
 *  Created on: 2019年5月11日
 *      Author: 一步之遥
 */

#include "cs_char.h"

#include <map>

namespace cs {

	charDictionary::charDictionary() {

	}

	charDictionary::~charDictionary() {
		charInfo.clear();
		tradToSimp.clear();
		simpToTrad.clear();
		fullToHalf.clear();
		halfToFull.clear();
	}

	void charDictionary::add_char_info(const _char& ch, const charFlag& flag) {
		charInfo[ch] = flag;
	}

	void charDictionary::add_trad_simp_relationship(const _char& _ch1, const _char& _ch2, bool flag) {
		if(!flag ) simpToTrad[_ch1] = _ch2;
		else tradToSimp[_ch1] = _ch2;
	}

	void charDictionary::add_full_half_relationship(const _char& full, const _char& half) {
		fullToHalf[full] = half;
		halfToFull[half] = full;
	}

	bool charDictionary::is_legal_char(const _char& ch) {
		return charInfo.find(ch) != charInfo.end();
	}

	bool charDictionary::is_chinese(const _char& ch) {
		if (!is_legal_char(ch)) return false;
		else return charInfo[ch] < 2;
	}

	bool charDictionary::is_simp(const _char& ch) {
		if (!is_legal_char(ch)) return false;
		else return charInfo[ch] == 0;
	}

	bool charDictionary::is_trad(const _char& ch) {
		if (!is_legal_char(ch)) return false;
		else return charInfo[ch] == 1;
	}

	bool charDictionary::is_english(const _char& ch) {
		if (!is_legal_char(ch)) return false;
		else return charInfo[ch] == 2;
	}

	bool charDictionary::is_number(const _char& ch) {
		if (!is_legal_char(ch)) return false;
		else return charInfo[ch] == 3;
	}

	bool charDictionary::is_symbol(const _char& ch) {
		if (!is_legal_char(ch)) return false;
		else return charInfo[ch] == 4;
	}

	bool charDictionary::is_full(const _char& ch) {
		return fullToHalf.find(ch) != fullToHalf.end();
	}

	bool charDictionary::is_half(const _char& ch) {
		return halfToFull.find(ch) != halfToFull.end();
	}

	_char charDictionary::simp_to_trad(const _char& simp) {
		if (simpToTrad.find(simp) == simpToTrad.end()) return simp;
		else return simpToTrad[simp];
	}

	_char charDictionary::trad_to_simp(const _char & trad) {
		if (tradToSimp.find(trad) == tradToSimp.end()) return trad;
		else return tradToSimp[trad];
	}

	_char charDictionary::half_to_full(const _char& half) {
		if(halfToFull.find(half) == halfToFull.end()) return half;
		else return halfToFull[half];
	}

	_char charDictionary::full_to_half(const _char& full) {
		if(fullToHalf.find(full) == fullToHalf.end()) return full;
		else return fullToHalf[full];
	}

}
