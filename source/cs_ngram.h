#pragma once
/*
 * ngram.h
 *
 *  Created on: 2019年5月15日
 *      Author: 一步之遥
 */

#include <map>
#include <vector>

#include "cs_define.h"

namespace cs {

	using std::map;
	using std::vector;

	class triGram {
	private:
		bool _legal_trans[5][5][5];                                                            /// 标注合法的转移
		vector <ull> _state;                                                                   /// 储存3元关系组的状态
		vector <uint> _freq;                                                                   /// 储存3元关系组的频率
		double _pr[15];                                                                        /// 储存平滑后低于频率阈值的概率
		ull _low_freq[15];                                                                    /// 低频节点的数量
		ull _total_sum;                                                                        /// 总数
		uint _min_tri_gram_pos;                                                                /// 最小的三元组的下标值，方便训练
		uint _threshold;                                                                       /// 进行Good-turing平滑的阈值
		bool _is_trained;                                                                      /// 是否已经对统计数据进行处理
		bool _is_smooth;                                                                       /// 是否已进行Good-turing平滑
		double _par1, _par2, _par3;                                                            /// 用于二阶tri gram模型的数据填充
	public:
		triGram();                                                                             /// 构造函数
		~triGram();                                                                            /// 析构函数
		void train();                                                                          /// 训练参数加平滑数据
		void good_turing_smooth();                                                             /// good-turing 对低频数据进行平滑
		double log_prob(uint sta1, _char ch1, uint sta2, _char ch2, uint sta3, _char ch3);     /// 获得三元组的概率（对数）
		vector <_word> cut_sentence(const _sentence& sentence);                                /// 切分词
		vector <charState> tag_sentence(const vector <_char> char_list);                       /// 给句子打上标签
		double div(ull _value1, ull _value2);                                                  /// 安全除法
		double find_tri(const ull& key);                                                       /// 查找三元组的频率
		uint find(const ull & key);                                                            /// 查找三元组的频率
		void clear();                                                                          /// 清除函数
		void show();                                                                           /// 展示函数，用于调试
		bool read_model(const std::string & fp);                                               /// 读取统计好的模型
	};

}
