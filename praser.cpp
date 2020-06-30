#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef pair<int,int> pii;
#define mp make_pair
#define fi first
#define se second
#define All(x) (x).begin(),(x).end()
#define Y1 "YES"
#define N1 "NO"
#define ENDL '\n'
#define count2(x) _builtin_popcount(x)

map<string,char>STmap;

inline int read(){
    int s=0,w=1;char ch=getchar();
    while(ch<'0'||ch>'9'){if(ch=='-')w=-1;ch=getchar();}
    while(ch>='0' && ch<='9')s=s*10+ch-'0',ch=getchar();
    return s*w;

}

struct Inode{//A-> alpha~B~beta,a 
	char A;
	string alpha;
	char B;
	string beta;
	char zhanwangfua;
	string cp(){
		return A+alpha+B+beta+zhanwangfua;
	}
	void clear(){
		A=B=zhanwangfua=0;
		alpha.clear();
		beta.clear();
	}
}; 

bool cmpInode(Inode a,Inode b){
	return a.cp()<b.cp();
}

typedef pair<char,string> G;//扩广文法 
string stringtosymbols(string s){
	string buf;
	string ans;
	for(auto ch:s){
		if(ch==' '||ch=='\n'){
			if(STmap.find(buf)==STmap.end()){
				if(buf.size()==1)
				cout<<"find a default end symbol.\n";
				else
				{
					cout<<"error not in STmap but not a char symbol\n";
					return "error";
				}
				ans+=buf;
			}
			else ans+=STmap[buf];
			buf.clear();
		}
		else buf+=ch;
	}
	return ans;
}

bool isend(char p){//判断是否是终结符 
//E和S被占用为S` S @ # 
	//if(p<0) return true;
 	//return (p>='a'&&p<='z')||(p>='0'&&p<='9')||p=='.'||p=='#'||p=='@'||p=='='||p=='*'||p=='+'||p=='('||p==')';
 	//非终结符是大写字母
	 if(p>='A'&&p<='Z')return false;
	 return true;
}

vector<char> FIRST(string betaa,vector<G> Gvector,int pos=0){//对给出的串betaa求first集 
	set<char> First;
	for(int i=0;i<betaa.size();++i){//遍历betaa产生First集 B#
		char p=betaa[i];
		//终结符的FIRST集是自己 
		if(isend(p)){
			if(p=='@')continue;
			First.insert(p);
			break;
		}
		for(int j=pos;j<Gvector.size();++j){//X=X1X2...  B=X1X2X3
			if(Gvector[j].fi==p){
				for(int k=0;k<Gvector[j].se.size();++k){//@表示空集
					set<char> FirstTmp;
					if(isend(Gvector[j].se[k])){//如果是终结符 
						FirstTmp.insert(Gvector[j].se[k]);//加入这个First集合 
					}
					else{
						string tk;tk.push_back(Gvector[j].se[k]);
						vector<char>tpv=FIRST(tk,Gvector,j+1);//递归求FIRST集 
						for(auto ch:tpv)FirstTmp.insert(ch);
					} 
					for(auto ch:FirstTmp){
						if(ch=='@'&&k!=Gvector[j].se.size()-1) continue;
						First.insert(ch);
					} 
					if(FirstTmp.find('@')==FirstTmp.end()){//没有空集就跳出来 
						break;
					}
					if(k!=Gvector[j].se.size()-1){
						FirstTmp.erase('@');
					} 
				}
			}
		}
		if(First.find('@')==First.end()){//没有空集就跳出来 
			break;
		}
		if(i!=betaa.size()-1){
			First.erase('@');
		} 
	}
	vector<char> ans;
	for(char i:First){
		ans.push_back(i);
	//	cout<<i<<" ";
	}
	//cout<<endl;
	return ans;
}

vector<Inode> closeure(vector<Inode> I,vector<G> Gvector){//扩广文法集
	for(int i=0;i<I.size();++i){//for(I中的每个项[A->α.Bβ,a) 
//	cout<<"Case1\n";
		for(int j=0;j<Gvector.size();++j){//for(G`中的每个产生式B->y) 
			if(Gvector[j].fi==I[i].B){
//	cout<<Gvector[j].fi<<" "<<Gvector[j].se<<" "<<"Case2\n";
	//cout<<"Case3\n";
				string betaa=I[i].beta+I[i].zhanwangfua;
				//for(FIRST(βa)中的每个终结符号b
			//	cout<<"in\n";
				vector<char>FST=FIRST(betaa,Gvector);
				//含ε的FST集 
			//	cout<<"out\n";
			/*
				if(FST.empty()){
					cerr<<Gvector[j].fi<<endl;
					cerr<<I[i].beta<<" "<<I[i].zhanwangfua<<endl;
					cerr<<"Fisrt empty\n";
					getchar(); 
				}*/
				assert(FST.empty()==0);
				for(auto ch:FST){
//	cout<<"Case4\n";
					//assert(ch!='@');
					//if(ch=='@')continue;
					Inode pInode;
					string sy=Gvector[j].se;
					pInode.zhanwangfua=ch;
					assert(!sy.empty());
					pInode.A=I[i].B;
					if(sy[0]=='@'){
						pInode.B='@';
						pInode.beta="@";
					}
					else if(sy.size()==1){
						pInode.B=sy[0];
						pInode.beta="@";
					}
					else{
						pInode.B=sy[0];
						pInode.beta=sy.substr(1);
					}
					int onlyonce=0;
					for(int p=0;p<I.size();++p){
						if(I[p].cp()==pInode.cp()){
							onlyonce=1;
							break;
						}
					}
					if(onlyonce==0)
					I.push_back(pInode);
					//cout<<"add"<<endl; 
				} 
			}
		}
	}
	return I;
}

vector<Inode> GOTO(vector<Inode> I,char X,vector<G>Gvector){
	vector<Inode>J;
	for(Inode ci:I){
		//X!=εX!=#
		if(ci.B==X){
			if(ci.alpha.empty()!=1&&ci.alpha.back()=='@'){
				
			}
			else
			ci.alpha.push_back(ci.B);
			if(ci.beta.empty()){
				ci.B='@';
			}
			else{
				ci.B=ci.beta[0];
				ci.beta=ci.beta.substr(1);
				if(ci.beta.empty())
				ci.beta='@';
			}
			J.push_back(ci);
		}
	//	cout<<ci.cp()<<endl;
	}
	return closeure(J,Gvector);
}

pair<vector<vector<Inode>>,vector<pair<char,pii>> > items(vector<G> Gpp,set<char> &Symbol){
	vector<pair<char,pii>>past;
	vector<vector<Inode>>C;
	vector<Inode>B;
	//占用E和S作为两个非终结符，S为开始符，E为S^
	//Gpp.push_back(mp('E',"S"));
	//构造符号表； 
	for(auto i:Gpp){
		Symbol.insert(i.fi);
		for(auto p:i.se){
			Symbol.insert(p);
		}
	}
	Inode item0;
	item0.A='E';
	item0.B='S';
	item0.beta='@';
	item0.zhanwangfua='#';
	B.push_back(item0);
	B=closeure(B,Gpp);
	C.push_back(B);
	//将C初始化为{closure}（E->.S,#） 
	for(int i=0;i<C.size();++i){
		cout<<"C.size():"<<C.size()<<ENDL;
		for(auto symbol:Symbol){
			vector<Inode>InodeGO=GOTO(C[i],symbol,Gpp);
			if(InodeGO.empty())continue;
			int checker=0;//判断在C里是否有和InodeGO相同的 
			//static int cnt=1;
			//if(cnt==1){
			//	for(auto ccc:InodeGO){
		//			cout<<ccc.cp()<<endl;
			//	} 
			//	cnt++;
			//}
			sort(All(InodeGO),cmpInode);
			string hased;
			for(auto ccc:InodeGO){
				hased+=ccc.cp();
			//	cout<<ccc.cp()<<endl;
			} 
			for(int pp=0;pp<C.size();++pp)
			{
				if(InodeGO.size()!=C[pp].size())continue;
				sort(All(C[pp]),cmpInode);
				string pased;
				for(int qpp=0;qpp<C[pp].size();++qpp){
					pased+=C[pp][qpp].cp();
				}
				if(pased==hased){
					checker=pp+1;break;
				}
			}
			if(checker==0)
			{
			//	static int cpa=0;
			//	cout<<cpa++<<endl;
				//cout<<hased<<endl;
			//	for(auto ci:InodeGO){
					//for(auto ci:ai){
						//cout<<"cp"<<ci.cp()<<endl;
			//			cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
					//}
			//	}
			//cout<<"add:"<<hased<<endl;
				pii x=mp(i,C.size()); 
				past.emplace_back(symbol,x);
				C.push_back(InodeGO);
			}
			else{
				pii x=mp(i,checker-1);
				past.emplace_back(symbol,x);
			}
		}
	}
	//cout<<"size:";
	//cout<<C.size()<<endl;
	return mp(C,past);
} 
//ACTIONGOTO函数使用的全局变量 
vector<char>ansEndSymbols;
vector<char>ansNoendSymbols;
vector<vector<pair<int,char>>>ansAction;
vector<vector<int>>ansGoto;
vector<G>ansG;
// 
void teststring(const string w,const vector<char>EndSymbols,const vector<char>NoendSymbols,const vector<vector<pair<int,char>>>vAction,const vector<vector<int>>vGoto){
	//string input 输入串  vector<char>EndSymbols终结符表 vector<char>NoendSymbols非终结符表 
	// ansG是扩广文法集 
	map<char,int>EndSymbolMap;
	map<char,int>NoendSymbolMap;
	for(int ch=0;ch<EndSymbols.size();++ch){
		EndSymbolMap[EndSymbols[ch]]=ch;
	}
	for(int ch=0;ch<NoendSymbols.size();++ch){
		NoendSymbolMap[NoendSymbols[ch]]=ch;
	}
	vector<int>statestack;
	vector<char>symbolstack;
	int id=1;
	int posptr=0;
	if(w.empty())return ;
	char a=w[posptr];
	statestack.push_back(0);
	while(1){//always repeat
		int nowstate=statestack.back();
	//action second type :char a=accept s=yiru r=guiyue
		pair<int,char>fromaction=vAction[nowstate][EndSymbolMap[a]];
		if(fromaction.second=='s'){//if action[s,a]=yiru t
			cout<<"id:"<<id++;
			cout<<"栈:";
			for(auto ps:statestack)
			cout<<ps<<" ";
			cout<<"符号:";
			for(auto ps:symbolstack)
			cout<<ps<<" ";
			cout<<"动作:移入操作\n";
			int newstate=fromaction.first;
			statestack.push_back(newstate);
			symbolstack.push_back(a);
			posptr++;
			a=w[posptr];
		}
		else if(fromaction.second=='r'){//if action[s,a]=guiyue y
			cout<<"id:"<<id++;
			cout<<"栈:";
			for(auto ps:statestack)
			cout<<ps<<" ";
			cout<<"符号:";
			for(auto ps:symbolstack)
			cout<<ps<<" ";
			int posG=fromaction.first;
			int popsize=ansG[posG].se.size();
			for(int ir=0;ir<popsize;++ir){
				statestack.pop_back();
				symbolstack.pop_back();
			}
			symbolstack.push_back(ansG[posG].fi);
			int gototA=vGoto[statestack.back()][NoendSymbolMap[symbolstack.back()]];
			statestack.push_back(gototA);
			cout<<"动作:根据"<< ansG[posG].fi<<"->"<<ansG[posG].se<<"归约\n"; 
		}
		else if(fromaction.second=='a'){
			cout<<"id:"<<id++;
			cout<<"栈:";
			for(auto ps:statestack)
			cout<<ps<<" ";
			cout<<"符号:";
			for(auto ps:symbolstack)
			cout<<ps<<" ";
			cout<<"动作:语法分析完成\n";
			break; 
		}
		else{
			cout<<"id:"<<id++;
			cout<<"栈:";
			for(auto ps:statestack)
			cout<<ps<<" ";
			cout<<"符号:";
			for(auto ps:symbolstack)
			cout<<ps<<" ";
			cout<<"语法分析出错!剩余输入串:"; 
			for(int ps=posptr;ps<w.size();++ps)
			cout<<w[ps]<<endl;
			assert(1!=1);
			//报错 
		}
	}
}

void ACTIONGOTO(pair<vector<vector<Inode>>,vector<pair<char,pii>>> everypair,set<char> &Symbol,vector<G>Gvector){
	vector<char>EndSymbols;
	vector<char>NoendSymbols;
	for(char i:Symbol){
		if(isend(i))
		EndSymbols.push_back(i);
		else
		NoendSymbols.push_back(i);
	}
	EndSymbols.push_back('#');
	map<char,int>EndSymbolMap;
	map<char,int>NoendSymbolMap;
	for(int ch=0;ch<EndSymbols.size();++ch){
		EndSymbolMap[EndSymbols[ch]]=ch;
	}
	for(int ch=0;ch<NoendSymbols.size();++ch){
		NoendSymbolMap[NoendSymbols[ch]]=ch;
	}
	int ActionWidth=EndSymbols.size();
	int GotoWidth=NoendSymbols.size();
	int depth=everypair.fi.size();
	//char a=accept s=yiru r=guiyue
	pair<int,char> Action[ActionWidth][depth];
	int Goto[GotoWidth][depth];
	memset(Action,0,sizeof(Action));
	memset(Goto,0,sizeof(Goto));
	vector<vector<Inode>> &LRset=everypair.fi;
	for(int i=0;i<LRset.size();++i){
		for(int j=0;j<LRset[i].size();++j){
			Inode InodeX=LRset[i][j];
			char InodeBa=InodeX.B;
			if(InodeBa=='@'){//判断开始符 
				if(InodeX.A=='E'&&InodeX.alpha=="S"&&InodeX.zhanwangfua=='#'){
					Action[EndSymbolMap['#']][i].fi=666;
					Action[EndSymbolMap['#']][i].se='a';
				} 
				else{
					int findAtoAlpha=-1;
					G AtoAlpha;
					AtoAlpha.fi=InodeX.A;
					AtoAlpha.se=InodeX.alpha;
					if(AtoAlpha.se.size()>1&&AtoAlpha.se.back()=='@'){
						AtoAlpha.se.pop_back();
					}
					if(AtoAlpha.se.empty()==1){
						AtoAlpha.se="@";
					}
					for(int indexG=0;indexG<Gvector.size();++indexG){
						if(Gvector[indexG]==AtoAlpha){
							findAtoAlpha=indexG;
							break;
						}
					}
					if(findAtoAlpha==-1){
						cout<<AtoAlpha.fi<<" se:"<<AtoAlpha.se<<endl;
						cout<<AtoAlpha.se.empty()<<endl;
						assert(findAtoAlpha!=-1);
					}
					Action[EndSymbolMap[InodeX.zhanwangfua]][i].fi=findAtoAlpha;
				//	cout<<"Yes"<<endl;
					Action[EndSymbolMap[InodeX.zhanwangfua]][i].se='r';
				} 
				continue;
			}
			vector<Inode> GOTOIia=GOTO(LRset[i],InodeBa,Gvector);
			if(GOTOIia.empty()){
				continue;
			}
			int checker=0;//判断在C里是否有和InodeX相同的 
			sort(All(GOTOIia),cmpInode);
			string hased;
			for(auto ccc:GOTOIia){
				hased+=ccc.cp();
			} 
			for(int cpi=0;cpi<LRset.size();++cpi){
				if(GOTOIia.size()!=LRset[cpi].size())continue;
				string pased;
				for(int qpp=0;qpp<LRset[cpi].size();++qpp){
					pased+=LRset[cpi][qpp].cp();
				}
				if(pased==hased){
					checker=cpi+1;
					break;
				}
			}
			assert(checker!=0);
			//assert(InodeBa!='S');
			if(isend(InodeBa)){//Action
				Action[EndSymbolMap[InodeBa]][i].fi=checker-1;
				//	cout<<"Yes"<<endl;
				Action[EndSymbolMap[InodeBa]][i].se='s';
			}
			else{
				if(InodeBa=='S')
				cout<<checker-1<<"assert!!!\n";
				Goto[NoendSymbolMap[InodeBa]][i]=checker-1;
			}
		}
	}
	
	vector<vector<pair<int,char>>>vAction;
	//vector<ActionWidth<type>> depth;
	for(int i=0;i<depth;++i){
		vector<pair<int,char>>listwidth;
		for(int j=0;j<ActionWidth;j++)
		listwidth.push_back(Action[j][i]);
		vAction.push_back(listwidth);
	}
	vector<vector<int>>vGoto;
	for(int i=0;i<depth;++i){
		vector<int>listwidth;
		for(int j=0;j<GotoWidth;j++)
		listwidth.push_back(Goto[j][i]);
		vGoto.push_back(listwidth);
	}
	
	/*
	printf("Table \n");
	cout<<"Name:";
	for(int j=0;j<EndSymbols.size();j++){
		cout<<setw(8)<<EndSymbols[j];
	}
	for(int j=0;j<NoendSymbols.size();j++){
		cout<<setw(8)<<NoendSymbols[j];
	}
	cout<<endl;
	for(int i=0;i<depth;++i){
		cout<<i<<setw(8);
		for(int j=0;j<EndSymbols.size();++j){
			if(Action[j][i].first==0){
				cout<<setw(8)<<"empty";
			}
			else if(Action[j][i].second=='a'){
				cout<<setw(8)<<"acc";
			}
			else if(Action[j][i].second=='r'){
				cout<<setw(8)<<"r"<<Action[j][i].first;
			}
			else{
				cout<<setw(8)<<Action[j][i].second<<Action[j][i].first;
			}
		}
		for(int j=0;j<NoendSymbols.size();++j){
			if(Goto[j][i]==0){
				cout<<setw(8)<<"empty";
			}
			else{
				cout<<setw(8)<<Goto[j][i];
			}
		}
		cout<<endl;
	}
	*/
	//output code
	FILE* file=fopen("language.csv","w");
	string res;
	res+="Name:,";
	for(int j=0;j<EndSymbols.size();j++){
		res+=EndSymbols[j];
		res+=",";
	}
	for(int j=0;j<NoendSymbols.size();j++){
		res+=NoendSymbols[j];
		res+=",";
	}
	res+='\n';
	for(int i=0;i<depth;++i){
		res+=to_string(i);
		res+=",";
		for(int j=0;j<EndSymbols.size();++j){
			if(Action[j][i].first==0){
				res+=",";
			}
			else if(Action[j][i].second=='a'){
				res+="acc,";
			}
			else if(Action[j][i].second=='r'){
				res+="r"+to_string(Action[j][i].first)+",";
			}
			else{
				res+=Action[j][i].second+to_string(Action[j][i].first)+",";
			}
		}
		for(int j=0;j<NoendSymbols.size();++j){
			if(Goto[j][i]==0){
				res+=",";
			}
			else{
				res+=to_string(Goto[j][i])+",";
			}
		}
			res+='\n';
	}
    fwrite(res.c_str(),res.size(),sizeof(char),file);
	fclose(file);
	//assert(ansAction.size()==vAction.size());
	//assert(ansGoto.size()==vGoto.size());
	//assert(ansEndSymbols.size()==EndSymbols.size());
	//assert(ansNoendSymbols.size()==NoendSymbols.size());
	//assert(ansG.size()==Gvector.size());
	//for(int i=0;i<NoendSymbols.size();++i)
	//assert(NoendSymbols[i]==ansNoendSymbols[i]);
	///for(int i=0;i<EndSymbols.size();++i)
	//assert(EndSymbols[i]==ansEndSymbols[i]);
	//for(int i=0;i<ansG.size();++i)
//	assert(ansG[i]==Gvector[i]);
//	for(int i=0;i<ansGoto.size();++i){
//		if(ansGoto[i].size()!=vGoto[i].size())
//		cout<<ansGoto[i].size()<<" "<<vGoto[i].size()<<endl;
//		for(int j=0;j<ansGoto[i].size();++j)
//		if(ansGoto[i][j]!=vGoto[i][j])
//		cout<<ansGoto[i][j]<<" "<<vGoto[i][j]<<endl;
		//assert(ansGoto[i]==vGoto[i]);
//	}
	
//	for(int i=0;i<ansAction.size();++i)
//	assert(ansAction[i]==vAction[i]);
	ansAction=vAction;
	ansGoto=vGoto;
	ansEndSymbols=EndSymbols;
	ansNoendSymbols=NoendSymbols;
	ansG=Gvector;//ansG是扩广文法集 
	res.clear();
	file=fopen("csl.txt","w");
	for(int j=0;j<EndSymbols.size();j++){
		res+=EndSymbols[j];
		res+="\n";
	}
	res+="########\n";
	for(int j=0;j<NoendSymbols.size();j++){
		res+=NoendSymbols[j];
		res+="\n";
	}
	res+="########\n";
	for(int i=0;i<depth;++i){
	for(int j=0;j<NoendSymbols.size();++j){
			res+=to_string(Goto[j][i])+"\n";
	}
	res+="########\n";
	}
	res+="###\n";
	for(int i=0;i<depth;++i){
	for(int j=0;j<EndSymbols.size();++j){
			res+=Action[j][i].se+to_string(Action[j][i].fi)+"\n";//int
	}
	res+="########\n";
	}
	res+="###\n";
	for(int j=0;j<ansG.size();++j){
			res.push_back(ansG[j].fi);//char
//			cout<<ansG[j].fi<<" "<<ansG[j].se<<" ";
			res+="\n"+ansG[j].se+"\n";//string
	}
	res+="###\n";
    fwrite(res.c_str(),res.size(),sizeof(char),file);
	fclose(file);
	cout<<"save to csl.txt sucessfully\n";
	return;
	//return mp(mp(vector<char>EndSymbols,vector<char>NoendSymbols),mp(vector<vector<pair<int,char>>>vAction,vector<vector<pair<int,char>>>vGoto));
}
void testFIRST(){
	cout<<"BeginFisrtTest\n";
	vector<G>Gvector;
	cout<<"Testcase1\n";
	Gvector.emplace_back('B',"aB");
	Gvector.emplace_back('B',"b");
	vector<char>pd=FIRST("B",Gvector);
	for(auto i:pd){
		cout<<i<<" ";
	}cout<<endl;
	Gvector.clear();
	cout<<"Testcase2\n";
	Gvector.emplace_back('B',"Ab");
	Gvector.emplace_back('A',"a");
	pd=FIRST("B",Gvector);
	for(auto i:pd){
		cout<<i<<" ";
	}cout<<endl;
	Gvector.clear();
	cout<<"Testcase3\n";
	Gvector.emplace_back('B',"AC");
	Gvector.emplace_back('A',"@");
	Gvector.emplace_back('A',"b");
	Gvector.emplace_back('C',"a");
	pd=FIRST("B",Gvector);
	for(auto i:pd){
		cout<<i<<" ";
	}cout<<endl;
	Gvector.clear();
	cout<<"Testcase4\n";
	Gvector.emplace_back('B',"Aa");
	Gvector.emplace_back('A',"@");
	Gvector.emplace_back('A',"b");
	pd=FIRST("B",Gvector);
	for(auto i:pd){
		cout<<i<<" ";
	}cout<<endl;
	Gvector.clear();
	cout<<"Testcase5\n";
	Gvector.emplace_back('B',"aB");
	Gvector.emplace_back('B',"b");
	pd=FIRST("B#",Gvector);
	for(auto i:pd){
		cout<<i<<" ";
	}cout<<endl;
}
void testCLOSE(){
	cout<<"BeginFisrtTest\n";
	vector<G>Gvector;
	cout<<"Testcase1\n";
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"BB");
	Gvector.emplace_back('B',"aB");
	Gvector.emplace_back('B',"b");
	Inode test;//A-> alpha~B~beta,a 
	test.A='E';
	test.B='S';
	test.zhanwangfua='#';
	test.beta="@";
	vector<Inode>I;//项目集 
	I.push_back(test);
	vector<Inode> ans1=closeure(I,Gvector);
	for(auto ci:ans1){
		cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
	Gvector.clear();
	cout<<"Testcase2\n";
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"AaAb");
	Gvector.emplace_back('S',"BbBa");
	Gvector.emplace_back('A',"@");
	Gvector.emplace_back('B',"@");
	test.clear();//A-> alpha~B~beta,a 
	test.A='E';
	test.B='S';
	test.zhanwangfua='#';
	test.beta="@";
	I.clear();//项目集 
	I.push_back(test);
	vector<Inode> ans2=closeure(I,Gvector);
	for(auto ci:ans2){
		cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
	Gvector.clear();
	cout<<"Testcase3\n";
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"L=R");
	Gvector.emplace_back('S',"R");
	Gvector.emplace_back('L',"*R");
	Gvector.emplace_back('L',"i");
	Gvector.emplace_back('R',"L");
	test.clear();//A-> alpha~B~beta,a 
	test.A='E';
	test.B='S';
	test.beta="@";
	test.zhanwangfua='#';
	I.clear();//项目集 
	I.push_back(test);
	vector<Inode> ans3=closeure(I,Gvector);
	for(auto ci:ans3){
		cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
}
void testitems1(set<char>&Symbol){
	cout<<"BeginFisrtitemTest1\n";
	vector<G>Gvector;
	cout<<"Testcase1\n";
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"BB");
	Gvector.emplace_back('B',"aB");
	Gvector.emplace_back('B',"b");
	pair<vector<vector<Inode>>,vector<pair<char,pii>>>ans1=items(Gvector,Symbol);//return Symbols
	cout<<"size:";
	cout<<ans1.fi.size()<<endl;
	for(int i=0;i<ans1.fi.size();++i){
			cout<<"I"<<i<<":"<<endl;
		//if(i>0)cout<<"past:"<<ans1.se[i-1].fi<<" "<<ans1.se[i-1].se<<endl;
		for(auto ci:ans1.fi[i]){
				//cout<<"cp"<<ci.cp()<<endl;
				cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		}
		
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
	for(auto ipii:ans1.se){
		cout<<ipii.fi<<" "<<ipii.se.fi<<" "<<ipii.se.se<<endl;
	}
	ACTIONGOTO(ans1,Symbol,Gvector);
	Gvector.clear();
}
void testitems2(set<char>&Symbol){
	cout<<"BeginFisrtitemTest2\n";
	vector<G>Gvector;
	cout<<"Testcase2\n";
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"L.L");
	Gvector.emplace_back('S',"L");
	Gvector.emplace_back('L',"LB");
	Gvector.emplace_back('L',"B");
	Gvector.emplace_back('B',"0");
	Gvector.emplace_back('B',"1");
	Symbol.clear();
	pair<vector<vector<Inode>>,vector<pair<char,pii>>>ans2=items(Gvector,Symbol);//return Symbols
	cout<<"size:";
	cout<<ans2.fi.size()<<endl;
	for(int i=0;i<ans2.fi.size();++i){
			cout<<"I"<<i<<":"<<endl;
		//if(i>0)cout<<"past:"<<ans1.se[i-1].fi<<" "<<ans1.se[i-1].se<<endl;
		for(auto ci:ans2.fi[i]){
				//cout<<"cp"<<ci.cp()<<endl;
				cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		}
		
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
	for(auto ipii:ans2.se){
		cout<<ipii.fi<<" "<<ipii.se.fi<<" "<<ipii.se.se<<endl;
	}
	//ACTIONGOTO(ans1,Symbol);
	Gvector.clear();
}
void testitems3(set<char>&Symbol){
	cout<<"BeginFisrtitemTest3\n";
	vector<G>Gvector;
	cout<<"Testcase3\n";
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"BB");
	Gvector.emplace_back('B',"aB");
	Gvector.emplace_back('B',"b");
	Symbol.clear();
	pair<vector<vector<Inode>>,vector<pair<char,pii>>>ans3=items(Gvector,Symbol);//return Symbols
	cout<<"size:";
	cout<<ans3.fi.size()<<endl;
	for(int i=0;i<ans3.fi.size();++i){
			cout<<"I"<<i<<":"<<endl;
		//if(i>0)cout<<"past:"<<ans1.se[i-1].fi<<" "<<ans1.se[i-1].se<<endl;
		for(auto ci:ans3.fi[i]){
				//cout<<"cp"<<ci.cp()<<endl;
				cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		}
		
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
	for(auto ipii:ans3.se){
		cout<<ipii.fi<<" "<<ipii.se.fi<<" "<<ipii.se.se<<endl;
	}
	//ACTIONGOTO(ans3,Symbol);
	Gvector.clear();
}
void testitems4(set<char>&Symbol){
	cout<<"BeginFisrtitemTest4\n";
	vector<G>Gvector;
	cout<<"Testcase4\n";
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"SS");
	Gvector.emplace_back('S',"A");
	Gvector.emplace_back('A',"a");
	Symbol.clear();
	pair<vector<vector<Inode>>,vector<pair<char,pii>>>ans4=items(Gvector,Symbol);//return Symbols
	cout<<"size:";
	cout<<ans4.fi.size()<<endl;
	for(int i=0;i<ans4.fi.size();++i){
			cout<<"I"<<i<<":"<<endl;
		//if(i>0)cout<<"past:"<<ans1.se[i-1].fi<<" "<<ans1.se[i-1].se<<endl;
		for(auto ci:ans4.fi[i]){
				//cout<<"cp"<<ci.cp()<<endl;
				cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		}
		
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
	for(auto ipii:ans4.se){
		cout<<ipii.fi<<" "<<ipii.se.fi<<" "<<ipii.se.se<<endl;
	}
	//ACTIONGOTO(ans4,Symbol);
	Gvector.clear();
}
void testitems5(set<char>&Symbol){
	cout<<"BeginFisrtitemTest5\n";
	vector<G>Gvector;
	cout<<"Testcase5\n";
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"CC");
	Gvector.emplace_back('C',"cC");
	Gvector.emplace_back('C',"d");
	Symbol.clear();
	pair<vector<vector<Inode>>,vector<pair<char,pii>>>ans5=items(Gvector,Symbol);//return Symbols
	cout<<"size:";
	cout<<ans5.fi.size()<<endl;
	for(int i=0;i<ans5.fi.size();++i){
			cout<<"I"<<i<<":"<<endl;
		//if(i>0)cout<<"past:"<<ans1.se[i-1].fi<<" "<<ans1.se[i-1].se<<endl;
		for(auto ci:ans5.fi[i]){
				//cout<<"cp"<<ci.cp()<<endl;
				cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		}
		
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
	for(auto ipii:ans5.se){
		cout<<ipii.fi<<" "<<ipii.se.fi<<" "<<ipii.se.se<<endl;
	}
	//auto ansp=ACTIONGOTO(ans5,Symbol,Gvector);
	Gvector.clear();
}
void testitems6(set<char>&Symbol){
	cout<<"BeginFisrtitemTest6\n";
	vector<G>Gvector;
	cout<<"Testcase6\n";
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"S+T");
	Gvector.emplace_back('S',"T");
	Gvector.emplace_back('T',"T*F");
	Gvector.emplace_back('T',"F");
	Gvector.emplace_back('F',"(S)");
	Gvector.emplace_back('F',"i");
	Symbol.clear();
	pair<vector<vector<Inode>>,vector<pair<char,pii>>>ans5=items(Gvector,Symbol);//return Symbols
	//cout<<"size:";
	//cout<<ans5.fi.size()<<endl;
	for(int i=0;i<ans5.fi.size();++i){
			cout<<"I"<<i<<":"<<endl;
		//if(i>0)cout<<"past:"<<ans1.se[i-1].fi<<" "<<ans1.se[i-1].se<<endl;
		for(auto ci:ans5.fi[i]){
				//cout<<"cp"<<ci.cp()<<endl;
				cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		}
		
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
	for(auto ipii:ans5.se){
	//	cout<<ipii.fi<<" "<<ipii.se.fi<<" "<<ipii.se.se<<endl;
	}
	ACTIONGOTO(ans5,Symbol,Gvector);
	Gvector.clear();
}
void testitems7(set<char>&Symbol){
	cout<<"BeginFisrtitemTest6\n";
	vector<G>Gvector;
	cout<<"Testcase6\n";
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"A");
	Gvector.emplace_back('A',"BA");
	Gvector.emplace_back('A',"@");
	Gvector.emplace_back('B',"aB");
	Gvector.emplace_back('B',"b");
	Symbol.clear();
	pair<vector<vector<Inode>>,vector<pair<char,pii>>>ans5=items(Gvector,Symbol);//return Symbols
	//cout<<"size:";
	//cout<<ans5.fi.size()<<endl;
	for(int i=0;i<ans5.fi.size();++i){
			cout<<"I"<<i<<":"<<endl;
		//if(i>0)cout<<"past:"<<ans1.se[i-1].fi<<" "<<ans1.se[i-1].se<<endl;
		for(auto ci:ans5.fi[i]){
				//cout<<"cp"<<ci.cp()<<endl;
				cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		}
		
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
	for(auto ipii:ans5.se){
	//	cout<<ipii.fi<<" "<<ipii.se.fi<<" "<<ipii.se.se<<endl;
	}
	ACTIONGOTO(ans5,Symbol,Gvector);
	Gvector.clear();
}
void expAitems(set<char>&Symbol){
	cout<<"BeginexpAitemsTest6\n";
	vector<G>Gvector;
	cout<<"expAitems\n";
	/*
	E → S
	S→A   |B
	A →   C  
	C → D  a;
	D → D[n]  |  c
	B →   F 

	*/
	Gvector.emplace_back('E',"S");
	Gvector.emplace_back('S',"A");Gvector.emplace_back('S',"B");
	Gvector.emplace_back('A',"C");
	Gvector.emplace_back('C',"Da;");
	Gvector.emplace_back('D',"D[n]");Gvector.emplace_back('D',"c");
	Gvector.emplace_back('B',"F");
	/*
	F → G=H;
      | d(H)F
      | d(H)F e F
      | f(H)F
      | g F f(H);
      | h;
*/

	
	Gvector.emplace_back('F',"G=H;");
	
	Gvector.emplace_back('F',"d(H)F");
	Gvector.emplace_back('F',"d(H)FeF");
	Gvector.emplace_back('F',"f(H)F");
	Gvector.emplace_back('F',"gFf(H);");
	Gvector.emplace_back('F',"h;");
	/* 
	G → G[H]  | a
	H →H  r  I   |  I
	I → I  i  J  | J
	J → J j K  | J k K  | K
	K → L<L |L l L|L m L|L>L|L
	L → L+M |L-M |M
	M → M*N|M/N|N
	N→！N | -N | O
	O→ (H) | G | n | o | p | q
	*/
	Gvector.emplace_back('G',"G[H]");Gvector.emplace_back('G',"a");
	Gvector.emplace_back('H',"HrI");Gvector.emplace_back('H',"I");
	Gvector.emplace_back('I',"IiJ");Gvector.emplace_back('I',"J");
	Gvector.emplace_back('J',"JjK");Gvector.emplace_back('J',"JkK");Gvector.emplace_back('J',"K");
	Gvector.emplace_back('K',"L<L");Gvector.emplace_back('K',"LlL");Gvector.emplace_back('K',"LmL");Gvector.emplace_back('K',"L>L");Gvector.emplace_back('K',"L");
	Gvector.emplace_back('L',"L+M");Gvector.emplace_back('L',"L-M");Gvector.emplace_back('L',"M");
	Gvector.emplace_back('M',"M*N");Gvector.emplace_back('M',"M/N");Gvector.emplace_back('M',"N");
	Gvector.emplace_back('N',"!N");Gvector.emplace_back('N',"-N");Gvector.emplace_back('N',"O");
	Gvector.emplace_back('O',"(H)");Gvector.emplace_back('O',"G");Gvector.emplace_back('O',"n");Gvector.emplace_back('O',"o");Gvector.emplace_back('O',"p");Gvector.emplace_back('O',"q");
	
	Symbol.clear();
	pair<vector<vector<Inode>>,vector<pair<char,pii>>>ans5=items(Gvector,Symbol);//return Symbols
	//cout<<"size:";
	//cout<<ans5.fi.size()<<endl;
	for(int i=0;i<ans5.fi.size();++i){
			cout<<"I"<<i<<":"<<endl;
		//if(i>0)cout<<"past:"<<ans1.se[i-1].fi<<" "<<ans1.se[i-1].se<<endl;
		for(auto ci:ans5.fi[i]){
				//cout<<"cp"<<ci.cp()<<endl;
				cout<<ci.A<<"->"<<ci.alpha<<"."<<ci.B<<'|'<<ci.beta<<","<<ci.zhanwangfua<<endl;
		}
		
		//cout<<"A:"<<ci.A<<" alpha:"<<ci.alpha<<" B:"<<ci.B<<" beta:"<<ci.beta<<" zhanwangfua:"<<ci.zhanwangfua<<endl;
	}
	for(auto ipii:ans5.se){
	//	cout<<ipii.fi<<" "<<ipii.se.fi<<" "<<ipii.se.se<<endl;
	}
	ACTIONGOTO(ans5,Symbol,Gvector);
	Gvector.clear();
}
void preexpAmap(){
	STmap["program"]='E';
	STmap["block"]='S';
	STmap["decls"]='A';
	STmap["stmts"]='B';
	STmap["decl"]='C';
	STmap["type"]='D';
	STmap["stmt"]='F';
	STmap["loc"]='G';
	STmap["bool"]='H';
	STmap["join"]='I';
	STmap["equality"]='J';
	STmap["rel"]='K';
	STmap["expr"]='L';
	STmap["term"]='M';
	STmap["unary"]='N';
	STmap["factor"]='O';
	
	STmap["id"]='a';
	STmap["num"]='b';
	STmap["basic"]='c';
	STmap["if"]='d';
	STmap["else"]='e';
	STmap["while"]='f';
	STmap["do"]='g';
	STmap["break"]='h';
	STmap["&&"]='i';
	STmap["=="]='j';
	STmap["!="]='k';
	STmap["<="]='l';
	STmap[">="]='m';
	STmap["num"]='n';
	STmap["real"]='o';
	STmap["ture"]='p';
	STmap["false"]='q';
	STmap["||"]='r';
}
void readfromcsl(){
	ifstream infile("csl.txt");  // 读操作
	if(!infile.is_open()){
        cout << "打开csl.txt文件失败" << endl;
        return;
    }
    string temp;
	while(getline(infile,temp)){
		if(temp=="########") {
			cout<<"read EndSymbols end\n";
			break;
		}
		char j;
		sscanf(temp.c_str(),"%c",&j);
		ansEndSymbols.push_back(j);
	}
	cout<<ansEndSymbols.size()<<"\n";
//	getchar();
	while(getline(infile,temp)){
		if(temp=="########") {
		//	cout<<"read NoendSymbols end\n";
			break;
		}
		char j;
		sscanf(temp.c_str(),"%c",&j);
		ansNoendSymbols.push_back(j);
	}//Goto
	cout<<ansNoendSymbols.size()<<"\n";
	//getchar();
	ansGoto.push_back({});
	while(getline(infile,temp)){
		vector<int>tp;
		while(1){
			if(temp=="########"){
			//	cout<<"read a end\n";
				break;
			}
			int j;
			char k;
			sscanf(temp.c_str(),"%d",&j);
			tp.emplace_back(j);
			getline(infile,temp);
		}
		ansGoto.push_back(tp);
		getline(infile,temp);
		if(temp=="###") break;
	}
	ansGoto.pop_back();
	cout<<ansGoto.size()<<"\n";
//	getchar();
	while(getline(infile,temp)){
		vector<pair<int,char>>tp;
		while(1){
			if(temp=="########"){
			//	cout<<"read a end\n";
				break;
			}
			int j;
			char k;
			sscanf(temp.c_str(),"%c%d",&k,&j);
		//	cout<<k<<" "<<j<<" "<<temp<<endl;
			tp.emplace_back(j,k);
			getline(infile,temp);
		}
		ansAction.push_back(tp);
		getline(infile,temp);
		if(temp=="###") break;
	}
	cout<<ansAction.size()<<"\n";
//	getchar();
	while(getline(infile,temp)){
		if(temp=="###") {
			cout<<"read EndSymbols end\n";
			break;
		}
		char j;
		sscanf(temp.c_str(),"%c",&j);
		getline(infile,temp);
		ansG.emplace_back(j,temp);
	}
	cout<<ansG.size()<<"\n";
	infile.close();
	cout<<"read to csl.txt sucessfully\n";
	return;
}
int main(){
//	preexpAmap();
	int n;
	//testFIRST();
	//testCLOSE();
	set<char>Symbol;
//	readfromcsl();
	expAitems(Symbol);
//	testitems1(Symbol);
//	testitems2(Symbol);
	//testitems3(Symbol);
	//testitems4(Symbol);
	//testitems5(Symbol);
	//testitems6(Symbol);
	//expAitems(Symbol);
	//testitems7(Symbol);
	string s;
	while(cin>>s){
	//	s=stringtosymbols(s);
	 	s+='#';
		teststring(s,ansEndSymbols,ansNoendSymbols,ansAction,ansGoto);
	} 
	return 0;
}

