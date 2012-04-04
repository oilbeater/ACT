#include <iostream>
#include<algorithm>
#include <string>
#include<vector>
#include<utility>
#include<stack>
#include<map>
#include"init1.h"
#include<stdio.h>
#include<cstring>
using namespace std;

/*Find the beging and end of each function block*/

bool findfuncbeg(string buf)
{
    if(buf.find(": enter")!=string::npos)
        return true;
    else
        return false;
}

bool findfuncend(string buf)
{
    if(buf.find(": ret")!=string::npos)
        return true;
    else 
        return false;
}
/*
Parse the 3-address code to mediate
parse parment
parse arith,move,login

*/
int main()
{
    int paralindex;//place to enter paraml
    string buf;
    vector< pair<int,int> > func;//Store the function block range
    stack<int> funsb;
    
    init();
    
    /*Store the 3-address code ande seperate the code*/
    while(getline(cin,buf))
    {
        code3add.push_back(buf);
        if(findfuncbeg(buf))
	    funsb.push(code3add.size());
	if(findfuncend(buf))
	    {
	        func.push_back(make_pair(funsb.top(),code3add.size()));
		funsb.pop();
	    }   
    }
 int i=-1;//the index of function
 int mm=0;
 int goindex=0;
   for(vector<string>::iterator it=code3add.begin();it!=code3add.end();it++)
   {
       /*if(it-code3add.begin()==go-1)
       {
           mediate[mediate.size()-1]+="}";
       }*/
       if((*it).find("nop")!=string::npos)
       {
           mediate.push_back("");
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }
       if((*it).find("entrypc")!=string::npos)
       {
           mediate.push_back("void main(){");
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   it++;
	   mediate.push_back("");
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   i++;
	   space=0;
           continue;
       }
       if((*it).find("enter")!=string::npos)
       {
           i++;
           mediate.push_back(parseenter(*it));
	   string code=code3add[func[i].second-1];
           char *p=new char[code.size()+1];
           strcpy(p,code.c_str());
   	   char src[20];
           sscanf(p,"%*s %*s %*s %s",src);
           string ssrc=src;
           ssrc=parsepar(ssrc);
           space=str2int(ssrc);
	   continue;
       }
       if((*it).find("ret")!=string::npos)
       {
           mediate.push_back("}");
//	   cout<<"come here"<<endl;
	   for(goindex;goindex!=go.size();goindex++)
	   {
	       if( mediate[go[goindex]-2].find("else")==string::npos)
	       {
	       mediate[go[goindex]-2]+="}";
//	       cout<<go[goindex]-2<<"plus a }"<<endl;
//	       cout<<mediate[go[goindex]-2]<<endl;
	       }
	       else
	       {
               mediate[go[goindex]-2]="}"+ mediate[go[goindex]-2];
	       }
	   }
           string funcp="";
	   space=space/8;
	   if(i<func.size())
	   {for(int m=0;m<space;m++)
	   {
	       funcp+="long "+param[m]+",";
	   }
	   funcp=funcp.substr(0,funcp.size()-1);
	   funcp+="){";
	   mediate[func[i].first-1]+=funcp;
	   for(mm;mm!=mediate.size();mm++)
	   {
	       if(selectcode(code3add[mm]))
	           {
		   finalcode.push_back(mediate[mm]);
		   }
	       else
	          if(code3add[mm].find("entrypc")!=string::npos)
		  {
		      finalcode.push_back(mediate[mm]);
		      paralindex=finalcode.size();
		      mm++;
		  }else
		  if(code3add[mm].find("enter")!=string::npos)
		  {
		      finalcode.push_back(mediate[mm]);
		      paralindex=finalcode.size();
		  }

	   }
           sort(paraml.begin(),paraml.end(),paramegt);
           vector<pair<string,int> >::iterator ittt=unique(paraml.begin(),paraml.end(),paramequal);
	    paraml.resize(ittt-paraml.begin());

            int prel=l_high;
            for(vector<pair<string,int> >::iterator it=paraml.begin();it!=paraml.end();it++)
            {
               if(prel-(*it).second==8)
               {
	           string temp="long "+(*it).first+";";
		   finalcode.insert(finalcode.begin()+paralindex,temp);
               }
               else
               {
	           string temp; 
                   int arrynuml=(prel-(*it).second)/8;
		   temp="long "+(*it).first+"["+int2str(arrynuml)+"];";
	           finalcode.insert(finalcode.begin()+paralindex,temp);
       }
       prel=(*it).second;
   }   
	   /*for(int mmm=0;mmm!=paraml.size();mmm++)
	   {
	       string temp="long ";
	       temp+=paraml[mmm].first+";";
	       finalcode.insert(finalcode.begin()+paralindex,temp);
	   }*/
	   paraml.clear();
	   	   }

//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }
       if((*it).find("move")!=string::npos)
       {
           mediate.push_back(parsemove(*it));
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }
       if((*it).find("blbc")!=string::npos||(*it).find("blbs")!=string::npos)
       {
           mediate.push_back(parselog(*it));
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }
       if((*it).find("br")!=string::npos)
       {
           mediate.push_back(parsebr(*it));
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }
       if((*it).find("load")!=string::npos)
       {
           mediate.push_back(parseload(*it));
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }
       if((*it).find("store")!=string::npos)
       {
           mediate.push_back(parsestore(*it));
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }
       if((*it).find("write")!=string::npos)
       {
           mediate.push_back(outputx(*it));
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }
       if((*it).find("wrl")!=string::npos)
       {
           mediate.push_back("WriteLine();");
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }
       if((*it).find("param")!=string::npos)
       {
           mediate.push_back(parseparf(*it));
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }
       if((*it).find("call")!=string::npos)
       {
           mediate.push_back(parsecall(*it));
//	   cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
	   continue;
       }

      mediate.push_back(parseari(*it));
//     cout<<mediate.size()<<":  "<<mediate[mediate.size()-1]<<endl;
   }
   /*for(vector<int>::iterator it=square.begin();it!=square.end();it++)
   {
       mediate[(*it)-1]="}"+mediate[(*it)-1];
       cout<<"instr"<<(*it)<<" is "<<mediate[(*it)-1]<<endl;
   }*/
   dfc();
   sort(paramg.begin(),paramg.end(),paramegt);
   vector<pair<string,int> >::iterator itt=unique(paramg.begin(),paramg.end(),paramequal);
	    paramg.resize(itt-paramg.begin());
   int pre=g_high;
   for(vector<pair<string,int> >::iterator it=paramg.begin();it!=paramg.end();it++)
   {
      if(pre-(*it).second==8)
       {
           cout<<"long "<<(*it).first<<";"<<endl;
	   pre=(*it).second;
       }
       else
       {
           int arrynum=(pre-(*it).second)/8;
	   cout<<"long "<<(*it).first<<"["<<arrynum<<"]"<<";"<<endl;
       }
   }
  for(vector<string>::iterator it=finalcode.begin();it!=finalcode.end();it++)
   {
       cout<<(*it)<<endl;
   }
 /*//Test mediate expression
   for(vector<string>::iterator it = mediate.begin();it!=mediate.end();it++)
   {
       cout<<"instr "<<it-mediate.begin()+1<<"is   "<<(*it)<<endl;
   }
 /*  
   for(vector<string>::iterator it = paramg.begin();it!=paramg.end();it++)
   {
       cout<<(*it)<<endl;
   }:w

   */
    return 0;
}
