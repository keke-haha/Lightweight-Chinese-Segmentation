//============================================================================
// Name        : main.cpp
// Author      : JustOneStep
// Version     :
// Copyright   : Your copyright notice
// Description : Chinese Segmention in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include <windows.h>
#include <cstdlib>
#include <dirent.h>

#include "cs_define.h"
#include "cs_tool.h"
#include "cs_hmm.h"
#include "cs_trie.h"
#include "cs_char.h"
#include "cs_ngram.h"
#include "cs_word.h"
using namespace std;

typedef pair <cs::_char, char> pair_val;
typedef cs::triGram trigram;
typedef cs::hmmSegmentation hmm;
typedef cs::charDictionary dict;
typedef cs::wordDictionary wdict;
trigram mytri;
hmm myhmm;
dict mydict;
wdict mywdict;

map <string, string> config;


/*--------------------------------------- 初始化函数声明 ---------------------------------------*/
void all_init();
bool init_charDictionary(cs::charDictionary & now_dict, const string & info_fp);             /// 初始化字典
bool init_wordDictionary(cs::wordDictionary & now_dict, const string & word_list_fp);        /// 初始化词典
bool init_hmmSegmentation_model(cs::hmmSegmentation & now_hmm, const string & model_fp);     /// 初始化HMM模型
bool init_triGram_model(cs::triGram & now_tri, const string & model_fp);                     /// 初始化TriGram模型
/*---------------------------------------------------------------------------------------------*/

/*---------------------------------------显示界面函数声明---------------------------------------*/
void show_main_page();                           /// 显示主页面
void show_subpage_one();                         /// 显示子页面1，繁简体转换页面
void show_subpage_two();                         /// 显示子页面2，全半角转换页面
void show_subpage_three();                       /// 显示子页面3，简单分词页面
void show_subpage_four();                        /// 显示子页面4，精确分词页面
void show_subpage_five();                        /// 显示子页面5，高精度分词页面
void show_bye_page();                            /// 显示结束界面
void show_in_page();                             /// 显示输入选择界面
void show_out_page();                            /// 显示输出选择界面
/*---------------------------------------------------------------------------------------------*/

/*------------------------------------界面信息处理函数声明---------------------------------------*/
void main_page();                                     /// 显示主页面信息处理
void subpage_one();                                   /// 子页面1，繁简体转换页面信息处理
void subpage_two();                                   /// 子页面2，全半角转换页面信息处理
void subpage_three();                                 /// 子页面3，简单分词页面信息处理
void subpage_four();                                  /// 子页面4，精确分词页面信息处理
void subpage_five();                                  /// 子页面5，高精度分词页面信息处理
bool in_page();                                       /// 输入选择界面信息处理
bool out_page();                                      /// 输出选择界面信息处理
bool in_out_path(string & in_path, string & out_path);/// 输入选择、输出选择信息处理

/*---------------------------------------------------------------------------------------------*/

/*-----------------------------------------工具函数声明-----------------------------------------*/
bool judge_fp_legal(const string & fp);          /// 判断文件路径合法性
bool judge_dp_legal(const string & dp);          /// 判断文件夹路径合法性
/*---------------------------------------------------------------------------------------------*/

/*--------------------------------------算法执行函数声明-----------------------------------------*/

string change_to_simp(dict & now_dict, string & sen);                        /// 转化成简体字
string change_to_trad(dict & now_dict, string & sen);                        /// 转化成繁体字
string change_to_full(dict & now_dict, string & sen);                        /// 转化成全角
string change_to_half(dict & now_dict, string & sen);                        /// 转化成半角
bool file_change_to_simp(dict & now_dict, string & in_fp, string & out_dir); /// 文件转化成简体字
bool file_change_to_trad(dict & now_dict, string & in_fp, string & out_dir); /// 文件转化成繁体字
bool file_change_to_full(dict & now_dict, string & in_fp, string & out_dir); /// 文件转化成全角
bool file_change_to_half(dict & now_dict, string & in_fp, string & out_dir); /// 文件转化成半角

bool file_dict_cut(wdict & now_dict, string & in_fp, string & out_dir);                     /// 文件快速分词
bool file_tri_cut(wdict & now_dict, trigram & now_tri, string & in_fp, string & out_dir);   /// 文件trigram分词
bool file_hmm_cut(wdict & now_dict, hmm & now_hmm, string & in_fp, string & out_dir);       /// 文件hmm分词

string dict_cut(wdict & now_dict, string & sen, bool flag = false);     /// 字典分词函数
string tri_cut(wdict & now_dict, trigram & now_tri, string & sen);      /// Tri-Gram模型分词函数
string hmm_cut(wdict & now_dict,hmm & now_hmm, string & sen);           /// 隐马模型分词函数
void crf_cut(string & sen);                                             /// 条件随机场分词函数
/*---------------------------------------------------------------------------------------------*/

int main() {
	all_init();
	main_page();
	return 0;
}

void all_init() {

	ifstream file_in("config.utf8");
	if(!file_in.is_open()) {
		cerr << "启动失败" << endl;
		exit(0);
	}
	string file_name, file_path;
	while(file_in >> file_name >> file_path)
		config[file_name] = file_path;
	file_in.close();

	init_charDictionary(mydict, config["CharDict"]);
	init_wordDictionary(mywdict, config["WordDict"]);
	init_hmmSegmentation_model(myhmm, config["HMM"]);
	init_triGram_model(mytri, config["TriGram"]);
}

bool init_charDictionary(cs::charDictionary & now_dict, const string & info_fp) {
	ifstream file_in(info_fp);
	if(!file_in.is_open()) {
		cout << "字典初始化失败!!!" << endl;
		return false;
	}
	string str_line;
	vector <string> str_list;
	while(getline(file_in, str_line)) {
		str_list = cs::split_string(str_line, ' ');
		if("///" == str_list[0])
			continue;

		if(str_list[1] == "0") {
			now_dict.add_char_info(cs::string_to_char(str_list[0]), cs::_simple);
		} else if(str_list[1] == "1") {
			now_dict.add_char_info(cs::string_to_char(str_list[0]), cs::_traditional);
		} else if(str_list[1] == "2") {
			now_dict.add_char_info(cs::string_to_char(str_list[0]), cs::_english);
		} else if(str_list[1] == "3") {
			now_dict.add_char_info(cs::string_to_char(str_list[0]), cs::_number);
		} else if(str_list[1] == "4") {
			now_dict.add_char_info(cs::string_to_char(str_list[0]), cs::_symbol);
		}

		if(str_list.size() != 3) continue;

		cs::_char first_char = cs::string_to_char(str_list[0]);
		cs::_char second_char = cs::string_to_char(str_list[2]);

		if(str_list[1] == "0")
			now_dict.add_trad_simp_relationship(first_char, second_char);
		else if(str_list[1] == "1")
			now_dict.add_trad_simp_relationship(first_char, second_char, true);
		else if(str_list[1] == "2" || str_list[1] == "3" || str_list[1] == "4")
			now_dict.add_full_half_relationship(first_char, second_char);
	}

	file_in.close();
	cout << "字典初始化成功" << endl;

	return true;
}

bool init_wordDictionary(cs::wordDictionary & now_dict, const string & word_list_fp) {
	ifstream file_in(word_list_fp);
	if(!file_in.is_open()) {
		cerr << "词典初始化失败" << endl;
		return false;
	}
	string fp;
	vector <string> fp_list;
	while(file_in >> fp) fp_list.push_back(fp);
	file_in.close();
	now_dict.initialization(fp_list);
	cout << "词典初始化成功" << endl;
	return true;
}

bool init_hmmSegmentation_model(cs::hmmSegmentation & now_hmm, const string & model_fp) {
	bool successful = now_hmm.read_model(model_fp);
	if(successful) {
		now_hmm.train();
		cout << "隐马模型初始化成功" << endl;
		return true;
	} else {
		now_hmm.clear();
		return false;
	}
}

bool init_triGram_model(cs::triGram & now_tri, const string & model_fp) {
	bool successful = now_tri.read_model(model_fp);
	if(successful) {
		now_tri.train();
		cout << "tri-gram模型初始化成功" << endl;
		return true;
	} else {
		now_tri.clear();
		return false;
	}
}


void show_main_page() {
	printf("\t\t----------------------------------------------------------------------------------------\n");
	printf("\t\t|                                        主界面                                        |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t|                  1.繁简体转换                                                        |\n");
	printf("\t\t|                                                        2.全半角转换                  |\n");
	printf("\t\t|                  3.简单分词                                                          |\n");
	printf("\t\t|                                                        4.精确分词                    |\n");
	printf("\t\t|                  5.高精度分词                                                        |\n");
	printf("\t\t|                                                        6.退出                        |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t----------------------------------------------------------------------------------------\n");
}

void show_subpage_one() {
	printf("\t\t----------------------------------------------------------------------------------------\n");
	printf("\t\t|                                    繁简体转换界面                                    |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t|                  1.简体转繁体                                                        |\n");
	printf("\t\t|                                     2.繁体转简体                                     |\n");
	printf("\t\t|                                                        3.退回主界面                  |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t----------------------------------------------------------------------------------------\n");
}

void show_subpage_two() {
	printf("\t\t----------------------------------------------------------------------------------------\n");
	printf("\t\t|                                    全半角转换界面                                    |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t|                  1.全角转半角                                                        |\n");
	printf("\t\t|                                     2.半角转全角                                     |\n");
	printf("\t\t|                                                        3.退回主界面                  |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t----------------------------------------------------------------------------------------\n");
}

void show_subpage_three() {
	printf("\t\t----------------------------------------------------------------------------------------\n");
	printf("\t\t|                                     简单分词界面                                     |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t|                  1.词语全切分                                                        |\n");
	printf("\t\t|                                      2.快速切分                                      |\n");
	printf("\t\t|                                                        3.退回主界面                  |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t----------------------------------------------------------------------------------------\n");
}

void show_subpage_four() {
	printf("\t\t----------------------------------------------------------------------------------------\n");
	printf("\t\t|                                     精确分词界面                                     |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t|                  1.隐马模型切分                                                      |\n");
	printf("\t\t|                                    2.Tri-Gram切分                                    |\n");
	printf("\t\t|                                                        3.退回主界面                  |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t----------------------------------------------------------------------------------------\n");
}

void show_subpage_five() {
	printf("\t\t----------------------------------------------------------------------------------------\n");
	printf("\t\t|                                    高精度分词界面                                    |\n");
	printf("\t\t|                         （由于解码速率，目前只支持直接输入）                         |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t|                  1.直接输入                            2.退回主界面                  |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t----------------------------------------------------------------------------------------\n");
}

void show_bye_page() {
	printf("\t\t----------------------------------------------------------------------------------------\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t|                                     感谢您的使用                                     |\n");
	printf("\t\t|                                        再见！                                        |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t----------------------------------------------------------------------------------------\n");
}

void show_in_page() {
	printf("\t\t----------------------------------------------------------------------------------------\n");
	printf("\t\t|                                     输入选择界面                                     |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t|                  1.直接输入                                                          |\n");
	printf("\t\t|                                                        2.文件输入                    |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t----------------------------------------------------------------------------------------\n");
}

void show_out_page() {
	printf("\t\t----------------------------------------------------------------------------------------\n");
	printf("\t\t|                                     输出选择界面                                     |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t|                  1.直接输出                                                          |\n");
	printf("\t\t|                                                        2.文件输出                    |\n");
	printf("\t\t|                                                                                      |\n");
	printf("\t\t----------------------------------------------------------------------------------------\n");
}

void main_page() {
	string str_line;
	show_main_page();
	while(true) {
		printf("请输入你的选择：");
		getline(cin, str_line);
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") {
			cerr << "输入不能为空，请从新输入\n";
		}
		else if(str_line == "1")  {
			subpage_one();
			show_main_page();
		} else if(str_line == "2")  {
			subpage_two();
			show_main_page();
		} else if(str_line == "3")  {
			subpage_three();
			show_main_page();
		} else if(str_line == "4")  {
			subpage_four();
			show_main_page();
		} else if(str_line == "5")  {
			subpage_five();
			show_main_page();
		} else if(str_line == "6")  {
			show_bye_page();
			Sleep(3000);
			break;
		} else {
			cerr << "输入错误，请从新输入\n";
		}
	}
}

void subpage_one() {
	show_subpage_one();
	string str_line, in_path, out_path;
	bool my_select;
	while(true) {
		printf("请输入你的选择：");
		getline(cin, str_line);
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") {
			cerr << "输入不能为空，请从新输入\n";
		} else if(str_line == "1") {
			my_select = in_page();
			if(!my_select) {
				cout << "请输入中文（80字以内）：";
				getline(cin, str_line);
				if(str_line.empty() || str_line == "")
					cerr << "输入为空，请从新选择\n";
				else
					cout << "转化结果：" << change_to_trad(mydict, str_line) << endl;
			} else {
				my_select = in_out_path(in_path, out_path);
				if(!my_select) continue;
				file_change_to_trad(mydict, in_path, out_path);
				show_subpage_one();
			}
		} else if(str_line == "2") {
			my_select = in_page();
			if(!my_select) {
				cout << "请输入中文（80字以内）：";
				getline(cin, str_line);
				if(str_line.empty() || str_line == "")
					cerr << "输入为空，请从新选择\n";
				else
					cout << "转化结果：" << change_to_simp(mydict, str_line) << endl;
			} else {
				my_select = in_out_path(in_path, out_path);
				if(!my_select) continue;
				file_change_to_simp(mydict, in_path, out_path);
				show_subpage_one();
			}
		} else if(str_line == "3") {
			break;
		} else {
			cerr << "输入错误，请从新输入\n";
		}
	}
}

void subpage_two() {
	show_subpage_two();
	string str_line, in_path, out_path;
	bool my_select;
	while(true) {
		printf("请输入你的选择：");
		getline(cin, str_line);
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") {
			cerr << "输入不能为空，请从新输入\n";
		} else if(str_line == "1") {
			my_select = in_page();
			if(!my_select) {
				cout << "请输入中文（80字以内）：";
				getline(cin, str_line);
				if(str_line.empty() || str_line == "")
					cerr << "输入为空，请从新选择\n";
				else
					cout << "转化结果：" << change_to_half(mydict, str_line) << endl;
			} else {
				my_select = in_out_path(in_path, out_path);
				if(!my_select) continue;
				file_change_to_half(mydict, in_path, out_path);
				show_subpage_two();
			}
		} else if(str_line == "2") {
			my_select = in_page();
			if(!my_select) {
				cout << "请输入中文（80字以内）：";
				getline(cin, str_line);
				if(str_line.empty() || str_line == "")
					cerr << "输入为空，请从新选择\n";
				else
					cout << "转化结果：" << change_to_full(mydict, str_line) << endl;
			} else {
				my_select = in_out_path(in_path, out_path);
				if(!my_select) continue;
				file_change_to_full(mydict, in_path, out_path);
				show_subpage_two();
			}
		} else if(str_line == "3") {
			break;
		} else {
			cerr << "输入错误，请从新输入\n";
		}
	}
}

void subpage_three() {
	show_subpage_three();
	string str_line, in_path, out_path;
	bool my_select;
	while(true) {
		printf("请输入你的选择：");
		getline(cin, str_line);
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") {
			cerr << "输入不能为空，请从新输入\n";
		} else if(str_line == "1") {
			cout << "请输入中文（80字以内）：";
			getline(cin, str_line);
			str_line = cs::remove_string(str_line, ' ');
			if(str_line.empty() || str_line == "")
				cerr << "输入为空，请从新选择\n";
			else
				cout << "分词结果：" << dict_cut(mywdict, str_line, true) << endl;
		} else if(str_line == "2") {
			my_select = in_page();
			if(!my_select) {
				cout << "请输入中文（80字以内）：";
				getline(cin, str_line);
				str_line = cs::remove_string(str_line, ' ');
				if(str_line.empty() || str_line == "")
					cerr << "输入为空，请从新选择\n";
				else
					cout << "分词结果：" << dict_cut(mywdict, str_line) << endl;
			} else {
				my_select = in_out_path(in_path, out_path);
				if(!my_select) continue;
				file_dict_cut(mywdict, in_path, out_path);
				show_subpage_three();
			}
		} else if(str_line == "3") {
			break;
		} else {
			cerr << "输入错误，请从新输入\n";
		}
	}
}

void subpage_four() {
	show_subpage_four();
	string str_line, in_path, out_path;
	bool my_select;
	while(true) {
		printf("请输入你的选择：");
		getline(cin, str_line);
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") {
			cerr << "输入不能为空，请从新输入\n";
		} else if(str_line == "1") {
			my_select = in_page();
			if(!my_select) {
				cout << "请输入中文（80字以内）：";
				getline(cin, str_line);
				str_line = cs::remove_string(str_line, ' ');
				if(str_line.empty() || str_line == "")
					cerr << "输入为空，请从新选择\n";
				else
					cout << "分词结果：" << hmm_cut(mywdict, myhmm, str_line) << endl;
			} else {
				my_select = in_out_path(in_path, out_path);
				if(!my_select) continue;
				file_hmm_cut(mywdict, myhmm, in_path, out_path);
				show_subpage_four();
			}
		} else if(str_line == "2") {
			my_select = in_page();
			if(!my_select) {
				cout << "请输入中文（80字以内）：";
				getline(cin, str_line);
				str_line = cs::remove_string(str_line, ' ');
				if(str_line.empty() || str_line == "")
					cerr << "输入为空，请从新选择\n";
				else
					cout << "分词结果：" << tri_cut(mywdict, mytri, str_line) << endl;
			}else {
				my_select = in_out_path(in_path, out_path);
				if(!my_select) continue;
				file_tri_cut(mywdict, mytri, in_path, out_path);
				show_subpage_four();
			}
		} else if(str_line == "3") {
			break;
		} else {
			cerr << "输入错误，请从新输入\n";
		}
	}
}

void subpage_five() {
	show_subpage_five();
	string str_line, sen;
	while(true) {
		printf("请输入你的选择：");
		getline(cin, str_line);
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") {
			cerr << "输入不能为空，请从新输入\n";
		} else if(str_line == "1") {
			cout << "请输入中文（80字以内）：";
			getline(cin, sen);
			crf_cut(sen);
		} else if(str_line == "2") {
			break;
		} else {
			cerr << "输入错误，请从新输入\n";
		}
	}
}

bool in_page() {
	show_in_page();
	string str_line;
	while(true) {
		printf("请输入你的选择：");
		getline(cin, str_line);
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") {
			cerr << "输入不能为空，请从新输入\n";
		} else if(str_line == "1") {
			return false;
		} else if(str_line == "2") {
			return true;
		} else {
			cerr << "输入错误，请从新输入\n";
		}
	}
	return false;
}

bool out_page() {
	show_out_page();
	string str_line;
	while(true) {
		printf("请输入你的选择：");
		getline(cin, str_line);
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") {
			cerr << "输入不能为空，请从新输入\n";
		} else if(str_line == "1") {
			return false;
		} else if(str_line == "2") {
			return true;
		} else {
			cerr << "输入错误，请从新输入\n";
		}
	}
	return false;
}

bool in_out_path(string & in_path, string & out_path) {

	printf("请输入你的输入文件的路径：");
	getline(cin, in_path);
	printf("请输入你的输出文件的存放路径：");
	getline(cin, out_path);
	if(!judge_fp_legal(in_path)) {
		cerr << "你的输入文件找不到" << endl;
		return false;
	}
	if(!judge_dp_legal(out_path)) {
		cerr << "你的输出文件的存放文件夹找不到" << endl;
		return false;
	}
	return true;
}

bool judge_fp_legal(const string & fp) {
	ifstream file_in(fp);
	if(file_in.is_open()) {
		file_in.close();
		return true;
	}
	else
		return false;
}

bool judge_dp_legal(const string & dp) {
	DIR * d = opendir(dp.c_str());
	if(d == NULL)
		return false;
	else {
		closedir(d);
		return true;
	}
}

string change_to_simp(dict & now_dict, string & sen) {
	vector <cs::_char> char_list = cs::sentence_to_char(sen);
	string ret;
	for(int i = 0, s = char_list.size(); i < s; i ++)
		ret += cs::char_to_string(now_dict.trad_to_simp(char_list[i]));
	return ret;
}

string change_to_trad(dict & now_dict, string & sen) {
	vector <cs::_char> char_list = cs::sentence_to_char(sen);
	string ret;
	for(int i = 0, s = char_list.size(); i < s; i ++)
		ret += cs::char_to_string(now_dict.simp_to_trad(char_list[i]));
	return ret;
}

string change_to_full(dict & now_dict, string & sen) {
	vector <cs::_char> char_list = cs::sentence_to_char(sen);
	string ret;
	for(int i = 0, s = char_list.size(); i < s; i ++)
		ret += cs::char_to_string(now_dict.half_to_full(char_list[i]));
	return ret;
}

string change_to_half(dict & now_dict, string & sen) {
	vector <cs::_char> char_list = cs::sentence_to_char(sen);
	string ret;
	for(int i = 0, s = char_list.size(); i < s; i ++)
		ret += cs::char_to_string(now_dict.full_to_half(char_list[i]));
	return ret;
}

bool file_change_to_simp(dict & now_dict, string & in_fp, string & out_dir) {
	ifstream file_in(in_fp.c_str());
	if(!file_in.is_open()) {
		cerr << "input file " << in_fp << "open error" << endl;
		return false;
	}
	clock_t startTime, endTime;
	startTime = clock();
	int char_num = 0;
	string str_line, tmp;
	vector <cs::_char> char_list;
	vector <string> sentence_list;
	while(getline(file_in, str_line)) {
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") continue;
		char_list = cs::sentence_to_char(str_line);
		char_num += char_list.size();
		for(int i = 0, s = char_list.size(); i < s; i ++)
			tmp += cs::char_to_string(now_dict.trad_to_simp(char_list[i]));
		if(tmp.empty()) continue;
		sentence_list.push_back(tmp);
		tmp.clear();
	}

	ofstream file_out(out_dir + "change_to_simp.utf8");
	if(!file_out.is_open()) {
		cerr << "output file " << out_dir << "open error" << endl;
		return false;
	}
	for(int i = 0, s = sentence_list.size(); i < s; i ++)
		file_out << sentence_list[i] << endl;
	file_out.close();
	endTime = clock();
	cout << "文件 " << in_fp << " 繁体转简体完成！" << endl << "整个文件共计 " << char_num << " 字\n";
	cout << "转换共花费时间 " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒\n";
	cout << "转换完成结果文件存储地址：" << out_dir + "change_to_simp.utf8" << endl;
	return true;
}

bool file_change_to_trad(dict & now_dict, string & in_fp, string & out_dir) {
	ifstream file_in(in_fp.c_str());
	if(!file_in.is_open()) {
		cerr << "input file " << in_fp << "open error" << endl;
		return false;
	}
	clock_t startTime, endTime;
	startTime = clock();
	int char_num = 0;
	string str_line, tmp;
	vector <cs::_char> char_list;
	vector <string> sentence_list;
	while(getline(file_in, str_line)) {
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") continue;
		char_list = cs::sentence_to_char(str_line);
		char_num += char_list.size();
		for(int i = 0, s = char_list.size(); i < s; i ++)
			tmp += cs::char_to_string(now_dict.simp_to_trad(char_list[i]));
		if(tmp.empty()) continue;
		sentence_list.push_back(tmp);
		tmp.clear();
	}

	ofstream file_out(out_dir + "change_to_trad.utf8");
	if(!file_out.is_open()) {
		cerr << "output file " << out_dir << "open error" << endl;
		return false;
	}
	for(int i = 0, s = sentence_list.size(); i < s; i ++)
		file_out << sentence_list[i] << endl;
	file_out.close();
	endTime = clock();
	cout << "文件 " << in_fp << " 简体转繁体完成！" << endl << "整个文件共计 " << char_num << " 字\n";
	cout << "转换共花费时间 " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒\n";
	cout << "转换完成结果文件存储地址：" << out_dir + "change_to_trad.utf8" << endl;
	return true;
}

bool file_change_to_full(dict & now_dict, string & in_fp, string & out_dir) {
	ifstream file_in(in_fp.c_str());
	if(!file_in.is_open()) {
		cerr << "input file " << in_fp << "open error" << endl;
		return false;
	}
	clock_t startTime, endTime;
	startTime = clock();
	int char_num = 0;
	string str_line, tmp;
	vector <cs::_char> char_list;
	vector <string> sentence_list;
	while(getline(file_in, str_line)) {
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") continue;
		char_list = cs::sentence_to_char(str_line);
		char_num += char_list.size();
		for(int i = 0, s = char_list.size(); i < s; i ++)
			tmp += cs::char_to_string(now_dict.half_to_full(char_list[i]));
		if(tmp.empty()) continue;
		sentence_list.push_back(tmp);
		tmp.clear();
	}

	ofstream file_out(out_dir + "change_to_full.utf8");
	if(!file_out.is_open()) {
		cerr << "output file " << out_dir << "open error" << endl;
		return false;
	}
	for(int i = 0, s = sentence_list.size(); i < s; i ++)
		file_out << sentence_list[i] << endl;
	file_out.close();
	endTime = clock();
	cout << "文件 " << in_fp << " 半角转全角完成！" << endl << "整个文件共计 " << char_num << " 字\n";
	cout << "转换共花费时间 " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒\n";
	cout << "转换完成结果文件存储地址：" << out_dir + "change_to_full.utf8" << endl;
	return true;
}

bool file_change_to_half(dict & now_dict, string & in_fp, string & out_dir) {
	ifstream file_in(in_fp.c_str());
	if(!file_in.is_open()) {
		cerr << "input file " << in_fp << "open error" << endl;
		return false;
	}
	clock_t startTime, endTime;
	startTime = clock();
	int char_num = 0;
	string str_line, tmp;
	vector <cs::_char> char_list;
	vector <string> sentence_list;
	while(getline(file_in, str_line)) {
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") continue;
		char_list = cs::sentence_to_char(str_line);
		char_num += char_list.size();
		for(int i = 0, s = char_list.size(); i < s; i ++)
			tmp += cs::char_to_string(now_dict.full_to_half(char_list[i]));
		if(tmp.empty()) continue;
		sentence_list.push_back(tmp);
		tmp.clear();
	}

	ofstream file_out(out_dir + "change_to_half.utf8");
	if(!file_out.is_open()) {
		cerr << "output file " << out_dir << "open error" << endl;
		return false;
	}
	for(int i = 0, s = sentence_list.size(); i < s; i ++)
		file_out << sentence_list[i] << endl;
	file_out.close();
	endTime = clock();
	cout << "文件 " << in_fp << " 全角转半角完成！" << endl << "整个文件共计 " << char_num << " 字\n";
	cout << "转换共花费时间 " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒\n";
	cout << "转换完成结果文件存储地址：" << out_dir + "change_to_half.utf8" << endl;
	return true;
}

bool file_dict_cut(wdict & now_dict, string & in_fp, string & out_dir) {
	ifstream file_in(in_fp.c_str());
	if(!file_in.is_open()) {
		cerr << "input file " << in_fp << "open error" << endl;
		return false;
	}
	clock_t startTime, endTime;
	int char_num = 0;
	startTime = clock();
	vector <string> sentence_list;
	string str_line, tmp;
	while(getline(file_in, str_line)) {
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") continue;
		char_num += cs::sentence_char_num(str_line);
		sentence_list.push_back(str_line);
	}
	file_in.close();
	endTime = clock();
	cout << "文件 " << in_fp << " 读取完成" << endl;
	cout << "读取共花费时间 " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒\n";
	startTime = clock();
	ofstream file_out(out_dir + "dict_cut_result.utf8");
	if(!file_out.is_open()) {
		cerr << "output file " << out_dir << "open error" << endl;
		return false;
	}
	vector <string> word_list;
	for(int i = 0, s = sentence_list.size(); i < s; i ++) {
		word_list = now_dict.dictionary_segmention(sentence_list[i]);
		for(int j = 0, ws = word_list.size(); j < ws; j ++) {
			if(j) tmp += " / ";
			tmp += word_list[j];
		}
		file_out << tmp << endl;
		tmp.clear();
	}
	file_out.close();
	endTime = clock();
	cout << "文件 " << in_fp << " 快速分词完成！" << endl << "整个文件共计 " << char_num << " 字\n";
	cout << "转换共花费时间 " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒\n";
	cout << "转换完成结果文件存储地址：" << out_dir + "dict_cut_result.utf8" << endl;
	return true;
}

bool file_tri_cut(wdict & now_dict, trigram & now_tri, string & in_fp, string & out_dir) {
	ifstream file_in(in_fp.c_str());
	if(!file_in.is_open()) {
		cerr << "input file " << in_fp << "open error" << endl;
		return false;
	}
	clock_t startTime, endTime;
	int char_num = 0;
	startTime = clock();
	vector <string> sentence_list;
	string str_line, tmp;
	while(getline(file_in, str_line)) {
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") continue;
		char_num += cs::sentence_char_num(str_line);
		sentence_list.push_back(str_line);
	}
	file_in.close();
	endTime = clock();
	cout << "文件 " << in_fp << " 读取完成" << endl;
	cout << "读取共花费时间 " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒\n";
	startTime = clock();
	ofstream file_out(out_dir + "tri_cut_result.utf8");
	if(!file_out.is_open()) {
		cerr << "output file " << out_dir << "open error" << endl;
		return false;
	}
	vector <string> word_list;
	for(int i = 0, s = sentence_list.size(); i < s; i ++) {
		word_list = now_tri.cut_sentence(sentence_list[i]);
		word_list = now_dict.dictionary_revise(word_list);
		for(int j = 0, ws = word_list.size(); j < ws; j ++) {
			if(j) tmp += " / ";
			tmp += word_list[j];
		}
		file_out << tmp << endl;
		tmp.clear();
	}
	file_out.close();
	endTime = clock();
	cout << "文件 " << in_fp << " tri-gram分词完成！" << endl << "整个文件共计 " << char_num << " 字\n";
	cout << "转换共花费时间 " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒\n";
	cout << "转换完成结果文件存储地址：" << out_dir + "tri_cut_result.utf8" << endl;
	return true;
}

bool file_hmm_cut(wdict & now_dict, hmm & now_hmm, string & in_fp, string & out_dir) {
	ifstream file_in(in_fp.c_str());
	if(!file_in.is_open()) {
		cerr << "input file " << in_fp << "open error" << endl;
		return false;
	}
	clock_t startTime, endTime;
	int char_num = 0;
	startTime = clock();
	vector <string> sentence_list;
	string str_line, tmp;
	while(getline(file_in, str_line)) {
		str_line = cs::remove_string(str_line, ' ');
		if(str_line.empty() || str_line == "") continue;
		char_num += cs::sentence_char_num(str_line);
		sentence_list.push_back(str_line);
	}
	file_in.close();
	endTime = clock();
	cout << "文件 " << in_fp << " 读取完成" << endl;
	cout << "读取共花费时间 " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒\n";
	startTime = clock();
	ofstream file_out(out_dir + "hmm_cut_result.utf8");
	if(!file_out.is_open()) {
		cerr << "output file " << out_dir << "open error" << endl;
		return false;
	}
	vector <string> word_list;
	for(int i = 0, s = sentence_list.size(); i < s; i ++) {
		word_list = now_hmm.cut_sentence(sentence_list[i]);
		word_list = now_dict.dictionary_revise(word_list);
		for(int j = 0, ws = word_list.size(); j < ws; j ++) {
			if(j) tmp += " / ";
			tmp += word_list[j];
		}
		file_out << tmp << endl;
		tmp.clear();
	}
	file_out.close();
	endTime = clock();
	cout << "文件 " << in_fp << " hmm分词完成！" << endl << "整个文件共计 " << char_num << " 字\n";
	cout << "转换共花费时间 " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "秒\n";
	cout << "转换完成结果文件存储地址：" << out_dir + "hmm_cut_result.utf8" << endl;
	return true;
}

string dict_cut(wdict & now_dict, string & sen, bool flag) {
	string ret;
	vector <string> tmp_list = now_dict.dictionary_segmention(sen, flag);
	for(string tmp : tmp_list) {
		 ret += tmp;
		 ret += " // ";
	}
	return ret;
}

string tri_cut(wdict & now_dict, trigram & now_tri, string & sen) {
	vector <string> tmp_list =  now_tri.cut_sentence(sen);
	tmp_list = now_dict.dictionary_revise(tmp_list);
	string ret;
	for(string tmp : tmp_list) {
		 ret += tmp;
		 ret += " // ";
	}
	return ret;
}

string hmm_cut(wdict & now_dict, hmm & now_hmm, string & sen) {
	vector <string> tmp_list =  now_hmm.cut_sentence(sen);
	tmp_list = now_dict.dictionary_revise(tmp_list);
	string ret;
	for(string tmp : tmp_list) {
		 ret += tmp;
		 ret += " // ";
	}
	return ret;
}

void crf_cut(string & sen) {
	vector <string> sen_list = cs::split_string(sen, ' ');
	if(sen_list.empty()) {
		cerr << "你的中文输入为空" << endl;
		return;
	}
	ifstream file_in;
	ofstream file_out;
	string str, tmp1, tmp2;
	vector <cs::_char> char_list;
	vector <string> out_list, state_list;
	file_out.open("data_in.utf8");
	if(!file_out.is_open()) {
		cerr << "data_in.utf8 open error" << endl;
		return;
	}
	for(int i = 0, sen_list_size = sen_list.size(); i < sen_list_size; i ++) {
		char_list = cs::sentence_to_char(sen_list[i]);
		for(int j = 0, s = char_list.size(); j < s; j ++) {
			str = cs::char_to_string(char_list[j]);
			out_list.push_back(str);
			file_out << str << endl;
		}
	}
	file_out.close();
	file_out.open("data_out.utf8");
	if(file_out.is_open()) file_out.close();
//	string op = "./" + config["CRFTest"] + " -m " + config["CRFModel"] + " data_in.utf8 >> data_out.utf8";
//	system(op.c_str());
	system("./crf_test.exe -m crf_train_model data_in.utf8 >> data_out.utf8");
	file_in.open("data_out.utf8");
	if(!file_in.is_open()) {
		cerr << "data_out.utf8 open error" << endl;
		return;
	}
	while(file_in >> tmp1 >> tmp2)
		state_list.push_back(tmp2);
	file_in.close();
	printf("分词结果：");
	for(int i = 0, s = out_list.size(); i < s; i ++) {
		if(i && (state_list[i] == "B" || state_list[i] == "S"))
			cout << " // ";
		cout << out_list[i];
	}
	cout << endl;
}
