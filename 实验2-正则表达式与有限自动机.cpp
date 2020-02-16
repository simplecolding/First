#include<iostream>
#include<string.h>
#include<stdio.h>
#include<vector>
#include<stdlib.h>
using namespace std;
int sum_end = 0;
int status = 0;
int elem = 0;
int start = 0;
int num = 0;
int dfa[100][100];//存储矩阵
int enddfa[100];//终态集合
char e[20];//字母表的元素
vector<int> vec;//辅助输出
int main()
{
    int inend(int* a,int b,int len);
	int getelemlocal(char *a,char b);
    void search(int now_status,int n,int now_elem_num);
	cout<<"输入状态的个数：";
	cin>>status;
	cout<<"输入始态：";
	cin>>start;
	////////////////////////////////////
	cout<<"输入终态的个数：";
	cin>>sum_end;
	cout<<"输入终态集合：";
///////////////////////////////////
	for(int i = 0;i<sum_end;i++)
	cin>>enddfa[i];
	cout<<"输入字母表的大小：";
	cin>>elem;
	cout<<"输入字母表元素：";
	for(int i = 0;i<elem;i++)
	{
	   cin>>e[i];
	}
	for(int i = 0;i<elem;i++)
	{
	   cout<<e[i];
	}
	cout<<"元素输入完毕\n";
	cout<<"输入状态转换矩阵：\n";
	for(int i = 0;i<status;i++)//数组初始化
	for(int j = 0;j<elem;j++)
		dfa[i][j]=-1;
	int begin = 0,end = 0;
	char cor;
	while(scanf("%d %c %d",&begin,&cor,&end)!=EOF)
	{
	if(begin>=status||end>=status||(e,cor)<0)
        {
            cout<<"数据错误！";
            return 0;
        }
        if(dfa[begin][getelemlocal(e,cor)]!=-1&&end!=getelemlocal(e,cor))//转换图中有，而且和之前的不一样（不是重复输入）
        {
            cout<<"非DFA";
            return 0;
        }
	    dfa[begin][getelemlocal(e,cor)]=end;
	}

    for(int i = 0;i<status;i++)
    {
        for(int j = 0;j<elem;j++)
        printf("%d",dfa[i][j]);
        cout<<endl;
    }
    int option=0;
    cout<<"1识别长度小于n的字符串，2识别字符串，其他退出：";
    while(scanf("%d",&option)!=EOF)
    {
        if(option==1)
        {
            cout<<"识别长度小于n的字符串的集合，请输入n：";
            cin>>num;
            search(start,0,0);
            cout<<"1识别长度小于n的字符串，2识别字符串，其他退出：";
        }
        else if(option==2)
        {
                    cout<<"输入要识别的字符串：";
            char input[100];
            cin>>input;
            int seei=0;
            int startstatus = 0;
            int flag = 0;
            int now_start=start;
            for(int seei = 0;seei<strlen(input);seei++)
            {
                if(dfa[now_start][getelemlocal(e,input[seei])]>=0)
                {
                    cout<<now_start<<endl;
                    now_start = dfa[now_start][getelemlocal(e,input[seei])];
                }

                else
                {
                    cout<<"该字符串不能识别！卡在了状态"<<now_start<<endl;
                    flag = 1;
                    break;
                }

            }
            if(!flag)
            {
                cout<<now_start<<endl;
                if(inend(enddfa,now_start,sum_end)) cout<<"字符串识别成功!\n";
                else cout<<"该字符串不能识别,最后一个状态不是终态！\n";
            }

            cout<<"1识别长度小于n的字符串，2识别字符串，其他退出：";
        }
        else
        {
            cout<<"\n再见！！！";
            return 0;
        }

    }


}
int getelemlocal(char *a,char b)
{
	int len = strlen(a);
	for(int i = 0;i<len;i++)
	{
		if(a[i]==b)
		return i;
	}
	return -1;
}
int inend(int* a,int b,int len)
{
    for(int i = 0;i<len;i++)
    if(b == a[i]) return 1;
    return 0;
}
void search(int now_status,int n,int now_elem_num)
{
    if(n<num&&dfa[now_status][now_elem_num]>=0)
    {
        vec.push_back(now_elem_num);
        if(inend(enddfa,dfa[now_status][now_elem_num],sum_end)&&n>=0)
        {

            for(int i = 0;i<vec.size();i++)
            {
                cout<<e[vec[i]];
            }
            cout<<"      #######\n";
        }
        search(dfa[now_status][now_elem_num],n+1,0);//多了一个元素
        if(!vec.empty())
        vec.pop_back();
    }
    for(int i = now_elem_num+1;i<elem;i++)
    {
        if(dfa[now_status][now_elem_num]>=0)
        search(now_status,n,i);
    }
}
