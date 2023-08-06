**原程序在VS Studio下可正常运行但在VSCode中会有问题，本仓库是为了实验git功能而尝试。**

# 大一上程序设计课程作业
## 简介
### 内包
- 实验源码:textAnalysis.cpp
- 实验报告
- 测试文本：test.txt A.txt B.txt
## 大作业参考选题之6：批量文本文件的词频 统计和检索

分工：独立完成

完成状况：上述功能基本实现

### 【任务描述】整理一批文本文件作为输入数据，实现以下处理功能：
1. 词频统计：统计输入文件中的每个单词在所有文件中出现的总次数，保存统计结果。
2. 建立单词索引：对所有单词记录每次出现时的信息，包括文件名、行号、所在的语句 内容等。
3. 查询词频：查询显示文件从出现频度最高的前10个单词；查询显示最热门的10个检索 单词；
输入一个单词，从统计结果中查询出该单词的在文件中出现总频次，查询出该单词被查询过的总次数。
4. 关键词检索：输入一个单词，查询出所有该单词出现过的文件名、行号和所在语句内容。
### 【基本要求】
1. 以上述功能为基础，设计所需的数据结构、程序功能模块等，可以根据需要扩展所需 功能。
2. 程序运行过程中，有方便合理的用户操作界面，能直观展示输入数据、处理过程及输 出结果。
3. 词频统计结果和索引数据，可以存储在数据文件或数据库中，数据结构、具体数据项 自定。
4. 输入文件内容为英文，不统计英文中出现的虚词，包括冠词、介词、连词和感叹词等
5. 输入的文件允许单个输入，也允许批量文件输入。

