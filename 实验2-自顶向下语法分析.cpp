#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string.h>
using namespace std;
string str;
int input_num = 0;
int location = 0;
int flag = 1;
vector<string> vec;
char map[50][50];
void T();
void E();
void E1();
void T1();
void F();

char a[100] ;
char fz[100] ; // ����ջ
int sz[100] ; // ����ջ
int fhead = 0 ; // ����ջָ��
int shead = 0 ; // ����ջָ��
int sum = 0;
void mathvt(char f) // ������ջ��ȡ��ջ������������ ���� f ���� �����������ջ��
{
    switch(f)
    {
        case '+' : sz[-- shead] += sz[shead + 1] ; break ;
        case '*' : sz[-- shead] *= sz[shead + 1] ; break ;
    }
    --fhead ;
    sz[shead+1]=0 ;
}



int invt(char a)
{
    if(a=='('||a==')'||a=='+'||a=='*')
    return 1;
    else return 0;
}
void match(char a)
{
    cout<<"match:"<<a<<endl;
    if(str[location]==a)
    location++;
    else flag = 0;
}
void E()
{
    cout<<"E->TE^"<<endl;
    T();
    E1();
}
void E1()
{
    if(str[location]=='+')
    {
        a[sum++] = '+';
        cout<<"E^->+TE^"<<endl;
        match('+');
        T();
        E1();
    }
}
void T()
{
    cout<<"T->FT^"<<endl;
    F();
    T1();
}
void T1()
{
    if(str[location]=='*')
    {
        a[sum++]='*';
        match('*');
        F();
        T1();
    }
}
void F()
{
    cout<<"F->i"<<endl;
    if(!invt(str[location])){
	    string ss;
        while(!invt(str[location])&&location<str.length())
        {
            a[sum++]=str[location];
            location++;
        }
		//input_num = atoi(ss.c_str());
		//vec.push_back(ss);
	}
    else if(str[location]=='(')
            {
                cout<<"F->(E)"<<endl;
                a[sum++]='(';
                match('(');
                E();
                if(str[location]==')')
                {
                    match(')');
                    a[sum++]=')';
                }

                else flag = 0;
            }
    else flag = 0;
}
void jsq()
{
    int len = strlen(a)-1;
        for(int i = 0 ; i <= len ; ++ i )
        {
            if(a[i] == '(' )
            {
                fz[++ fhead] = a[i] ;
                continue ;
            }
            if(a[i]==')')
            {
                while(fz[fhead] != '(')
                mathvt(fz[fhead]) ;
                --fhead ;
                continue ;
            }
            if(a[i] >= '0' && a[i] <= '9')
            {
                ++shead ;
                while(a[i] >= '0' && a[i] <= '9')
                    sz[shead] = sz[shead] *10 + a[i] - '0' ,i++;
                i--;
                continue;
            }
            else
            {
                if(a[i] == '*')
                {
                    fz[++fhead] = a[i];
                    continue;
                }
                else
                    while(fz[fhead] == '*'|| fz[fhead] == a[i])
                    {
                        mathvt(fz[fhead]);
                    }
                fz[++fhead] = a[i];
            }
        }
        while(fhead != 0) {
            mathvt(fz[fhead]) ;
            }
        cout<<endl<<"�������ǣ�"<<sz[shead]<<endl;
        fhead = 0;
        shead = 0;
        sum = 0;
        memset(a,'\0',sizeof(a));
        memset(fz,'\0',sizeof(fz));
        memset(sz,0,sizeof(sz));
}
int main()
{
    while(1)
    {
        memset(map,'\0',sizeof(map));
        vec.clear();
        location = 0;
        flag = 1;
        cout<<"����Ҫ�������ַ�����";
        cin>>str;
        E();
        if(flag&&location==str.length())
            cout<<"�����ɹ���"<<endl;
        else{
            cout<<"����ʧ�ܣ�"<<endl;
        }
        jsq();//�򵥼����������

    }
}
