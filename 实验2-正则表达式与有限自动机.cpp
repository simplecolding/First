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
int dfa[100][100];//�洢����
int enddfa[100];//��̬����
char e[20];//��ĸ���Ԫ��
vector<int> vec;//�������
int main()
{
    int inend(int* a,int b,int len);
	int getelemlocal(char *a,char b);
    void search(int now_status,int n,int now_elem_num);
	cout<<"����״̬�ĸ�����";
	cin>>status;
	cout<<"����ʼ̬��";
	cin>>start;
	////////////////////////////////////
	cout<<"������̬�ĸ�����";
	cin>>sum_end;
	cout<<"������̬���ϣ�";
///////////////////////////////////
	for(int i = 0;i<sum_end;i++)
	cin>>enddfa[i];
	cout<<"������ĸ��Ĵ�С��";
	cin>>elem;
	cout<<"������ĸ��Ԫ�أ�";
	for(int i = 0;i<elem;i++)
	{
	   cin>>e[i];
	}
	for(int i = 0;i<elem;i++)
	{
	   cout<<e[i];
	}
	cout<<"Ԫ���������\n";
	cout<<"����״̬ת������\n";
	for(int i = 0;i<status;i++)//�����ʼ��
	for(int j = 0;j<elem;j++)
		dfa[i][j]=-1;
	int begin = 0,end = 0;
	char cor;
	while(scanf("%d %c %d",&begin,&cor,&end)!=EOF)
	{
	if(begin>=status||end>=status||(e,cor)<0)
        {
            cout<<"���ݴ���";
            return 0;
        }
        if(dfa[begin][getelemlocal(e,cor)]!=-1&&end!=getelemlocal(e,cor))//ת��ͼ���У����Һ�֮ǰ�Ĳ�һ���������ظ����룩
        {
            cout<<"��DFA";
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
    cout<<"1ʶ�𳤶�С��n���ַ�����2ʶ���ַ����������˳���";
    while(scanf("%d",&option)!=EOF)
    {
        if(option==1)
        {
            cout<<"ʶ�𳤶�С��n���ַ����ļ��ϣ�������n��";
            cin>>num;
            search(start,0,0);
            cout<<"1ʶ�𳤶�С��n���ַ�����2ʶ���ַ����������˳���";
        }
        else if(option==2)
        {
                    cout<<"����Ҫʶ����ַ�����";
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
                    cout<<"���ַ�������ʶ�𣡿�����״̬"<<now_start<<endl;
                    flag = 1;
                    break;
                }

            }
            if(!flag)
            {
                cout<<now_start<<endl;
                if(inend(enddfa,now_start,sum_end)) cout<<"�ַ���ʶ��ɹ�!\n";
                else cout<<"���ַ�������ʶ��,���һ��״̬������̬��\n";
            }

            cout<<"1ʶ�𳤶�С��n���ַ�����2ʶ���ַ����������˳���";
        }
        else
        {
            cout<<"\n�ټ�������";
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
        search(dfa[now_status][now_elem_num],n+1,0);//����һ��Ԫ��
        if(!vec.empty())
        vec.pop_back();
    }
    for(int i = now_elem_num+1;i<elem;i++)
    {
        if(dfa[now_status][now_elem_num]>=0)
        search(now_status,n,i);
    }
}
