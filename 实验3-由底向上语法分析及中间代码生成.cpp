#include<iostream>
#include<vector>
#include<map>
#include <iomanip>
using namespace std;
char AA = '@';
string inputstring;
string instr;
string str[20];//str[0]�����ع㣬�±�Ϊ0������ʾ����ʽ���󲿡�
vector<string> vec_str;//�洢���еĻ�ǰ׺��
vector<string> vec_closure[500];//�����洢ĳ��״̬��lr0��Ŀ��closure
vector<char> vec_elem;//������¼ÿһ��closure��@֮���һ��Ԫ��
vector<int> vec_num;
vector<int> vec_state;
vector<int> vec_before[20];
map<int,map<char,int> >Action;//�����洢nfa״̬ת��ͼ
map<int,map<char,int> >Goto;//������¼goto
map<int,map<char,int> >Go;
map<char,string> first;
map<char,string> follow;
int cllo = 0;
int num = 0;//�����洢�ع�����ʽ�ĸ���
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
    cout<<"���������ʽ�ĸ�����";
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

    cout<<"������ķ������ع�󣨵�һ����ĸΪ�ķ��󲿣���"<<endl;
    for(int i = 0;i<num;i++)
    cout<<str[i]<<endl;
    cout<<num<<"������Ŀ"<<endl;
    for(int i = 0;i<vec_elem.size();i++)
    cout<<vec_elem[i];
    cout<<endl;
}
int canbedo()
{
    for(int i = 0;i<cllo;i++)
    {
        int flag1 = 0;//�ƽ���
        int flag2 = 0;//��Լ��
        for(int j = 0;j<vec_closure[i].size();j++)
        {
            int len = vec_closure[i][j].size()-1;
            if(vec_closure[i][j][len]==AA)
            {
                if(!flag2&&!flag1)
                {
                    flag2 = 1;
                }
                else return 0;//�ж����Լ�����ͬʱ���ڹ�Լ���ƽ�
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
        for(int i = vec_str.size()-1;i>=0;i--)//�����һ���ַ�����ʼ����
        {
            if(vec_str[i][1]>='A'&&vec_str[i][1]<='Z')//��һ���Ƿ��ս��
            {
                for(int j = 0;j<first[vec_str[i][1]].size();j++)//�����ս����first���Ϸŵ�ĿǰԪ�ص�first�����С�
                {
                    char a = first[vec_str[i][1]][j];
                    if(!inThisfirst(vec_str[i][0],a))
                    {
                        first[vec_str[i][0]] += a;
                        flag = 1;
                    }
                }
            }
            else{//��һ�����ս��
                if(!inThisfirst(vec_str[i][0],vec_str[i][1]))
                {
                    first[vec_str[i][0]] += vec_str[i][1];
                    flag = 1;
                }
            }
        }
    }
    cout<<endl<<"first���ϣ�"<<endl;
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
                if(vec_str[i][j]>='A'&&vec_str[i][j]<='Z')//�ҵ�һ�����ս��
                {
                    char ch = vec_str[i][j];
                    char fch = vec_str[i][0];
                    if(j==maxlen)//���Ǹó��Ե����һ�����ţ����ս����
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
                    else if(vec_str[i][j+1]>='A'&&vec_str[i][j+1]<='Z')//����������ս��֮�����һ�����ս��
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
                        char ch = vec_str[i][j+1];//��ʱ�÷��ս��֮���ֵΪ�ս��
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
    cout<<"follow����"<<endl;
    for(int i = 0;i<vec_elem.size();i++)
    {
        if(vec_elem[i]>='A'&&vec_elem[i]<='Z')
        {
            cout<<vec_elem[i]<<":"<<follow[vec_elem[i]]<<endl;
        }
    }
}
int findx(char a)//������Ŀ�±�
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
            vec_str.push_back(ss);//��ȡ���еĻ�ǰ׺��
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
void toadd(int now,int i)//�����ַ������乹��
{
        for(int j = 0;j<vec_closure[now][i].size();j++)//��������һ���ַ������ַ����ж��ٵ��ַ�
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
        //cout<<"��ʼ��"<<now<<" "<<cllo<<endl;
        //���ع㣬�ڷֱ�Ը�����д��������ǽ���nfa�Ĺ���
        for(int i = 0;i<vec_closure[now].size();i++)
        {
            toadd(now,i);
        }
        cout<<"��չ���"<<endl;//�ȹ���õ�ǰ��Ŀ��
        for(int n = 0;n < vec_elem.size();n++)//��һ��������ĳһ��״̬��ѭ��
        {
            int flag = 1;
            char elem = vec_elem[n];
            for(int i = 0;i<vec_closure[now].size();i++)//������ǰ״̬�������ַ���
            {
                int len = vec_closure[now][i].length()-1;
                if(vec_closure[now][i][len]=='@'&&now!=1)//������¼��Լ����
                {
                    int ng = find_num(vec_closure[now][i].substr(0,len));
                    if(notinnum(ng))
                    {
                        vec_num.push_back(ng);
                        vec_state.push_back(now);
                    }
                }
                for(int j = 0;j<vec_closure[now][i].size();j++)//��������һ���ַ������ַ����ж��ٵ��ַ�
                {
                    if(vec_closure[now][i][j]=='@')
                    {
                        if(vec_elem[n]==vec_closure[now][i][j+1])
                        {
                            int k = findstr(vec_closure[now][i])+1;//��vec_str���ҵ����ַ����������һ������Ϊ@����һλ��
                            if(k<vec_str.size())
                            {
                                if(now==4&&elem=='E')
                                {
                                    //cout<<vec_str[k]<<"qqqqqqqqqqqqqqqqqqqqqqqq"<<endl;
                                }
                                if(inclosure(vec_str[k])==-1)//��һ��,��������״̬�ж�û���ҵ���
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
                                else{//������������Ŀ�����е��������������Ŀ���ҵ���
                                    if(!flag) vec_closure[cllo].push_back(vec_str[k]);
                                    string a = vec_str[k];
                                    for(int i = 0;i<=cllo;i++)//��������Ŀ���Ҹ��ַ���
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
    cout<<endl<<"LR0���������£�"<<endl;
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
    cout<<endl<<"SLR1���������£�"<<endl;
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
    vector<char> char_stack;//����ջ
    char_stack.push_back('#');
    vector<int> state_stack;//״̬ջ
    state_stack.push_back(0);
    int p = 0;//ָʾ���봮��λ��ָ��
    int top_state = 0;
    char top_char;
    cout<<setw(3)<<"����"<<setw(10)<<"״̬ջ"<<setw(10)<<"����ջ"<<setw(10)<<"���봮"<<endl;
    int sum = 0;
    while(1)
    {
        top_state = state_stack.back();
        top_char =inputstring[p];
        if(!Action[top_state][top_char])
        {
            cout<<endl<<"����ʧ�ܣ�"<<endl;
            break;
        }
        int st = Action[top_state][top_char];
        //cout<<st<<endl;
        if(st<0)//��ʾ��ʱӦ�ý��й�Լ
        {
            st = -st;
            int len = str[st].length()-1;
            top_char = str[st][0];
            for(int i = 0;i<len;i++)
            {
                char_stack.pop_back();//ɾ����Լ���ַ�
                state_stack.pop_back();//ɾ����ǰ״̬
            }
            top_state = state_stack.back();
            top_state = Goto[top_state][top_char];//��ȡ���µ�״̬
            state_stack.push_back(top_state);//��goto��״̬ѹջ
            char_stack.push_back(top_char);//����Լ���ַ�ѹջ
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

    if(canbedo())//����������
    {
        cout<<"���Բ���LR0������"<<endl;
        lr0();
    }
    else{
        cout<<"����ʹ��LR0������"<<endl;
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
        cout<<"��������Ŵ���";
        cin>>inputstring;
        inputstring += '#';
        analyze();//��ʼ�ý����ķ�������з���
    }

}
