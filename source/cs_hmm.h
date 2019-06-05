#pragma once
/*
 * hmm.h
 *
 *  Created on: 2019年5月13日
 *      Author: 一步之遥
 */

#ifndef CS_HMM_H_
#define CS_HMM_H_

#include <vector>
#include <string>
#include <map>

#include "cs_define.h"

namespace cs {

	using std::map;
	using std::vector;
	using std::string;

	class hmmSegmentation {
	protected:
		uint _state_char_amount[5][65555];                                        // BMES四种状态中每种字符出现的次数
		ull _state_amout[5];                                                      // BMES四种状态出现的次数
		ull _init_state_amout[5];                                                 // BMES四种状态出现在句头的次数
		double _init_state_pro[5];                                                // BMES四种状态出现在句头的概率(取对数)
		ull _trans_state_amout[5][5];                                             // BMES四种状态相互转化的次数
		double _trans_state_pro[5][5];                                            // BMES四种状态相互转化的概率(取对数)
		ull _char_amout;                                                          // 所有字符出现的次数
		bool _is_trained;                                                         // 是否已训练
	public:
		hmmSegmentation();                                                        // 构造函数
		~hmmSegmentation();                                                       // 析构函数
		void clear();                                                             // 清除数据
		void train();                                                             // 将训练获得的统计信息转化成概率
		double get_state_char_pro(int flag, _char now_char);                      // 获得在每种状态下发射某字的概率
		vector <_word> cut_sentence(const _sentence & sen);                       // 切词
		vector <charState> viterbi(const vector <_char>& char_list);              // 维特比算法
		void show();                                                              // 展示函数，用于测试
		bool read_model(const string & read_fp);                                  // 模型读取
	};

}


#endif /* CS_HMM_H_ */
