#include<iostream>
#include<vector>
#include<map>
#include <iomanip>
using namespace std;
char AA = '@';
string inputstring;
string instr;
string str[20];//str[0]用来拓广，下标为0用来表示产生式的左部。
vector<string> vec_str;//存储所有的活前缀。
vector<string> vec_closure[500];//用来存储某个状态的lr0项目，closure
vector<char> vec_elem;//用来记录每一个closure的@之后的一个元素
vector<int> vec_num;
vector<int> vec_state;
vector<int> vec_before[20];
map<int,map<char,int> >Action;//用来存储nfa状态转换图
map<int,map<char,int> >Goto;//用来记录goto
map<int,map<char,int> >Go;
map<char,string> first;
map<char,string> follow;
int cllo = 0;
int num = 0;//用来存储拓广后产生式的个数
void toadd(int n,int k);
int findstr(string n);
void getclosure();
int inthis(int cll,string a);
int notelem(char a)
{
    for(int i = 0;i<vec_elem.size();i++)
    {
        if(vec_elem[i]==a)
        return 0;
    }
    return 1;
}
void reads()
{
    cout<<"请输入产生式的个数：";
    int k = 0,i = 0;
    cin>>k;
    while(k--)
    {
        cin>>instr;
        if(i==0)
        {
            str[i]="Q";
            str[i]+=instr[0];//S`->S
            vec_elem.push_back(instr[0]);
            i++;
        }
        int j = 1;string a="";
        while(j<instr.length())
        {
            if(notelem(instr[j])&&instr[j]!='|')
            {
                vec_elem.push_back(instr[j]);
            }
            if(instr[j]!='|')
            {
                a = a+instr[j];
            }
            else{
            str[i]=instr[0]+a;
            i++;
            a="";
            }
            j++;
        }
        if(a!="")
        {
            str[i]=instr[0]+a;
            i++;
        }
        num = i;
    }

    cout<<"输入的文法经过拓广后（第一个字母为文法左部）："<<endl;
    for(int i = 0;i<num;i++)
    cout<<str[i]<<endl;
    cout<<num<<"所有项目"<<endl;
    for(int i = 0;i<vec_elem.size();i++)
    cout<<vec_elem[i];
    cout<<endl;
}
int canbedo()
{
    for(int i = 0;i<cllo;i++)
    {
        int flag1 = 0;//移进项
        int flag2 = 0;//规约项
        for(int j = 0;j<vec_closure[i].size();j++)
        {
            int len = vec_closure[i][j].size()-1;
            if(vec_closure[i][j][len]==AA)
            {
                if(!flag2&&!flag1)
                {
                    flag2 = 1;
                }
                else return 0;//有多个规约项或者同时存在规约和移进
            }
            else
            {
                if(flag2)
                return 0;
                else
                flag1 = 1;
            }
        }
    }
    return 1;
}
int inThisfirst(char a,char b)
{
    if(b==AA) return 1;
    for(int i = 0;i<first[a].size();i++)
    {
        if(first[a][i]==b)
            return 1;
    }
    return 0;
}
int inThisfollow(char a,char b)
{
    if(b == AA) return 1;
    for(int i = 0;i<follow[a].size();i++)
    {
        if(follow[a][i]==b)
            return 1;
    }
    return 0;
}
void get_first()
{
    int flag = 1;
    while(flag)
    {
        flag = 0;
        for(int i = vec_str.size()-1;i>=0;i--)//从最后一个字符串开始计算
        {
            if(vec_str[i][1]>='A'&&vec_str[i][1]<='Z')//第一个是非终结符
            {
                for(int j = 0;j<first[vec_str[i][1]].size();j++)//将改终结符的first集合放到目前元素的first集合中。
                {
                    char a = first[vec_str[i][1]][j];
                    if(!inThisfirst(vec_str[i][0],a))
                    {
                        first[vec_str[i][0]] += a;
                        flag = 1;
                    }
                }
            }
            else{//第一个是终结符
                if(!inThisfirst(vec_str[i][0],vec_str[i][1]))
                {
                    first[vec_str[i][0]] += vec_str[i][1];
                    flag = 1;
                }
            }
        }
    }
    cout<<endl<<"first集合："<<endl;
    for(int i = 0;i<vec_elem.size();i++)
    {
        if(vec_elem[i]>='A'&&vec_elem[i]<='Z')
        {
            cout<<vec_elem[i]<<":"<<first[vec_elem[i]]<<endl;
        }
    }
}
void get_follow()
{
    follow['Q'] += "#";
    int flag = 1;
    while(flag)
    {
        flag = 0;
        for(int i = 0;i<vec_str.size();i++)
        {
            if(vec_str[i][1]!=AA) continue;
            int maxlen = vec_str[i].size()-1;
            for(int j = 2;j<vec_str[i].size();j++)
            {
                if(vec_str[i][j]>='A'&&vec_str[i][j]<='Z')//找到一个非终结符
                {
                    char ch = vec_str[i][j];
                    char fch = vec_str[i][0];
                    if(j==maxlen)//这是该尝试的最后一个符号（非终结符）
                    {
                        for(int k = 0;k<follow[fch].size();k++)
                        {
                            if(!inThisfollow(ch,follow[fch][k]))
                            {
                                follow[ch] += follow[fch][k];
                                flag = 1;
                            }
                        }
                    }
                    else if(vec_str[i][j+1]>='A'&&vec_str[i][j+1]<='Z')//跟在这个非终结符之后的是一个非终结符
                    {
                        char ch = vec_str[i][j+1];
                        char fch = vec_str[i][j];
                        for(int k = 0;k<first[ch].size();k++)
                        {
                            if(first[ch][k]=='^') continue;
                            if(!inThisfollow(fch,first[ch][k]))
                            {
                                follow[fch] += first[ch][k];
                                flag = 1;
                            }
                        }
                    }
                    else{
                        cout<<"AAAAA"<<vec_str[i][j+1];
                        char ch = vec_str[i][j+1];//此时该非终结符之后的值为终结符
                        char fch = vec_str[i][j];
                        if(!inThisfollow(fch,ch))
                        {
                            follow[fch] += ch;
                            flag = 1;
                        }
                    }
                }
            }
        }
    }
    cout<<"follow集："<<endl;
    for(int i = 0;i<vec_elem.size();i++)
    {
        if(vec_elem[i]>='A'&&vec_elem[i]<='Z')
        {
            cout<<vec_elem[i]<<":"<<follow[vec_elem[i]]<<endl;
        }
    }
}
int findx(char a)//返回项目下标
{
    for(int i = 0;i<vec_str.size();i++)
    {
        if(vec_str[i][0]==a)
            return i;
    }
}
void closure()
{
    for(int i = 0;i<num;i++)
    {
        int len = str[i].length();
        for(int j = 1;j<=str[i].length();j++)
        {
            string ss="";
            if(j==len)
            ss = str[i]+"@";
            else
            {
                ss = str[i].substr(0,j)+"@";
                ss += str[i].substr(j);
            }
            vec_str.push_back(ss);//获取所有的活前缀。
        }
    }
    for(int i = 0;i<vec_str.size();i++)
        cout<<vec_str[i]<<endl;
    cout<<endl;
    getclosure();
    for(int i = 0;i<cllo;i++)
    {
        for(int j = 0;j<vec_closure[i].size();j++)
        cout<<vec_closure[i][j]<<endl;
        cout<<"closure:"<<i<<endl<<endl;
    }
}
void toadd(int now,int i)//输入字符串求其构造
{
        for(int j = 0;j<vec_closure[now][i].size();j++)//搜索其中一个字符串，字符串有多少的字符
        {
            if(vec_closure[now][i][j]=='@')
            {
                if(j!=vec_closure[now][i].size()-1)
                {
                    if(vec_closure[now][i][j+1]>='A'&&vec_closure[now][i][j+1]<='Z')
                    {
                        int flag = findx(vec_closure[now][i][j+1]);
                        //cout<<vec_closure[now][i][j+1]<<"++++++++++++++++++++++++"<<now<<"+++";
                        while(flag<vec_str.size()&&vec_str[flag][0]==vec_closure[now][i][j+1])
                        {
                            if(vec_str[flag][1]=='@'&&!inthis(now,vec_str[flag]))
                            {
                                vec_closure[now].push_back(vec_str[flag]);
                                flag++;
                            }
                           flag++;
                        }
                    }
                }

            }
        }
}
int inclosure(string a)
{
    for(int i = 0;i<=cllo;i++)
    {
        for(int j = 0;j<vec_closure[i].size();j++)
        {
            if(vec_closure[i][j]==a)
            return i;
        }
    }
    return -1;
}
int inthis(int cll,string a)
{
    for(int i = 0;i<vec_closure[cll].size();i++)
    {
        if(vec_closure[cll][i]==a)
        return 1;
    }
    return 0;
}
int notinnum(int nu)
{
    for(int i = 0;i<vec_num.size();i++)
    {
        if(vec_num[i]==nu)
        return 0;
    }
    return 1;
}
int find_num(string a)
{
    for(int i = 0;i<num;i++)
    {
        if(str[i]==a)
            return i;
    }
    return -1;
}
void getclosure()
{
    vec_closure[0].push_back(vec_str[0]);
    vec_before[1].push_back(0);
    cllo++;
    //int first = 1;
    for(int now = 0;now<cllo;now++)
    {
        //cout<<"开始！"<<now<<" "<<cllo<<endl;
        //先拓广，在分别对各项进行处理，这里是进行nfa的构造
        for(int i = 0;i<vec_closure[now].size();i++)
        {
            toadd(now,i);
        }
        cout<<"拓展完毕"<<endl;//先构造好当前项目集
        for(int n = 0;n < vec_elem.size();n++)//对一个符号在某一个状态中循环
        {
            int flag = 1;
            char elem = vec_elem[n];
            for(int i = 0;i<vec_closure[now].size();i++)//遍历当前状态的所有字符串
            {
                int len = vec_closure[now][i].length()-1;
                if(vec_closure[now][i][len]=='@'&&now!=1)//用来记录规约集合
                {
                    int ng = find_num(vec_closure[now][i].substr(0,len));
                    if(notinnum(ng))
                    {
                        vec_num.push_back(ng);
                        vec_state.push_back(now);
                    }
                }
                for(int j = 0;j<vec_closure[now][i].size();j++)//搜索其中一个字符串，字符串有多少的字符
                {
                    if(vec_closure[now][i][j]=='@')
                    {
                        if(vec_elem[n]==vec_closure[now][i][j+1])
                        {
                            int k = findstr(vec_closure[now][i])+1;//在vec_str中找到该字符串，找其后一个，即为@后移一位。
                            if(k<vec_str.size())
                            {
                                if(now==4&&elem=='E')
                                {
                                    //cout<<vec_str[k]<<"qqqqqqqqqqqqqqqqqqqqqqqq"<<endl;
                                }
                                if(inclosure(vec_str[k])==-1)//第一个,在其他的状态中都没有找到。
                                {
                                    if(flag)
                                    {
                                        vec_closure[cllo].push_back(vec_str[k]);
                                        flag = 0;
                                        Go[now][elem] = cllo;
                                        //cout<<"gggggggggggggggggg"<<Go[cllo-1][elem]<<endl;
                                    }
                                    else{
                                        vec_closure[cllo].push_back(vec_str[k]);
                                        Go[now][elem] = cllo;
                                    }
                                }
                                else{//处理在其他项目集里有的情况。在其他项目集找到了
                                    if(!flag) vec_closure[cllo].push_back(vec_str[k]);
                                    string a = vec_str[k];
                                    for(int i = 0;i<=cllo;i++)//在其他项目集找该字符串
                                    {
                                        for(int j = 0;j<vec_closure[i].size();j++)
                                        {
                                            if(vec_closure[i][j]==a)
                                            {
                                                Go[now][elem] = i;
                                            }

                                        }
                                    }
                                }
                            }

                        }
                    }
                }

            }
            if(!flag)
            {
                vec_before[cllo].push_back(now);
                cllo++;
            }

        }
    }

}
int findstr(string n)
{
    for(int i = 0;i<vec_str.size();i++)
    {
        if(vec_str[i]==n)
        return i;
    }
}
int in_state(int n)
{
    //for(int i = 0;i<vec_state;i++);
}
void lr0()
{
    Action[1]['#']=99;
    vec_elem.push_back('#');
    for(int i = 0;i<cllo;i++)
    {
        for(int j = 0;j<vec_elem.size();j++)
        {
            if(Go[i][vec_elem[j]])
            {
                if(vec_elem[j]>='A'&&vec_elem[j]<='Z')
                Goto[i][vec_elem[j]]=Go[i][vec_elem[j]];
                else
                {
                    Action[i][vec_elem[j]]=Go[i][vec_elem[j]];
                }

            }
        }
    }
    for(int i = 0;i<vec_state.size();i++)
    {
        for(int j = 0;j<vec_elem.size();j++)
        {
            if(vec_elem[j]>='A'&&vec_elem[j]<='Z') continue;
            else{
                Action[vec_state[i]][vec_elem[j]]=-vec_num[i];
            }
        }
    }
    cout<<endl<<"LR0分析表如下："<<endl;
    cout<<" ";
    for(int i = 0;i<vec_elem.size();i++)
    {
        if(vec_elem[i]>='A'&&vec_elem[i]<='Z') continue;
        else
        cout<<setw(3)<<vec_elem[i];
    }
    for(int i = 0;i<vec_elem.size();i++)
    {
        if(vec_elem[i]>='A'&&vec_elem[i]<='Z')
        cout<<setw(3)<<vec_elem[i];
    }
    cout<<endl;
    for(int i = 0;i<cllo;i++)
    {
        cout<<i;
        for(int j = 0;j<vec_elem.size();j++)
        {
            if(vec_elem[j]>='A'&&vec_elem[j]<='Z') continue;
            else
            {
                if(Action[i][vec_elem[j]]) cout<<setw(3)<<Action[i][vec_elem[j]];
                else cout<<setw(3)<<"x";
            }
        }
        for(int j = 0;j<vec_elem.size();j++)
        {
            if(vec_elem[j]>='A'&&vec_elem[j]<='Z')
            {
                if(Goto[i][vec_elem[j]]) cout<<setw(3)<<Goto[i][vec_elem[j]];
                else cout<<setw(3)<<"x";
            }
        }
        cout<<endl;
    }
}
int in_follow(char fo,char c)
{
    for(int i = 0;i<follow[fo].size();i++)
    {
        if(follow[fo][i]==c)
            return 1;
    }
    return 0;
}
void slr1()
{
    Action[1]['#']=99;
    vec_elem.push_back('#');
    for(int i = 0;i<cllo;i++)
    {
        for(int j = 0;j<vec_elem.size();j++)
        {
            if(Go[i][vec_elem[j]])
            {
                if(vec_elem[j]>='A'&&vec_elem[j]<='Z')
                Goto[i][vec_elem[j]]=Go[i][vec_elem[j]];
                else
                {
                    Action[i][vec_elem[j]]=Go[i][vec_elem[j]];
                }

            }
        }
    }
    for(int i = 0;i<vec_state.size();i++)
    {
        for(int j = 0;j<vec_elem.size();j++)
        {
            int n = vec_num[i];
            if(!in_follow(str[n][0],vec_elem[j])) continue;
            if(vec_elem[j]>='A'&&vec_elem[j]<='Z') continue;
            else{
                Action[vec_state[i]][vec_elem[j]]=-vec_num[i];
            }
        }
    }
    cout<<endl<<"SLR1分析表如下："<<endl;
    cout<<" ";
    for(int i = 0;i<vec_elem.size();i++)
    {
        if(vec_elem[i]>='A'&&vec_elem[i]<='Z') continue;
        else
        cout<<setw(3)<<vec_elem[i];
    }
    for(int i = 0;i<vec_elem.size();i++)
    {
        if(vec_elem[i]>='A'&&vec_elem[i]<='Z')
        cout<<setw(3)<<vec_elem[i];
    }
    cout<<endl;
    for(int i = 0;i<cllo;i++)
    {
        cout<<i;
        for(int j = 0;j<vec_elem.size();j++)
        {
            if(vec_elem[j]>='A'&&vec_elem[j]<='Z') continue;
            else
            {
                if(Action[i][vec_elem[j]]) cout<<setw(3)<<Action[i][vec_elem[j]];
                else cout<<setw(3)<<"x";
            }
        }
        for(int j = 0;j<vec_elem.size();j++)
        {
            if(vec_elem[j]>='A'&&vec_elem[j]<='Z')
            {
                if(Goto[i][vec_elem[j]]) cout<<setw(3)<<Goto[i][vec_elem[j]];
                else cout<<setw(3)<<"x";
            }
        }
        cout<<endl;
    }
}
void analyze()
{
    vector<char> char_stack;//符号栈
    char_stack.push_back('#');
    vector<int> state_stack;//状态栈
    state_stack.push_back(0);
    int p = 0;//指示输入串的位置指针
    int top_state = 0;
    char top_char;
    cout<<setw(3)<<"步骤"<<setw(10)<<"状态栈"<<setw(10)<<"符号栈"<<setw(10)<<"输入串"<<endl;
    int sum = 0;
    while(1)
    {
        top_state = state_stack.back();
        top_char =inputstring[p];
        if(!Action[top_state][top_char])
        {
            cout<<endl<<"分析失败！"<<endl;
            break;
        }
        int st = Action[top_state][top_char];
        //cout<<st<<endl;
        if(st<0)//表示此时应该进行规约
        {
            st = -st;
            int len = str[st].length()-1;
            top_char = str[st][0];
            for(int i = 0;i<len;i++)
            {
                char_stack.pop_back();//删除规约的字符
                state_stack.pop_back();//删除当前状态
            }
            top_state = state_stack.back();
            top_state = Goto[top_state][top_char];//获取最新的状态
            state_stack.push_back(top_state);//将goto的状态压栈
            char_stack.push_back(top_char);//将规约的字符压栈
            int a=0;
            string b;
            for(int i = 0;i<state_stack.size();i++)
                a = a*10+state_stack[i];
            for(int i = 0;i<char_stack.size();i++)
                b += char_stack[i];
            cout<<setw(3)<<sum++<<setw(10)<<a<<setw(10)<<b<<setw(10)<<inputstring.substr(p)<<endl;
        }
        else{
            if(st==99)
            {
                cout<<"ACCEPT!"<<endl;
                break;
            }
            state_stack.push_back(st);
            char_stack.push_back(top_char);
            int a=0;
            string b;
            for(int i = 0;i<state_stack.size();i++)
            {
                a = a*10+state_stack[i];
            }
            for(int i = 0;i<char_stack.size();i++)
                b += char_stack[i];
            cout<<setw(3)<<sum++<<setw(10)<<a<<setw(10)<<b<<setw(10)<<inputstring.substr(p)<<endl;
            p++;
        }
    }
}
int main()
{
    reads();
    closure();
    int n = 0;
    cout<<canbedo();
    get_first();
    get_follow();
    for(int j = 0;j<vec_elem.size();j++)
        cout<<vec_elem[j];
    for(int i = 0;i<cllo;i++)
    {
        //cout<<endl;
        for(int j = 0;j<vec_elem.size();j++)
        {
            if(Go[i][vec_elem[j]])
            cout<<i<<"-->"<<vec_elem[j]<<"  to  "<<Go[i][vec_elem[j]]<<endl;
        }
    }

    if(canbedo())//建立分析表
    {
        cout<<"可以采用LR0分析："<<endl;
        lr0();
    }
    else{
        cout<<"不能使用LR0分析："<<endl;
        slr1();
    }
    cout<<endl;
    /*for(int i = 0;i<cllo;i++)
    {
        cout<<i<<":";
        for(int j = 0;j<vec_before[i].size();j++)
        cout<<vec_before[i][j];
        cout<<endl;
    }*/
    while(1)
    {
        cout<<"请输入符号串：";
        cin>>inputstring;
        inputstring += '#';
        analyze();//开始用建立的分析表进行分析
    }

}
