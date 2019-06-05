# Lightweight-Chinese-Segmentation
轻量级中文分词系统（Lightweight Chinese Segmentation）  
一枚机器学习刚入门的小白的第一次尝试  
所有文件全放到source文件夹里了    
***由于Tri-Gram的模型太大没有放到github中，所以这里放个[百度云链接](https://pan.baidu.com/s/1zHnhOm3pUCuqtC1T2J6srA) 密码：63w7***  
***想使用的可以下载完成后把Tri-Gram模型放到source/init文件夹中或是修改配置文件config.utf8中关于Tri-Gram的模型的地址即可***



- ### 源代码

  - #### cs_define.h

    **这是一个定义文件，定义各种变量**

  - #### cs_char.h 和 cs_char.cpp

    **字典类，用于存储合法*中文字符*、*英文字符*、*阿拉伯数字*和*标点符号*，可以检查字符合法、中文字符繁简体的转换和其他字符全半角的转换。注意所有的字符都是Unicode中的字符**

  - #### cs_trie.h 和 cs_trie.cpp

    **字典树类和AC自动机类，写好的板子，只需要修改cs_trie.h中的trie_char的定义即可使用；AC自动机类和字典树类相似，只是多了一个fail数组，而且里面又写好的AC自动机的正向最大匹配的方法，速度几乎是线性，当然和[hancks](<https://github.com/hankcs/AhoCorasickDoubleArrayTrie> )大神双数组的AC自动机的分词速度还是有一定的差距**

  - #### cs_word.h 和 cs_word.cpp

    **词典类，用于存储中文词语，储存方式就是使用了AC自动机类，里面有词典快速分词和词典修正分词两个重要的方法**

  - #### cs_hmm.h 和 cs_hmm.cpp

    **一阶隐马字符标注模型，学习了[JIeba](https://github.com/fxsjy/jieba)分词中字符标注方法，将字符分类成此*词头*、*词中*、*词尾*和*独立成词*四类。解码方式使用了维特比算法，模型是是从SIGHAN提供的[back off](http://sighan.cs.uchicago.edu/bakeoff2005/)语料和流传在民间的《人民日报》2014年的语料利用最大似然估计的方法统计出的模型，分词结果再用词典修正，但效果一般**

  - #### cs_ngram.h 和 cs_ngram.cpp

    **三元组模型，借鉴了hancks大神的Tri-Gram模型加了一点优化，利用了状态压缩的方法将三元组的状态压缩成一个长整型，还对低出现率的三元组做了数据平滑，分词结果再用词典修正**

  - #### cs_tool.h 和 cs_tool.cpp

    **工具函数，里面主要有具体字符和储存方式之间的转换函数以及一些其他的小工具**

  - #### crf_test.exe、crf_train_model 和 libcrfpp.dll

    **是利用一个名为[CRF++](https://taku910.github.io/crfpp/)工具报训练出来的模型和使用的可执行文件，具体如何使用网上有很多教程，感兴趣的可以自己去搜，最后的效果挺棒的。其实想自己写一个CRF的，但最后因为太难放弃了，还是太弱了。**

  - #### main.cpp

    **主函数，里面有一些画的命令行界面和初始化函数、各种分词方法的调用，总的来说就是一个小分词系统的主函数，支持手动输入中文分词和中文文本分词。**

    

- ### 其他文件

  - #### config.utf8 

    **初始化的配置文件的地址**

  - #### init文件夹

    **存放各种初始化的文件**

  - #### makefile

    **由于是编译器自动编译的，没有写，会尽快补上！！！**







