#include <bits/stdc++.h>
#define GOTO 1
#define ACTION 0
#define SHIFT 0
#define REDUCE 1
#define ACCEPT 2
#define ERROR -1

using namespace std;

struct prod{
	string rule; // actual production rule
	int num; // no of terminals/non-terminals on RHS
};

class state{
	public:
	vector<prod> items;
	map<char,int> trans;
	bool isRedundant;
	state(){
		isRedundant=true;
	}
};

struct entry{
	int type; // 0-action, 1-goto
	int action; // 0-shift, 1-reduce, 2-accept, -1 error
	int val;
};

prod init;
int totStates=0;
vector<prod> prods;
vector<state> states;
map<set<string>,int> mp;
map<char,int> loc;
entry table[20][10];
stack<char> st;
char startSymbol;
int no_of_terminals;
int distinct_states=0,final_state;

void findClosure(string rule,set<string> &mySet){
	char temp='\0'; 
	for(int i=0;i<rule.size();i++)
		if(rule[i]=='.' && i+1<rule.size())
			temp=rule[i+1];
	if(temp=='\0')
		return;
	for(int j=0;j<prods.size();j++){
		if(prods[j].rule[0]==temp){
			init.rule=prods[j].rule.substr(0,2) + "." + prods[j].rule.substr(2);
			if(mySet.find(init.rule)==mySet.end()){
					mySet.insert(init.rule);
				//	cout << "Finding closure: " << init.rule << endl;
				findClosure(init.rule,mySet);
			}
		}
	}
}


string shiftDot(string rule,char &imp){
	string newRule="";
		bool var=false;
		for(int j=0;j<rule.size();j++){
			if(rule[j]=='.')
				var=true;
			else{
				newRule.push_back(rule[j]);
				if(var){
					imp=rule[j]; // symbol after dot
					newRule.push_back('.'); // dot pushed after that symbol
					var=false;
				}
			}
		}
	//	cout << "newRule: " << newRule << " imp: " << imp << endl;
	return newRule;
}

bool generateState(string rule,int &setNo){
	set<string> mySet;
	mySet.insert(rule);
//	cout << "generate state called with " << rule << endl;
	findClosure(rule,mySet);
	state tempState;
	for(set<string>::iterator it=mySet.begin();it!=mySet.end();it++){
		init.rule=(*it);
		tempState.items.push_back(init);
 	//	cout << "Printing state: " << *it << endl;
	}
	if(mp.find(mySet)==mp.end()){
		mp[mySet]=totStates++;
		states.push_back(tempState);
		setNo=mp[mySet];
		return true;
	}
	setNo=mp[mySet];
	return false;
}

bool insertIntoState(string rule,int oldStateNo,int &newSetNo){
	set<string> newSet;
	newSet.insert(rule);
	findClosure(rule,newSet);
	
	state oldState=states[oldStateNo];
	set<string> oldSet;
	
	for(vector<prod>::iterator it=oldState.items.begin();it!=oldState.items.end();it++){
		newSet.insert(it->rule);
		oldSet.insert(it->rule);
	}
	
	state newState;
	for(set<string>::iterator it=newSet.begin();it!=newSet.end();it++){
		init.rule=(*it);
		newState.items.push_back(init);
	}
	
	if(mp.find(newSet)==mp.end()){ // new state does NOT exist before
	//	mp.erase(oldSet);
		mp[newSet]=totStates++;
		states.push_back(newState);
		newSetNo=mp[newSet];
		return true;
	}else{                         // new state exist before
		newSetNo=mp[newSet];
		return false;
	}
}

void findTransitions(int p){
	state tempState=states[p];
	for(int i=0;i<tempState.items.size();i++){
		string rule=tempState.items[i].rule;
		if(rule[rule.size()-1]=='.')
			continue;
		char imp;
		string newRule=shiftDot(rule,imp);
		int tran;
		bool isNew;
		if(states[p].trans.find(imp)!=states[p].trans.end())  // transition state already exist for current input character
			isNew=insertIntoState(newRule,states[p].trans[imp],tran);
		else
			isNew=generateState(newRule,tran);
		states[p].trans[imp]=tran; 
		if(isNew)
			findTransitions(tran);
	}
}

void printStates(){
	for(int i=0;i<states.size();i++){
		cout << endl << "state: " << i << endl ;
		cout << "items: " << endl;
		for(vector<prod>::iterator it=states[i].items.begin(); it!= states[i].items.end();it++){
			cout << it->rule << endl;
		}
		cout << "trans: " << endl;
		for(map<char,int>::iterator it=states[i].trans.begin(); it!=states[i].trans.end();it++){
			cout << it->first << " " << it->second << endl;
		}
	}
}

int findProduction(string rule){
	for(int i=0;i<prods.size();i++){
		if(rule.compare(prods[i].rule)==0)
			return i;
	}
	return -1;
}


void fillTable(){
	for(int i=0;i<states.size();i++){
		for(int j=0;j<loc.size();j++){
			table[i][j].type=ACTION;
			table[i][j].action=ERROR;
			table[i][j].val=-1;
		}
	}
}


void generateTable(){
	for(int i=0;i<states.size();i++){
		bool isFinal=false;
		for(vector<prod>::iterator it=states[i].items.begin(); it!= states[i].items.end();it++){
			if(isFinal)
				cout << "Reduce Reduce conflict in grammar in state: " << i << endl;
			string rule = it->rule;
			int size=rule.size();
			if(rule[size-1]=='.'){
				if(rule[size-2]=='$'){
					final_state=i;
					for(int j=0;j<no_of_terminals;j++){
						table[i][j].type=ACTION;
						table[i][j].action=ACCEPT; // accepting state
					}
				}
				else{
					int index=findProduction(rule.substr(0,rule.size()-1));
					for(int j=0;j<no_of_terminals;j++){
						table[i][j].type=ACTION;
						table[i][j].action=REDUCE;
						table[i][j].val=index;
					}
				}
				isFinal=true;
			}
		}
		
		for(map<char,int>::iterator it=states[i].trans.begin(); it!=states[i].trans.end();it++){
			if(isFinal)
				cout << "Shift Reduce conflict in grammar in state: " << i << endl;
			if(it->first>='A' && it->first<='Z'){ // checking for terminals in transition function or Goto Function
				table[i][loc[it->first]].type=GOTO;
				table[i][loc[it->first]].val=it->second;
			}
			else{  // action type
				table[i][loc[it->first]].type=ACTION;
				table[i][loc[it->first]].action=SHIFT;
				table[i][loc[it->first]].val=it->second;
			}
		}
	}
}
	
void printTable(){
	cout << "state No\t";
//		for(map<char,int>::iterator it=loc.begin();it!=loc.end();it++)
//			cout << it->first << "\t";
		cout << "+\t" << "(\t" << ")\t" << "i\t" << "$\t" << "E\t" << "S";
		cout << endl;
	for(int i=0;i<totStates;i++){
		if(states[i].isRedundant)
			continue;
		cout << "state " << i << "\t\t";
		for(int j=0;j<loc.size();j++){
			if(table[i][j].type==ACTION)
				cout << "a" << " ";
			else
				cout << "g" << " ";
			if(table[i][j].action==SHIFT)
				cout << "s" << " ";
			else if(table[i][j].action==REDUCE)
				cout << "r" << " ";
			else if(table[i][j].action==ACCEPT)
				cout << "a" << " ";
			else
				cout << "e" << " ";
			cout << table[i][j].val << "\t";
		}
		cout << endl;
	}
}

void parseInput(string inp){
	string old;
	int i;
	for(i=0;i<inp.size();){
		entry e=table[st.top()-'0'][loc[inp[i]]];
	//	cout << "proc: " << i << " " << inp[i] << " stack Top: " << st.top() << endl;
		if(e.type==ACTION && e.action==ERROR){
			cout << "Parsing failed " << endl;
			return;
		}else if(e.type==ACTION && e.action==SHIFT){
			//cout << "pushing: " << inp[i] << endl;
			st.push(inp[i]);
			st.push(e.val+'0');
		//	cout << "pushing: " << e.val << endl;
			i++;
		}
		else if(e.type==ACTION && e.action==REDUCE){
			string temp="";
			int index=e.val;
			int num=prods[index].num;
			num*=2;
//			cout << "num " << num << endl;
//			cout << "prod used: " <<  index << endl;
			bool inc=false;
			while(num--){
				if(inc){
					temp.push_back(st.top());
					inc=false;
				}
				else
					inc=true;
				st.pop();
			}
			reverse(temp.begin(),temp.end());
		
			cout << temp << " is reduced to " << prods[index].rule[0] << endl;
			
			int state=table[st.top()-'0'][loc[prods[index].rule[0]]].val;
			if(state==-1){
				cout << "Parsing failed" << endl;
				return;
			}
//			cout << "First pushing: " << prods[index].rule[0] << endl;
//			cout << "then pushing: " << state << endl;
			st.push(prods[index].rule[0]);
			old=prods[index].rule[0];
			st.push(state+'0');	
		}
		else if(e.type==ACTION && e.action==ACCEPT){
			cout << "Parsing successful" << endl;
			return;
		}
	}
	if(i==inp.size() && (st.top()-'0')==final_state){
		cout << old << " is reduced to " << startSymbol << endl;
		cout << "Parsing successful" << endl;
	}
}

void loadMap(){
	loc['+']=0;
	loc['(']=1;
	loc[')']=2;
	loc['i']=3;
	loc['$']=4;
	loc['E']=5;
	loc['S']=6;
}

void deleteRedundantStates(){
	states[0].isRedundant=false;
	for(int i=0;i<totStates;i++){
		for(int j=0;j<loc.size();j++){
			if((table[i][j].type==ACTION && table[i][j].action==SHIFT) || table[i][j].type==GOTO)
				states[table[i][j].val].isRedundant=false;
		}
	}
	for(int i=0;i<totStates;i++){
		if(!states[i].isRedundant)
			distinct_states++;
	}
}

int main(){
	startSymbol='S';
	no_of_terminals=5;
	init.rule="S=E";
	init.num=1;
	prods.push_back(init);
	init.rule="E=E+(E)";
	init.num=5;
	prods.push_back(init);
	init.rule="E=i";
	init.num=1;
	prods.push_back(init);

	loadMap();
	
	string startRule="S=.E$";
	//startRule.push_back(startSymbol);
	int p;
	generateState(startRule,p);
	findTransitions(p);
	printStates();
	fillTable();
	generateTable();
	deleteRedundantStates();
	printTable();
	cout << "Distinct States are: " << distinct_states << endl;
	string input="i+(i+(i))$";
	st.push('0');
	parseInput(input);

	return 0;
}
