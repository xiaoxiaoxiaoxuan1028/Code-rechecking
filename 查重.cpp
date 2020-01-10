#include <bits/stdc++.h>
using namespace std;

double repeat = 0;//用于记录重复字符数
int sig;//用于去除“/*...*/“注释的标记

void Delete(string &line)
{
    int n,m;
    if((n=line.find("/*")) != -1 && (m=line.find("*/")) == -1)
	{
        //如果找到“/*”（即注释开始标志），并且在同一行没有找到”*/“（即注释结束标志）
        sig = 1;
        return;//在找到”*/“之前，读入内容不存入字符串数组中
    }
    if((n=line.find("/*")) != -1 && (m = line.find("*/")) != -1)
	{
        //如果找到”/*“，并且在同一行找到了“*/”，则将字符串中“/*...*/”全部删除
        line.erase(n, m+2);
    }
    if((n=line.find("*/")) != -1 && sig == 1)
	{//这里要注意find（”*/“）返回的是”*“所在的位置
        //如果在sig=1（之前找到过“/*”）的情况下，找到了“*/”
        //则将这一行中的到“*/”的内容全部删除
        sig = 0;
        line.erase(0, n+2);
    }
    //以下判断语句是用于判断是否为头文件
    if((n=line.find("#")) != -1 && !line.empty())
	{
        line = "";//如果是头文件，则将这一行全部删除
    }
    //以下判断语句是用于删除“//”的注释
    if((n = line.find("//")) != -1)
        line.erase(n, line.length());
    //如果找到，则将从“//”开始到最后的字符串全部删除
    //以下判断语句是用于去除代码中的所有空格
    int index = 0;
    if(!line.empty())
	{
        while((index = line.find(" ",index)) != string::npos)
		{
            line.erase(index, 1);
            //如果找到，则将这个找到的空格删除，然后继续查找
        }
    }
    //以下判断语句是用于去除代码中的句末分号
    if((n=line.find_last_of(";")) == line.length()-1 && !line.empty())
        line.erase(n, line.length());
    //如果找到，则将其删除
}

//读入文件，并将文件中的代码以行进行处理后存储到String数组中
void readinfile(string filename, vector<string> &file_text)
{
    ifstream infile;
    infile.open(filename.data());
    assert(infile.is_open());//如果打开文件失败，则提出警示
    string line;//用于存储每一行
    while(getline(infile, line))
	{//通过getline函数直接读入每一行
        if(line != "\n"){//如果line=“\n”，则直接不存入String数组
            Delete(line);//对行进行处理
            if(line!="" && sig==0)//如果处理后字符串不为空并且sig=0（表示不是注释）
            {
                file_text.push_back(line);//则将该处理后的字符串存入数组
            }
        }
    }
    infile.close();//记得关闭文件
}//读入文件

//求解两个字符串的最长公共子序列,通过动态规划的方式求解
int CompareInLine(string line1,string line2)
{
    int n = line1.length();
    int m = line2.length();
    int dp[n+1][m+1];
    memset(dp,0,sizeof(dp));//将二维数组初始化--清零
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j)
		{
            if(line1[i-1] == line2[j-1])
				//如果当前字符相等则当前最长公共子序列数公式为：
                dp[i][j] = max(dp[i][j], dp[i-1][j-1]+1);
            else//如果当前字符不相等，则最长公共子序列数公式为：
                dp[i][j] = max(dp[i][j-1], dp[i-1][j]);
        }
    return dp[n][m];
}

//以下为计算两个代码的重复率
double Calculate(vector<string> file_text)
{
    int totalnum = 0;
    for(int i = 0; i < file_text.size(); ++i)
	{
        totalnum += file_text[i].length();//统计代码总字符数
    }
    return repeat/totalnum*100;//表示为百分数的形式
}

int main()
{
    string file1,file2;
    file1 = "./test_1.cpp";
    file2 = "./test_2.cpp";
    string a,b;
    int maxnum = -1;
    int num;
    vector<string> file1_text,file2_text;
    readinfile(file1, file1_text);
    readinfile(file2, file2_text);
    //这里是将两个文件进行处理后，分别存入两个string数组中
    for(int i = 0; i < file2_text.size(); ++i)
	{
        for(int j = 0; j < file1_text.size(); ++j)
		{
            num = CompareInLine(file2_text[i], file1_text[j]);
            if(num > maxnum)
                maxnum = num;
            //这里是将file2中的一行与file1中的每一行进行比较，然后取最大值
            //目的是为了防止交换某几行带来的重复率降低的情况
        }
        repeat += maxnum;//统计最长公共子序列
        maxnum = 0;
    }
    cout<<"两代码重复率为："<<Calculate(file2_text)<<"%"<<endl;
    return 0;
}
