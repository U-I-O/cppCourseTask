//Text_Analysis.cpp
/*
大作业参考选题之6：批量文本文件的词频 统计和检索
【任务描述】整理一批文本文件作为输入数据，实现以下处理功能：
1. 词频统计：统计输入文件中的每个单词在所有文件中出现的总次数，保存统计结果。
2. 建立单词索引：对所有单词记录每次出现时的信息，包括文件名、行号、所在的语句 内容等。
3. 查询词频：查询显示文件从出现频度最高的前10个单词；查询显示最热门的10个检索 单词；
输入一个单词，从统计结果中查询出该单词的在文件中出现总频次，查询出该单词被查询过的总次数。
4. 关键词检索：输入一个单词，查询出所有该单词出现过的文件名、行号和所在语句内容。
【基本要求】
1. 以上述功能为基础，设计所需的数据结构、程序功能模块等，可以根据需要扩展所需 功能。
2. 程序运行过程中，有方便合理的用户操作界面，能直观展示输入数据、处理过程及输 出结果。
3. 词频统计结果和索引数据，可以存储在数据文件或数据库中，数据结构、具体数据项 自定。
4. 输入文件内容为英文，不统计英文中出现的虚词，包括冠词、介词、连词和感叹词等
5. 输入的文件允许单个输入，也允许批量文件输入。
【扩展要求】 1. 输入文件内容扩展为中文，进行中文词语切分后，再做中文词频统计和检索。
*/

//
//头部代码：引入相关文件，并定义一些静态变量和数据结构
//
#ifndef _HEAD_H
#define _HEAD_H

#pragma warning(disable: 4267)//忽视了vector的size函数返回的unsighed int无法精确转换到int的警告

#include<iostream>
#include<thread>
#include<fstream>
#include<string>
#include<ctime>
#include <algorithm>
#include<cmath>
#include<vector>
#include<regex>
#include<Windows.h>
#include<iomanip>
#include<cassert>

using namespace std;
using namespace chrono;

//
//常量
const int MaxSize = 10000;	//文章单词最大容量
//静态量
 int sum = 0;	//已统计单词总数(不重复)


//
//（50-120）行为信息储存数据结构
//


//
////PLACE结构体储存单词的位置信息
//
struct Place {
	string FileName = "None";//文件名
	int linenum = 0;//行数
	int SentenceNum = 0;//所在语句标号
};


//
//WF结构体储存单个单词
//
struct WordFrequency {
	string Wordname;//单词名
	int frequency = 0;//词频
	int required = 0;//被查询次数
	vector<Place>place;//储存位置的动态数组
};



//
//字符串数组储存虚词库，Process函数将调用把虚词给排除掉掉
//虚词不计入统计
//
vector<string>FunctionWord = {
	"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
	"an","the","there","who","up",
	"in","on","from","above","behind",
	"at","by","for","from","in","of","on","to","with",
	"onto","into","within",
	"and","both","as","not","but","yet","while",
	"or","either","so","that","whether","if","when","while","this","no","some","what",
	"before","after","once","untill","till","because","althought",
	"thought","even","whatever","however","unless","than","wherever",
	"am","is","are","were","was","be","have","has","had",
	"i","you","me","he","him","she","her","it","they","them","them","his","your","their","hers","theirs","my","us","we"
};

//
//符号库
//用于判定语句数
vector<char>punctuation = {
	'.','?','!',';',','
};


//范围为文件作用域
//实际信息储存体
//动态数组WF储存单词统计信息,起始的sum为0
static vector<WordFrequency>WF(sum);
//动态数组FN储存输入的文件名
vector<string>FN;

//
//相关函数的前向申明放这里
//
void MajorMenu();
void ProcessMenu();
void SearchMenu();

void BulkImport();
void StatisticsData(string filename);
int totlalline(string filename, string name);
bool Punctuation(char ch);
string WordTransition(string word);
void outputWF();
void frequencysort();
void SaveData();
void HotWord();
void SearchWord();
bool isValidFilePath(const string& str);
bool Empty();

#endif


//
//主函数
//
int main() {
	

	MajorMenu();//主菜单
	return 0;
};


//
//（128-230）行：菜单界面
//

//
//主菜单
//
void MajorMenu() {
	while (true) {
		system("cls");	//清屏
		cout << "*******************英文单词的词频统计和检索系统*******************" << endl;
		cout << "---主菜单---" << endl;
		cout << "1.处理文件" << endl;
		cout << "2.查询" << endl;
		cout << "3.退出系统" << endl;
		cout << "请按相应的数字键进行选择：" << endl;
		int n;
		cin >> n;

		switch (n) {
		case 1:
			ProcessMenu();//处理完成后的菜单
			break;
		case 2:
			SearchMenu();//查找菜单
			break;
		case 3:
			cout << "正在退出..." << endl;
			system("cls");	//清屏
			exit(0);
			break;	
		default:
			cout << "输入的不是有效符号，请重新输入" << endl;
			system("cls");	//清屏
			system("pause");	//暂停
			MajorMenu();
		};
	}	//for
}
	

//
//处理文件菜单
//同时完成文件路径的储存
//
void ProcessMenu() {
		system("cls");	//清屏
		cout << "*******************英文单词的词频统计和检索系统*******************" << endl;
		cout << "请逐个输入待处理文本的文件地址" << endl;
		cout << "输入3结束导入处理文件" << endl;

		while (true) {
			string FileName;
			cin >> FileName;

			const string& str = FileName;
			if (isValidFilePath(str)) {
				cout << "你输入的不是有效文件地址" << endl;
				cout << "将返回到主菜单" << endl;
				system("pause");
				MajorMenu();
			}
			else {
				if (FileName == "3") {
					cout << "已退出导入文件" << endl;
					if (Empty()) {
						BulkImport();//批量导入文件
						frequencysort();
						outputWF();
						SaveData();
						cout << "将返回到主菜单" << endl;
						system("pause");	//暂停
						MajorMenu();
					}
				}
				cout <<endl<< "请输入文件路径，或输入3退出"<<endl;
				FN.push_back(FileName);//向FN数组末尾添加文件路径
			}
			//测试用代码
			//测试程序时使用
			//FileName = "file.txt";
			//BulkImport();//批量导入文件
			//frequencysort();
			//outputWF();
			//SaveData();
			//cout << "将返回到主菜单" << endl;
			//system("pause");	//暂停
			//MajorMenu();
			//end
		};
}

//
//查找菜单
//
void SearchMenu() {
	while (true) {
		system("cls");	//清屏
		cout << "*******************英文单词的词频统计和检索系统*******************" << endl;
		cout << "---查找菜单---" << endl;
		cout << "1.最热单词（显示文件从出现频度最高的前10个单词）" << endl;
		cout << "2.关键词检索" << endl;
		cout << "3.返回上一级" << endl;
		cout << "请按相应的数字键进行选择：" << endl;
		int n;
		cin >> n;
		switch (n) {
		case 1:
			HotWord();
			break;
		case 2:
			SearchWord();
			break;
		case 3:
			system("cls");	//清屏
			MajorMenu();
			break;
		default:
			cout << "输入的不是有效符号，请重新输入" << endl;
		}
	};
}


//
// （230-650）行：文本处理代码段
// 

//
//批量导入文本
//
void BulkImport() {
	cout << "正在导入文本" << endl;
	int TxtNum = FN.size();//文本数量
	for (int i = 0; i < TxtNum; i++) {
		string filename = FN[i];//filename表示当前处理的文本路径
		StatisticsData(filename);
	};
};


//
//文件处理函数
// 调用预处理函数，分析统计函数，保存输出数据函数
// 
void StatisticsData(string filename) {

	//储存文件名
	string name;
	name = filename.substr(filename.rfind("\\") + 1, filename.rfind("."));

	ifstream fin;	//创建只可读对象
	fin.open(filename);//关联文件
	
	//判断文件是否成功打开
	if (!fin.is_open()) {
		system("cls");
		cout << name << "没有成功打开\n" << endl;
	}
	else
		cout << "成功打开了" << name << "文件" << endl;

	//按行分析操作
	//按行读取
	string LineText;	//储存每一行字符串
	int linenum = 0;	//储存行数
	string word;	//储存单词
	
	int totallinenum = totlalline(filename, name);

	while (getline(fin, LineText)) {
		//统一为小写
		LineText = WordTransition(LineText);
		int sentencenum = 0;//标记所在语句号用于后期查询
		for (char & ch : LineText) {//读取单词
			
			//检测是否为句子末尾
			
			if (Punctuation(ch))
				sentencenum++;

			//读取一个字符
			if (ch >= 'a' && ch <= 'z') {
				if (word == "\0")	//word为空，放入第一个字母
					word = ch;
				else
					word += ch;	//word不为空，拼接字母组成单词
			}	//if
				//一个单词读取完毕

			else {
				
				//判断之前的word里面是否有单词
				if (word != "\0") {
					//有单词，总数+1
					sum++;
					//限制单个文件读取单词数量不超过MaxSize
					if (sum >= MaxSize) {
						cout << "文章单词超出统计上限，系统已退出" << endl;
						fin.close();	//关闭文件
						exit(0);	//退出程序
						system("pause");	//暂停
					}

					//排掉虚词,保存单词信息
					//迭代器访问vector数组
					bool functionwordtest = true;//默认通过
					vector<string>::const_iterator iter;
					iter = FunctionWord.begin();
					for (iter; iter != FunctionWord.end(); iter++)
						if (word == *iter)functionwordtest = false;

					//
					//对WF结构体判空
					bool empty = true;
					empty = (WF.size() == 0);

					//
					//对单词进行重复性检测
					bool repeat = false;//默认不重复
					vector<WordFrequency>::iterator iter_0=WF.end();
					for (vector<WordFrequency>::iterator iter = WF.begin(); iter != WF.end(); iter++) {
						if (iter->Wordname == word) {
							repeat = true;
							iter_0 = iter;
						}
					}
					
					//
					//通过虚词检测则准备存入WF数组
					if (functionwordtest) {
						Place pl;
						pl.FileName = name;
						pl.linenum = linenum;
						pl.SentenceNum = sentencenum;
						WordFrequency wf;//当前单词信息
						wf.Wordname = word;
						wf.required = 0;
						wf.frequency = 1;
						wf.place.push_back(pl);
						if(empty){
							WF.push_back(wf);
						}
						else {
							if (repeat) {	
								//单词重复
								iter_0->frequency++;	//词频+1
								//位置储存
								iter_0->place.push_back(pl);

							}	//if
							else {
								//单词不重复
								WF.push_back(wf);
							}
					
						}
					}
					word = "\0";	//重置word，用于存放新单词

				}//if
				
			}	//else
			
		}//for
		linenum = linenum + 1;//每轮循环行数加1		
		if (linenum == totallinenum+1)
			break;
	}//while
	fin.close();//关闭文件
};

//
//大写英语单词转化成小写
//
string WordTransition(string word) {
	for (int i = 0; i < int(word.size()); i++) {	//获取字符串长度，使用length()也可以
		if (word[i] >= 'A' && word[i] <= 'Z')	//判断字符是否是大写字母
			word[i] = word[i] + 32;	//ASCII码表中十进制 A==65  a==97  中间相差32，后面的也是如此
	}	//for
	return word;	//返回小写单词
}

//
//判断读取的字符是否为标点
bool Punctuation(char ch) {
	int i = 0;
	bool mark=true;
	while (mark) {
		int i = 0;
		if (ch == punctuation[i]){
			return true;
			mark = false;
		}
		if (i <= punctuation.size()) {
			return 0;
			mark = false;
		}
		i++;
	};
	return mark;
}



//
//在处理完文本后自动保存数据
//
void SaveData() {

	system("cls");
	cout << "正在保存..." << endl;
	ofstream fout;//创建只写对象fout
	fout.open("output.txt", ios::trunc);//准备写入统计数据,并重置文本
	//检测是否成功创建文件
	if (!fout.is_open()) {
		cout << "结果无法成功保存" << endl;
		system("pause");
	}
	fout << "--------Word----------{Frequency,Required}------Place{filename,(line,sentencenum)}---------------------" << endl;//写入标题信息
	for (int i = 0; i < WF.size(); i++) {		//写入WF结构体到文件中
		string part1 = WF[i].Wordname;
		string part2 = "{" + to_string(WF[i].frequency) + "," + to_string(WF[i].required) + "}";
		string part3 ="{";
		for (int n=0; n < WF[i].place.size(); n++) {
			if (regex_match(part3, regex(WF[i].place[n].FileName)))
				part3 += "  "+WF[i].place[n].FileName;
			part3 += "(" + to_string(WF[i].place[n].linenum) + "," + to_string(WF[i].place[n].SentenceNum) + ")";
		}
		fout <<setw(20)<<part1<< setw(10)<<part2<<setw(50)<<part3+"}" << endl;
		
	}
	fout.close();
	cout << "保存完成，保存到文件\"output.txt\"" << endl;

}

//
//输出WF数组，检测功能是否正常实现
//
void outputWF() {
	cout << "--------Word----------{Frequency,Required}------Place{filename,(line,sentencenum)}---------------------" << endl;//写入标题信息
	for (int i = 0; i < WF.size(); i++) {		//写入WF结构体到文件中
		string part1 = WF[i].Wordname;
		string part2 = "{" + to_string(WF[i].frequency) + "," + to_string(WF[i].required) + "}";
		string part3 = "{";
		for (int n = 0; n < WF[i].place.size(); n++) {
			if (regex_match(part3, regex(WF[i].place[n].FileName)))
				part3 += "  " + WF[i].place[n].FileName;
			part3 += "(" + to_string(WF[i].place[n].linenum) + "," + to_string(WF[i].place[n].SentenceNum) + ")";
		}
		cout << setw(20) << part1 << setw(10) << part2 << setw(70) << part3 + "}" << endl;

	}
	cout << endl;
	cout << "输出完毕" << endl;
	system("pause");
}

//
//获取文本总行数
int totlalline(string filename,string name) {
	ifstream fin;	//创建只可读对象
	fin.open(filename);//关联文件

	//判断文件是否成功打开
	if (!fin.is_open()) {
		system("cls");
		cout << name << "没有成功打开\n" << endl;
	}
	int totalLineNum = 0;
	string temp;
	while (getline(fin, temp, '\n'))
	{
		++totalLineNum;
	}
	return totalLineNum;
}


//
//根据频次排序WF数组
void frequencysort() {

	//反向迭代器
	vector<WordFrequency>::reverse_iterator iter_0 = WF.rbegin();
	//用0号迭代器遍历WF数组
	for (iter_0; iter_0 < WF.rend(); iter_0++) {
		vector<WordFrequency>::reverse_iterator iter_1 = WF.rbegin();
		//用1号迭代器遍历WF数组
		for (iter_1; iter_1 < WF.rend(); iter_1++) {
			if (iter_0->frequency > iter_1->frequency)
				//交换两个迭代器所指元素
				iter_swap(iter_0, iter_1);
		}
	}
	cout << "根据频次顺序调整完毕" << endl;
}

//
//显示出现频次最高的10的单词
void HotWord() {
	//对WF结构体判空
	bool empty = true;
	empty = (WF.size() == 0);
	if (empty)
	{
		cout << "请先分析文本后再进行查询" << endl;
		cout << "将返回主菜单" << endl;
		system("pause");
		MajorMenu();
	}
	else {
		system("cls");
		cout << "*******************英文单词的词频统计和检索系统*******************" << endl;
		cout << endl;
		vector<WordFrequency>::reverse_iterator iter = WF.rbegin();
		for (iter; iter <= (WF.rbegin() + 9); iter++) {
			string part1 = iter->Wordname;
			string part2 = "{" + to_string(iter->frequency) + "," + to_string(iter->required) + "}";
			string part3 = "{";
			for (int n = 0; n < iter->place.size(); n++) {
				if (regex_match(part3, regex(iter->place[n].FileName)))
					part3 += "  " + iter->place[n].FileName;
				part3 += "(" + to_string(iter->place[n].linenum) + "," + to_string(iter->place[n].SentenceNum) + ")";
			}
			cout << setw(20) << part1 << setw(10) << part2 << setw(70) << part3 + "}" << endl;

		}
		//暂停程序让用户看清输出结果
		system("pause");
	}
}

//
//查询一个单词的信息，并显示它的所在语句
void SearchWord() {
	system("cls");
	//对WF结构体判空
	bool empty = true;
	empty = (WF.size() == 0);
	if (empty)
	{
		cout << "请先分析文本后再进行查询" << endl;
		cout << "将返回主菜单" << endl;
		system("pause");
		MajorMenu();
	}
	else {
		string WORD;
		cout << "请输入你要查找的单词" << endl<<"输入3返回上一级\n";
		cin >> WORD;
		if (WORD == "3") {

			cout << "正在返回上一级";
			SearchMenu();
		}

		//mark用于判断输入的词是否在词库中
		bool mark = true;
		for (int i = 0; i < WF.size(); i++) {
			if (WORD == WF[i].Wordname) {
				mark = false;//mark标记着存在此单词
				system("cls");
				cout << "*******************英文单词的词频统计和检索系统*******************" << endl;
				cout << endl;
				cout << "单词名:" << WF[i].Wordname << endl
					<< "出现频次：" << WF[i].frequency << "  "
					<< "被查询次数:" << WF[i].required << endl;
				WF[i].required++;//被查询数加一

				for (int n = 0; n < WF[i].place.size(); n++) {
					cout << "文件名：" << WF[i].place[n].FileName << " "
						<< WF[i].place[n].linenum << "行"
						<< WF[i].place[n].SentenceNum << "句"
						<< "\n出现的第" << n + 1 << "次为:";
					//查找FN数组中的此单词地址
					string address;//储存此次打开的文件地址
					string str = WF[i].place[n].FileName;
					//find函数如果找到了则返回指针
					for (int u = 0; u < FN.size(); u++) {
						if (FN[u].find(str) != string::npos) {
							address = FN[u];
						}
						else;
					}

					//只读对象fin 用于提取出单词所在语句
					fstream fin;
					fin.open(address);
					//检测是否成功创建文件
					if (!fin.is_open()) {
						cout << "结果无法成功打开单词所在文件" << endl;
						system("pause");
					}

					//按行分析操作
					//按行读取
					string LineText;	//储存每一行字符串
					int linenum = 0;	//储存行数
					regex reg("[(--);.?！]");//匹配句号、问号、感叹号

					int totallinenum = totlalline(address, str);
					
					bool mark = true;//如果成功输出了单词则改为False控制单词是否找到的信息输出
					while (getline(fin, LineText)) {
						//找目标行
						if (linenum == WF[i].place[n].linenum) {

							//LineText = WordTransition(LineText);//统一为小写

							int sentencenum = 0;//标记所在语句号用于后期查询

							for (char& ch : LineText) {//读取单词
								
								//检测是否为句子末尾
								
								if (Punctuation(ch))
									sentencenum++;
								
								//匹配对应语句
								if (sentencenum == WF[i].place[n].SentenceNum) {
									cout << ch;
									mark = false;
									}
						
							}//for
						}//判定目标行的if结束

						//每轮循环行数加1
						linenum = linenum + 1;		

						if ((linenum== totallinenum)&&(mark)) {
							cout << "未在" + str + "文件中找到语句" << endl;
							break;
						}//if
					}//while

					fin.close();//关闭文件
					cout << endl;
				}

			}
			else if((i==WF.size()-1)&&mark){
				cout << "词库中没有所查询的单词，请检查是拼写的正确性" << endl;
				system("pause");
				SearchMenu();
			};
		};
		system("pause");//让用户看清数据
	}
}

//
//判断是否输入的为有效路径
bool isValidFilePath(const std::string& path)
{
	regex pattern("^txt");
	return regex_search(path, pattern);
}

//
//判断数组是否为空
bool Empty(){
	if (FN.size() == 0)
		return false;
	else
		return true;
}