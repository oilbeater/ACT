#include <iostream>
#include <string>
#include<stack>
#include<cstring>
#include<stdio.h>
#include <utility>
#include<algorithm>
#include<vector>
#include<map>
#include<set>
#include"init.h"
using namespace std;
/*for each function set up a CFG when encounter an "enter" and
 * output the result when encounter a "ret" clear the struct 
 * cfg_start for the next function*/
int main()
{
    init();
    vector<int> cfg_start;
    vector<string> code3add;
    string buf;
    map<int,Def> def_t;
    set<string> defname;
    /*store the 3-add-code*/
    while(getline(cin,buf))
    {
	code3add.push_back(buf);
    }
    /*generate the interins for next use*/
    for(vector<string>::iterator it=code3add.begin();it!=code3add.end();it++)
    {
	
	string op=parse_ins_op(*(it));
	if(op=="move")
        {
            interins.push_back(parsemove(*it));
	    continue;
        }
        if(op=="load")
        {
            interins.push_back(parseload(*it));
	    continue;
        }
        if(op=="store")
        {
   	    interins.push_back(parsestore(*it));
	    continue;
        }
        if(find(ariins.begin(),ariins.end(),op)!=ariins.end())
        {
            interins.push_back(parseari((*it),op));
	    continue;
        }
        interins.push_back("");
     }
     /*first loop to find all defition*/
    for(vector<string>::iterator it=interins.begin();it!=interins.end();it++)
    {
	if((*it).find("=")!=string::npos&&(*it).find("==")==string::npos&&(*it).find("!=")==string::npos)
	{
	    Def def;
	    def.num=it-interins.begin()+1;
	    def.name=(*it).substr(0,(*it).find("="));
	    def.value=(*it).substr((*it).find("=")+1);
	    def.isconst=iscons(def.value);
	    def_t.insert(pair<int,Def>(def.num,def));
		defname.insert(def.name);
	}
    }
    for(map<int,Def>::iterator it=def_t.begin();it!=def_t.end();it++)
    {
	for(map<int,Def>::iterator itt=def_t.begin();itt!=def_t.end();itt++)
	{
	    if((*it).second.name==(*itt).second.name&&(*it).second.num!=(*itt).second.num)
	    {
		(*it).second.kill.push_back((*itt).second.num);
	    }
	}
	sort((*it).second.kill.begin(),(*it).second.kill.end());
    }
    /*insert the start point of each basic block 
     * when encounter enter,blbc,blbs,br,call*/
    for(vector<string>::iterator it=code3add.begin();it!=code3add.end();it++)
    {
        string op=parse_ins_op(*it);
	if(op=="enter")
	{
            cfg_start.push_back(it-code3add.begin()+1);
	    continue;
	}
	if(op=="blbc"||op=="blbs")
	{
	    cfg_start.push_back(parse_bl(*it));
	    cfg_start.push_back(it-code3add.begin()+2);
	    continue;
	}
	if(op=="br")
	{
	    cfg_start.push_back(parse_br(*it));
	    cfg_start.push_back(it-code3add.begin()+2);
	    continue;
	}
	if(op=="call")
	{
	    cfg_start.push_back(it-code3add.begin()+2);
	    continue;
	}
	/*the edge only linked the last instr of one basic block,
	 * so just take care of the last instr of each basic block,
	 * the last basic block edge is omit due it has no edge*/
	if(op=="ret")
	{
	    sort(cfg_start.begin(),cfg_start.end());
	    cfg_start.resize(unique(cfg_start.begin(),cfg_start.end())-cfg_start.begin());
            vector<vector<int> > cfg_goto;
	    for(vector<int>::iterator it=cfg_start.begin();it!=cfg_start.end()-1;it++)
            {
		int offset=*(it+1)-1;
		string code=*(code3add.begin()+offset-1);
		string op=parse_ins_op(code);
	        vector<int> go;
		if(op=="blbc"||op=="blbs")
		{
		    go.push_back(parse_bl(code));
		    go.push_back(*(it+1));
		    cfg_goto.push_back(go);
		    continue;
		}
		if(op=="br")
		{
		    go.push_back(parse_br(code));
                    cfg_goto.push_back(go);
		    continue;
		}
		go.push_back(*(it+1));
		cfg_goto.push_back(go);
	    }
	    
	    /*putout the cfg result of one function*/
	   /* cout<<"Function: "<<cfg_start[0]<<endl;
	    cout<<"Basic blocks:";
	    for(int i=0;i<cfg_start.size();i++)
		cout<<" "<<cfg_start[i];
	    cout<<endl;
	    cout<<"CFG:"<<endl;*/
	    for(int i=0;i<cfg_start.size()-1;i++)
	    {
		//cout<<cfg_start[i]<<" ->";
		sort(cfg_goto[i].begin(),cfg_goto[i].end());
		/*for(int j=0;j<cfg_goto[i].size();j++)
		{
		    cout<<" "<<cfg_goto[i][j];
		}
		cout<<endl;*/
	    }
	    //cout<<cfg_start[cfg_start.size()-1]<<" ->"<<endl;//add the last basic block edge

	    /*analyize the SCR*/
	    vector<vector<int> > scr;
	    for(int i=0;i!=cfg_start.size()-1;i++)
	    {
		for(int j=0;j<cfg_goto[i].size();j++)
		{
		    if(cfg_start[i]>cfg_goto[i][j])
		    {
			vector<int> temp_scr;
			int min=cfg_goto[i][j];
			stack<int> s_scr;
			temp_scr.push_back(cfg_start[i]);
			temp_scr.push_back(cfg_goto[i][j]);
                        s_scr.push(cfg_start[i]);
			while(!s_scr.empty())
			{
			    int bottom=s_scr.top();
			    s_scr.pop();
			    for(int k=0;k<cfg_start.size()-1;k++)
			    {
			        for(int l=0;l<cfg_goto[k].size();l++)
				{
				    if(bottom==cfg_goto[k][l]&&!exist(cfg_start[k],temp_scr))
				    {
					s_scr.push(cfg_start[k]);
					temp_scr.push_back(cfg_start[k]);
				    }
				}
			    }
			}
			sort(temp_scr.begin(),temp_scr.end());
			scr.push_back(temp_scr);
		      }
		}
	    }
	    sort(scr.begin(),scr.end(),scr_sort);
	    if(!scr.empty()){
	    for(vector<vector<int> >::iterator it=scr.begin();it!=scr.end()-1;)
	    {
		for(vector<vector<int> >::iterator itt=it+1;itt!=scr.end();itt++)
		{
		    if(exist_block(*it,*itt))
		    {
			scr.erase(it);
			it=scr.begin();break;
		    }
		    else
			it++;
		}
	    }
	    /*cout<<"Function: "<<cfg_start[0]<<endl;
	    cout<<"Basic blocks:";
	    for(int i=0;i<cfg_start.size();i++)
		cout<<" "<<cfg_start[i];
	    cout<<endl;
            cout<<"SCR  :";
	    for(int n=0;n!=scr.size();n++)
	    {
	        for(int q=0;q!=scr[n].size();q++)
	        cout<<" "<<scr[n][q];
	        cout<<endl;
	    }*/
	    }
	    /*reaching definition*/
	    vector<int> cfg_end;
	    for(vector<int>::iterator it=cfg_start.begin();it!=cfg_start.end()-1;it++)
	    {
		if(*(it+1)-(*it)>1)
		    cfg_end.push_back(*(it+1)-1);
		else
		    cfg_end.push_back(*it);
	    }
	    set<int> t;
	    vector<set<int> > in_t(code3add.size(),t);
	    vector<set<int> > out_t(code3add.size(),t);
	    int change=1;
	    while(change==1){
		change=0;
	    for(vector<string>::iterator it=interins.begin()+cfg_start[0]-1;it!=interins.begin()+cfg_start[cfg_start.size()-1];it++)
	    {
		int index=it-interins.begin();
		int out_size=out_t[index].size();
		if((*it).find("=")!=string::npos)
		{
		    out_t[index].insert(index+1);
		}
		for(set<int>::iterator it=in_t[index].begin();it!=in_t[index].end();it++)
		{
		    out_t[index].insert(*it);
		}
		for(int i=0;i<def_t[index+1].kill.size();i++)
		{
		    out_t[index].erase(def_t[index+1].kill[i]);
		}
		if(out_size!=out_t[index].size())
		{
		    change=1;
		}
		if(find(cfg_end.begin(),cfg_end.end(),index+1)==cfg_end.end())
		{
		    if(index+1<in_t.size()-1)
			in_t[index+1]=out_t[index];
		}else{
		for(int i=0;i<cfg_end.size();i++)
		{
		    if(cfg_end[i]==index+1)
		    {
			for(int j=0;j<cfg_goto[i].size();j++)
			{
			    for(set<int>::iterator it=out_t[index].begin();it!=out_t[index].end();it++)
			    {
				in_t[cfg_goto[i][j]-1].insert(*it);
			    }
			}
			break;
		    }
		}
		}
             }
	}
	    /*constant propagation*/
	    int const_use=0;
	    for(vector<string>::iterator it=code3add.begin();it!=code3add.end();it++)
	    {
		string op=parse_ins_op(*it);
                if(find(ariins.begin(),ariins.end(),op)!=ariins.end())
		{
		    set<int> in=in_t[it-code3add.begin()];
		    set<string> use1,use2;
		    string param1=parse_ins_param(parse_first_param(*it));
                    string param2=parse_ins_param(parse_second_param(*it));
		    for(set<int>::iterator itt=in.begin();itt!=in.end();itt++)
		    {
			if(def_t[*itt].name==param1)
			{
			    use1.insert(def_t[*itt].value);
			}
			if(def_t[*itt].name==param2)
			{
			    use2.insert(def_t[*itt].value);
			}
		    }
		    if(use1.size()==1&&strisint(*(use1.begin())))
		    {
			string temp=parse_first_param(*it);
			int s_beg=(*it).find(temp);
			(*it).replace(s_beg,temp.size(),*(use1.begin()));
			const_use++;
		    }
		    if(use2.size()==1&&strisint(*(use2.begin())))
		    {
			string temp=parse_second_param(*it);
			int s_beg=(*it).find(temp);
			(*it).replace(s_beg,(parse_second_param(*it)).size(),*(use2.begin()));
			const_use++;
		    }
		    continue;
		}
		if((op=="move")||(op=="param")||(op=="load")||(op=="store"))
		{
		    set<int> in=in_t[it-code3add.begin()];
		    set<string> use1;
		    string param1=parse_ins_param(parse_first_param(*it));
                    for(set<int>::iterator itt=in.begin();itt!=in.end();itt++)
		    {
			if(def_t[*itt].name==param1)
			{
			    use1.insert(def_t[*itt].value);
			}
		    }
		    if(use1.size()==1&&strisint(*(use1.begin())))
		    {
			string temp=parse_first_param(*it);
			int s_beg=(*it).find(temp);
			(*it).replace(s_beg,temp.size(),*(use1.begin()));
			const_use++;
		    }
		    continue;
		}
	    }
//	    cout<<"Function: "<<cfg_start[0]<<endl;
//	    cout<<"Number of constants propagated: "<<const_use<<endl;
		/*varible liveness analysis*/
/*		set<string> t;
		int in_scr=0,out_scr=0;
		vector<set<string> > in_name(code3add.size(),t);
		vector<set<string> > out_name(code3add.size(),t);
		int change=1;
		while(change==1)
		{
		    change=0;
		    for(int i=cfg_start[cfg_start.size()-1];i!=cfg_start[0]-1;i--)
		    {
			int old_size=in_name[i].size();
		        vector<string>::iterator it=code3add.begin()+i-1;
                        string op=parse_ins_op(*it);
			if(op=="move"||op=="store")
			{
                            string param2=parse_ins_param(parse_second_param(*it));
                            string param1=parse_ins_param(parse_first_param(*it));
			    in_name[i]=out_name[i];
			    in_name[i].erase(param2);
			    if(defname.find(param1)!=defname.end())
			        in_name[i].insert(param1);
			    if(find(cfg_start.begin(),cfg_start.end(),i+1)!=cfg_start.end())
			    {
				for(int j=0;j<cfg_goto.size();j++)
				{
				    if(find(cfg_goto[j].begin(),cfg_goto[j].end(),i+1)!=cfg_goto[j].end())
				    {
					for(set<string>::iterator it=in_name[i].begin();it!=in_name[i].end();it++)
					    out_name[cfg_end[j]-1].insert(*it);
				    }
				}
			    }
			    else
			    {
			        for(set<string>::iterator it=in_name[i].begin();it!=in_name[i].end();it++)
				    out_name[i-1].insert(*it);
			    }
			    /*cout<<i;
			    for(set<string>::iterator it=in_name[i].begin();it!=in_name[i].end();it++)
				cout<<"  "<<*it;
			    cout<<endl;
			    if(old_size!=in_name[i].size())
				change=1;
			    continue;
			}
			if(op=="param"||op=="write"||op=="load")
			{
			    string param1=parse_ins_param(parse_first_param(*it));
			    in_name[i]=out_name[i];
			    if(defname.find(param1)!=defname.end())
			        in_name[i].insert(param1);

			    if(find(cfg_start.begin(),cfg_start.end(),i+1)!=cfg_start.end())
			    {
				for(int j=0;j<cfg_goto.size();j++)
				{
				    if(find(cfg_goto[j].begin(),cfg_goto[j].end(),i+1)!=cfg_goto[j].end())
				    {
					for(set<string>::iterator it=in_name[i].begin();it!=in_name[i].end();it++)
					    out_name[cfg_end[j]-1].insert(*it);
				    }
				}
			    }
			    else
			    {
			        for(set<string>::iterator it=in_name[i].begin();it!=in_name[i].end();it++)
				    out_name[i-1].insert(*it);
			    }
			    /*cout<<i;
			    for(set<string>::iterator it=in_name[i].begin();it!=in_name[i].end();it++)
				cout<<"  "<<*it;
			    cout<<endl;
                            if(old_size!=in_name[i].size())
				change=1;
			    continue;
                        }
			if(find(ariins.begin(),ariins.end(),op)!=ariins.end())
			{
			    string param2=parse_ins_param(parse_second_param(*it));
                            string param1=parse_ins_param(parse_first_param(*it));
			    in_name[i]=out_name[i];
			    in_name[i].erase(param2);
			    if(defname.find(param1)!=defname.end())
			        in_name[i].insert(param1);
			     if(defname.find(param2)!=defname.end())
			        in_name[i].insert(param2);
                             if(find(cfg_start.begin(),cfg_start.end(),i+1)!=cfg_start.end())
			     {
				for(int j=0;j<cfg_goto.size();j++)
				{
				    if(find(cfg_goto[j].begin(),cfg_goto[j].end(),i+1)!=cfg_goto[j].end())
				    {
					for(set<string>::iterator it=in_name[i].begin();it!=in_name[i].end();it++)
					    out_name[cfg_end[j]-1].insert(*it);
				    }
				}
			     }
			    else
			    {
			        for(set<string>::iterator it=in_name[i].begin();it!=in_name[i].end();it++)
				    out_name[i-1].insert(*it);
			    }
			    /*cout<<i;
			    for(set<string>::iterator it=in_name[i].begin();it!=in_name[i].end();it++)
				cout<<"  "<<*it;
			    cout<<endl;
                            if(old_size!=in_name[i].size())
				change=1;
			    continue;
                        }
		    }
		}
            /*dead code elimination*/
	    /*for(set<string>::iterator it=defname.begin();it!=defname.end();it++)
	    {
		bool islive=false;
		for(vector<set<string> >::iterator itt=in_name.begin();itt!=in_name.end();itt++)
		{
		    if((*itt).find(*it)!=(*itt).end())
		    {
			islive=true;
			break;
		    }
		}
		set<int> src_set;
                for(int n=0;n!=scr.size();n++)
	        {
	            for(int q=0;q!=scr[n].size();q++)
		    {
			vector<int>::iterator it=find(cfg_start.begin(),cfg_start.end(),scr[n][q]);
			int index=it-cfg_start.begin();
			for(int i=cfg_start[index];i<=cfg_end[index];i++)
			    src_set.insert(i);
		    }
	        }
		if(islive==false)
		{
		    int line_no;
		    for(map<int,Def>::iterator itt=def_t.begin();itt!=def_t.end();itt++)
		    {
			if((*itt).second.name==(*it))
			{
			    line_no=(*itt).second.num-1;
			    int s_beg=code3add[line_no].find(": ")+1;
			    code3add[line_no]=code3add[line_no].substr(0,s_beg+1);
			    code3add[line_no]+="nop";
			    if(src_set.find(line_no)==src_set.end())
			    {
				out_scr++;
			    }
			    else
			    {
				in_scr++;
			    }
			}
		    }
		}
	    }*/
            /*cout<<"Function: "<<cfg_start[0]<<endl;
	    cout<<"Number of statements eliminated in SCR: "<<in_scr<<endl;
	    cout<<"Number of statements eliminated not in SCR: "<<out_scr<<endl;
*/
	    /*clear the varible for next use*/
	    cfg_goto.clear();
	    cfg_start.clear();
	    continue;
	}
    }
    for(vector<string>::iterator it=code3add.begin();it!=code3add.end();it++)
	cout<<(*it)<<endl;
    return 0;
}
