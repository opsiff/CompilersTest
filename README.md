# README

## praser.cpp

一个作业向的编译器LR1文法及语法分析部分的代码

### 编译环境

DevC++ 5.11 with -stdc++11

### 功能

- 根据文法生成LR1分析表

- 实现龙书求FIRST集、求项目规范集族(CLOSURE、GOTO、items)、利用分析表进行语法分析的算法

- 输出到language.csv输出分析表

- 输出到csl.txt方便重新导入已生成的分析表进行分析(未完成有bug)

- 根据分析表对表达式进行语法分析并在控制台输出错误分析过程
