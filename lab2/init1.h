#include<utility>
#include<algorithm>
#include<cstring>
#include<stdio.h>
#include <map>
#include<vector>
#include<string>
using namespace std;
const long g_high=32768;
const long l_high=0;
vector<string> code3add;//Store the 3-address code
vector<string> mediate;//The value of each 3-address code
vector<int> square;//note the place to insert "}"
vector<pair<string,int> > paraml;//record the param
vector<string> paramf;//function call varible
vector<pair<string,int> > paramg;//global varible
string param[100];//function defition varible
map <string, string> arithin;
map <string, string> login;
int funcnum;//the index of function
int space;//the space of function param
vector<string> finalcode;
vector <int> go;//else part
void init()
{
/*initialization the arithin*/
    arithin["add"]="+";
    arithin["sub"]="-";
    arithin["mul"]="*";
    arithin["div"]="/";
    arithin["mod"]="%";
    arithin["neg"]="!=";
    arithin["cmpeq"]="==";
    arithin["cmple"]="<=";
    arithin["cmplt"]="<";
    login["blbc"]="!=0";
    login["blbs"]="==0";
}
string parseadd(string code)
{
    string name;
    int s_beg,s_end;
    for(string::iterator it=code.begin();it!=code.end();it++)
    {
        if((*it)!='(')
	{
	    s_beg=it-code.begin();
	    break;
	}
    }
    for(string::iterator it=code.begin();it!=code.end();it++)
    {
        if((*it)=='+')
	{
	    s_end=it-code.begin();
	    break;
	}
    }
    name=code.substr(s_beg,s_end-s_beg);
    code.replace(code.begin()+s_beg,code.begin()+s_end,"0");
    return name+"["+code+"/8]";

}
bool paramequal(pair<string,int> i,pair<string,int> j)
{
    return(i.first==j.first);
}
bool paramegt(pair<string,int> i,pair<string,int> j)
{
    return(i.second>j.second);
}
int str2int(string str)
{
    int i;
    char *pp=new char[str.size()+1];
    strcpy(pp,str.c_str());
    sscanf(pp,"%d",&i);
    return i;

}
string int2str(int num)
{
    char buffer[20];
    sprintf(buffer,"%d",num);
    string s(buffer);
    return s;
}
string parsepar(string par)
{
    int index;
    //find varity
    if(par.find("#")!=string::npos)
    {
        index=par.find("#");
	int address=str2int(par.substr(index+1));
	string paro,pare;
	if(par.find("_base")!=string::npos&&par.find("-")==string::npos)
	{
	    index=par.find("_base");
	    paro=par.substr(0,index);
	    paramg.push_back(make_pair(paro,address));
	    	}
	paro=par.substr(0,index);
	if(par.find("-")!=string::npos)
	{
	    {
	        paraml.push_back(make_pair(paro,address));
		unique(paraml.begin(),paraml.end(),paramequal);
	    }    
	}
	else
	{
	    pare=par.substr(par.find("#")+1);
	    if(str2int(pare)<=space+8)
	    {
	        int pos=(space+8-str2int(pare))/8;
	        param[pos]=paro;
	    }
	}

        return paro;
    }
    else
    //find the expression
        if(par.find("(")!=string::npos)
	{
	    string num=par.substr(par.find("(")+1,par.find(")")-par.find("(")-1);
	    int i;
	    char *p=new char[num.size()+1];
	    strcpy(p,num.c_str());
	    sscanf(p,"%d",&i);
	    return mediate[i-1];
	}
       else
       //find the location to jump
            if(par.find("[")!=string::npos)
       	{
	    string num=par.substr(par.find("[")+1,par.find("]")-par.find("(")-2);
	    return num;
	}
      else
      if(par=="GP")
          return "GP";

    return par;
}
/*translate the arithmatic expression to mediate expression*/
string parseari(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char op[20],par1[20],par2[20];
    sscanf(p,"%*s %*s %s %s %s",op,par1,par2);
    string oop=op,ppar1=par1,ppar2=par2;
    map<string,string>::iterator it=arithin.find(oop);
   if(it!=arithin.end())
        oop=it->second;
    ppar1=parsepar(ppar1);
    ppar2=parsepar(ppar2);
    delete[] p;
    if(oop=="+"&&(ppar2=="GP"||ppar2=="FP"))
    {
        return ppar1;
    }
    return "("+ppar1+oop+ppar2+")";
}
/*translate the move to C expression*/
string parsemove(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char src[20],des[20];
    sscanf(p,"%*s %*s %*s %s %s",src,des);
    string ssrc=src,ddes=des;
    ssrc=parsepar(ssrc);
    ddes=parsepar(ddes);
    delete[] p;
    return ddes+"="+ssrc+";";
}
/*parse the load expression*/
string parseload(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char src[20];
    sscanf(p,"%*s %*s %*s %s",src);
    string ssrc=src;
    ssrc=parsepar(ssrc);
    if(ssrc.find("+")==string::npos)
    return ssrc;
    else
    {
       /* ssrc=ssrc.substr(1,ssrc.size()-2);
        string t1,t2;
	t1=ssrc.substr(0,ssrc.find("+"));
	t2=ssrc.substr(ssrc.find("+")+1);
        t2="[("+t2+")/8]";*/
	return parseadd(ssrc);

    }
}

/*parse the store expression*/
string parsestore(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char par1[20],par2[20];
    sscanf(p,"%*s %*s %*s %s %s",par1,par2);
    string ppar1=par1,ppar2=par2;
    ppar1=parsepar(ppar1);
    ppar2=parsepar(ppar2);
    if(ppar2.find("+")==string::npos)
        {return(ppar2+"="+ppar1+";");}
    else
    {
       /* ppar2=ppar2.substr(1,ppar2.size()-2);
        string t1,t2;
	t1=ppar2.substr(0,ppar2.find("+"));
	t2=ppar2.substr(ppar2.find("+")+1);
        t2="[("+t2+")/8]";*/
	return parseadd(ppar2)+"="+ppar1+";";
    }


}
string parselog(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char op[20],par1[20],par2[20];
    sscanf(p,"%*s %*s %s %s %s",op,par1,par2);

    string oop=op,ppar1=par1,ppar2=par2;
    map<string,string>::iterator it=login.find(oop);
   if(it!=arithin.end())
        oop=it->second;
    ppar1=parsepar(ppar1);
    ppar2=parsepar(ppar2);
    delete[] p;
    
    int i=str2int(ppar2);
    square.push_back(str2int(ppar2));

    go.push_back(i);
    if(code3add[i-2].find("br")!=string::npos)
    {
        char *m=new char[code3add[i-2].size()+1];
	strcpy(m,code3add[i-2].c_str());
	char jp[20];
	sscanf(m,"%*s %*s %*s %s",jp);
	string jjp=jp;
	jjp=parsepar(jjp);
	delete[] m;
	if(str2int(jjp)<=mediate.size())
	    return("while("+ppar1+oop+"){");
    }
    return "if("+ppar1+oop+"){";

}
/*parse enter*/
string parseenter(string code)
{
    funcnum++;
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char src[20],temp[20];
    sprintf(temp, "%d", funcnum);  
    sscanf(p,"%*s %*s %*s %s",src);
    string ssrc=src;
    ssrc=parsepar(ssrc);
    space=str2int(ssrc);
    string s(temp);
    string term= "void f"+s+"(";
    return term;
}
string parseret(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char src[20];
    sscanf(p,"%*s %*s %*s %s",src);
    string ssrc=src;
    ssrc=parsepar(ssrc);
    space=str2int(ssrc);
}
/*find while loop*/
string parsebr(string code)
{
    int intr;
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char src[20];
    sscanf(p,"%*s %d %*s %*s %s",&intr,src);
    string ssrc=src;
    ssrc=parsepar(ssrc);
    if(str2int(ssrc)<=intr)
    {
        return "";
    }
    else
    {
        go.push_back(str2int(ssrc));
        return "else{";
    }
}
/*output transfer*/
string outputx(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char src[20];
    sscanf(p,"%*s %*s %*s %s",src);
    string ssrc=src;
    ssrc=parsepar(ssrc);
    return("WriteLong("+ssrc+")"+";");
}
/*function call related parseparf,parsecall*/
string parseparf(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char src[20];
    sscanf(p,"%*s %*s %*s %s",src);
    string ssrc=src;
    ssrc=parsepar(ssrc);
    paramf.push_back(ssrc);
    return "";
}
string parsecall(string code)
{
    char *p=new char[code.size()+1];
    strcpy(p,code.c_str());
    char src[20];
    sscanf(p,"%*s %*s %*s %s",src);
    delete[] p;
    string ssrc=src;
    ssrc=parsepar(ssrc);
    string term=mediate[str2int(ssrc)-1];
    term=term.substr(5,term.find("(")-4);
    for(vector<string>::iterator it=paramf.begin();it!=paramf.end();it++)
    {
        term+=*it;
	if(it!=paramf.end()-1)
	{
	    term+=",";
	}
    }
    term+=");";
    paramf.clear();
    return term;
}
/*output select*/
bool selectcode(string code)
{
    if(code.find("br")!=string::npos||code.find("blbc")!=string::npos||code.find("blbs")!=string::npos||code.find("call")!=string::npos||code.find("move")!=string::npos||code.find("store")!=string::npos||code.find("write")!=string::npos||code.find("wrl")!=string::npos||code.find("ret")!=string::npos)
    return true;
}
/*default cout*/
void dfc()
{
    cout<<"#include<stdio.h>"<<endl;
    cout<<"#define WriteLine() printf(\"\\n\");  "<<endl;
    cout<<"#define WriteLong(x) printf(\" %lld\", (long)x);   "<<endl;
    cout<<"#define long long long"<<endl;
    cout<<"#define ReadLong(a) if (fscanf(stdin, \"%lld\", &a) != 1) a = 0;"<<endl;
}
