#include <iostream>
#include<map>
#include <string>
#include<cstring>
#include<stdio.h>
#include <utility>
#include<algorithm>
#include<vector>
using namespace std;
vector<string> interins;
vector<string> ariins;
map <string, string> arithin;
bool equalvec(vector<int>::iterator itb,vector<int>::iterator ite)
{
    int num=*itb;
    vector<int>::iterator it=itb;
    while(it!=ite)
    {
	if((*it)!=num)
	    return false;
	it++;
    }
    return true;
}
void init()
{
/*initialization the arithin*/
    arithin["add"]="+";
    arithin["sub"]="-";
    arithin["mul"]="*";
    arithin["div"]="/";
    arithin["mod"]="%";
    arithin["cmple"]="<=";
    arithin["cmpeq"]="==";
    arithin["cmplt"]="<";
    ariins.push_back("cmple");
    ariins.push_back("cmpeq");
    ariins.push_back("cmplt");
    ariins.push_back("add");
    ariins.push_back("sub");
    ariins.push_back("mul");
    ariins.push_back("div");
    ariins.push_back("mod");
}
/*string process function*/
typedef struct Definition
{
    int num;
    string name;
    string value;
    bool isconst;
    vector<int> kill;
}Def;

bool iscons(string value)
{
    for(unsigned i=0;i!=value.size();i++)
    {
	if((value[i]>='a'&&value[i]<='z')||(value[i]>='A'&&value[i]<='Z')||value[i]=='('||value[i]==')')
	    return false;
    }
    return true;
}
int str2int(string str)
{
    int i;
    char *pp=new char[str.size()+1];
    strcpy(pp,str.c_str());
    sscanf(pp,"%d",&i);
    delete[] pp;
    return i;
}

string int2str(int num)
{
    char buffer[20];
    sprintf(buffer,"%d",num);
    string s(buffer);
    return s;
}
bool strisint(string code)
{
    for(unsigned i=0;i<code.size();i++)
    {
	if(code[i]<'0'||code[i]>'9')
	    return false;
    }
    return true;
}
bool strhasnochar(string code)
{
    if(code.find("[")==string::npos)
	return true;
    int index=code.find("[");
    for(unsigned i=index;i!=code.size();i++)
    {
	if((code[i]>'a'&&code[i]<'z')||(code[i]>'A'&&code[i]<'Z'))
	    return false;
    }
    return true;
}
int parse_ins_num(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    int num;
    sscanf(p,"%*s %d",&num);
    delete[] p;
    return num;
}
string parse_first_param(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char pparm[20];
    sscanf(p,"%*s %*s %*s %s",pparm);
    string parm=pparm;
    delete[] p;
    return parm;
}
string parse_second_param(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char pparm[20];
    sscanf(p,"%*s %*s %*s %*s %s",pparm);
    string parm=pparm;
    delete[] p;
    return parm;
}
string parse_ins_param(string parm)
{

    if(parm.find("(")!=string::npos)
    {
	parm=parm.substr(parm.find("(")+1,parm.find(")")-parm.find("(")-1);
	unsigned num=str2int(parm);
	if(num>interins.size())
        return " ";
	return interins[num-1];
    }
    else
    {
	if(parm=="GP"||parm=="FP")
	{
	    return "--";
	}
    }
    if(parm.find("#")!=string::npos)
    {
	parm=parm.substr(0,parm.find("#"));
	if(parm.find("_base")!=string::npos)
	{
	    parm=parm.substr(0,parm.find("_base"));
	}
	return parm;
    }
    return parm;
}

string parsemove(string code)
{
    string param1=parse_ins_param(parse_first_param(code));
    string param2=parse_ins_param(parse_second_param(code));
    return param2+"="+param1;
}
string parseari(string code,string op)
{
    op=arithin[op];
    string param1=parse_ins_param(parse_first_param(code));
    string param2=parse_ins_param(parse_second_param(code));
    if(param1.find("--")!=string::npos)
    {
	param1.replace(param1.find("--"),2,"0");
    }
    if(param2.find("--")!=string::npos)
    {
	param2.replace(param2.find("--"),2,"0");
    }
    return "("+param1+op+param2+")";
}
string parseload(string code)
{
    string param1=parse_ins_param(parse_first_param(code));
    int p_beg=0,p_end;
    while(param1[p_beg]=='(')
	p_beg++;
    p_end=p_beg;
    while(param1[p_end]!='+')
	p_end++;
    string name=param1.substr(p_beg,p_end-p_beg);
    if(param1.find("--")!=string::npos)
    {
	return name;
    }
    param1.replace(p_beg,p_end-p_beg,"0");
    return name+"["+param1+"/8"+"]";
}
string parsestore(string code)
{
    string param1=parse_ins_param(parse_first_param(code));
    string param2=parse_ins_param(parse_second_param(code));
    int p_beg=1,p_end;
    while(param2[p_beg]=='(')
	p_beg++;
    p_end=p_beg;
    while(param2[p_end]!='+')
	p_end++;
    string name=param2.substr(p_beg,p_end-p_beg);
    if(param2.find("--")!=string::npos)
    {
	return name+"="+param1;
    }
    param2.replace(p_beg,p_end-p_beg,"0");
    return name+"["+param2+"/8"+"]="+param1;
}
string parse_ins_op(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char op[20];
    sscanf(p,"%*s %*s %s",op);
    string oop=op;
    delete[] p;
    return oop;
}

int parse_bl(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char jump[20];
    sscanf(p,"%*s %*s %*s %*s %s",jump);
    string jjump=jump;
    jjump=jjump.substr(1,jjump.size()-2);
    delete[] p;
    return str2int(jjump);
}

int parse_br(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char jump[20];
    sscanf(p,"%*s %*s %*s %s",jump);
    string jjump=jump;
    jjump=jjump.substr(1,jjump.size()-2);
    delete[] p;
    return str2int(jjump);
}
/*vector if two block have same int*/
bool exist_block(vector<int> a,vector<int> b)
{
    vector<int>::iterator it_a=a.begin();
    vector<int>::iterator it_b=b.begin();
    while(it_a!=a.end()&&it_b!=b.end())
    {
	if(*(it_a)==*(it_b))
	    return true;
	else
	    if(*(it_a)>*(it_b))
		it_b++;
	    else
		it_a++;
    }
    return false;
}
/*merge two vector*/
vector<int> merge_region(vector<int> a,vector<int> b)
{
    for(vector<int>::iterator it=b.begin();it!=b.end();it++)
    {
	a.push_back(*it);
    }
    a.resize(unique(a.begin(),a.end())-a.begin());
    return a;
}
/*test if an int exist in a vector*/
bool exist(int t,const vector<int> a)
{
    for(unsigned i=0;i<a.size();i++)
    {
	if(t==a[i])
	    return true;
    }
    return false;
}
/*sort the scr by size*/
bool scr_sort(const vector<int> a,const vector<int> b)
{
    if(a.size()>b.size())
	return false;
    else
	return true;
}
