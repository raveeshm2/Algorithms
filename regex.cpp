#include <bits/stdc++.h>

// Works only for two variables, regular expression. Change code appropriately for more than 2 variables.
// symbols used
// * - Klean Closure
// + - Disjunction

using namespace std;

class nst{
	public:
		vector<int> a[2],e;
		bool f;
	nst(){
		f=false;
	}
};

nst init_nfa_state;
int nfa_size=0;
vector<nst> nfa;
stack<int> st;

class dst{
	public:
		int a[2];
		bool f;
	dst(){
		f=false;
		a[0]=a[1]=-1;
	}
};

dst init_dfa_state;
vector<dst> dfa;

string insert_concat(string regex){
	string ret="";
	char c,c2;
	for(int i=0;i<regex.size();i++){
		c=regex[i];
		if(i+1<regex.size()){
			c2=regex[i+1];
			ret+=c;
			if(c!='(' && c2!=')' && c!='+' && c2!='+' && c2!='*')
				ret+='.';
		}
	}
	ret+=regex[regex.size()-1];
	return ret;
}

int priority(char c){
	switch(c){
		case '*': return 3;
		case '.': return 2;
		case '+': return 1;
		default: return 0;
	}
	return 0;
}

string postFix(string regex){
	string exp="";
	stack<char> s;
	for(int i=0;i<regex.size();i++){
		switch(regex[i]){
			case 'a':
			case 'b':
				exp+=regex[i];
				break;
			case '(':
				s.push(regex[i]);
				break;
			case ')':
				while(s.top()!='('){
					exp+=s.top();
					s.pop();
				}
				s.pop();
				break;
			default:
				while(!s.empty() && priority(s.top())>=priority(regex[i])){
					exp+=s.top();
					s.pop();
				}
				s.push(regex[i]);
		}
	}
	while(!s.empty()){
		exp+=s.top();
		s.pop();
	}
	return exp;
}

void character(int x){
	nfa.push_back(init_nfa_state);
	nfa.push_back(init_nfa_state);
	nfa[nfa_size].a[x].push_back(nfa_size+1);
	st.push(nfa_size);
	nfa_size++;
	st.push(nfa_size);
	nfa_size++;
}

void concat(){
	int d=st.top();
	st.pop();
	int c=st.top();
	st.pop();
	int b=st.top();
	st.pop();
	int a=st.top();
	st.top();
	nfa[b].e.push_back(c);
	st.push(a);
	st.push(d);
}

void _union(){
	nfa.push_back(init_nfa_state);
	nfa.push_back(init_nfa_state);
	int d=st.top();
	st.pop();
	int c=st.top();
	st.pop();
	int b=st.top();
	st.pop();
	int a=st.top();
	st.pop();
	nfa[nfa_size].e.push_back(a);
	nfa[nfa_size].e.push_back(c);
	nfa[b].e.push_back(nfa_size+1);
	nfa[d].e.push_back(nfa_size+1);
	st.push(nfa_size);
	nfa_size++;
	st.push(nfa_size);
	nfa_size++;
}

void kleen_star(){
	nfa.push_back(init_nfa_state);
	nfa.push_back(init_nfa_state);
	int b=st.top();
	st.pop();
	int a=st.top();
	st.pop();
	nfa[nfa_size].e.push_back(a);
	nfa[nfa_size].e.push_back(nfa_size+1);
	nfa[b].e.push_back(a);
	nfa[b].e.push_back(nfa_size+1);
	st.push(nfa_size);
	nfa_size++;
	st.push(nfa_size);
	nfa_size++;
}

void reg_to_nfa(string post){
	for(int i=0;i<post.size();i++){
		switch(post[i]){
			case 'a':
				character(post[i]-'a');
				break;
			case 'b':
				character(post[i]-'a');
				break;
			case '.':
				concat();
				break;
			case '+':
				_union();
				break;
			case '*':
				kleen_star();
				break;
		}		
	}
}

void displayNFA(){
	cout << "State\t|\ta\t|\tb\t|\teps\t|\tFinal State" << endl;
	for(int i=0;i<nfa.size();i++){
		cout << i << "\t|\t";
		for(int j=0;j<nfa[i].a[0].size();j++)
			cout << nfa[i].a[0][j] << " ";
		cout << "\t|\t";
		for(int j=0;j<nfa[i].a[1].size();j++)
			cout << nfa[i].a[1][j] << " ";
		cout << "\t|\t";
		for(int j=0;j<nfa[i].e.size();j++)
			cout << nfa[i].e[j] << " ";
		cout << "\t|\t";
		if(nfa[i].f)
			cout << "yes";
		else
			cout << "no"; 
		cout << endl;
	}
}

void epsilon_closure(int state,set<int> &si){
	for(int i=0;i<nfa[state].e.size();i++){
		if(si.find(nfa[state].e[i])==si.end()){
			si.insert(nfa[state].e[i]);
			epsilon_closure(nfa[state].e[i],si);
		}
	}
}

set<int> state_change(set<int> &si,int c){
	set<int> res;
	if(c==0){
		for(set<int>::iterator it=si.begin();it!=si.end();it++){
			for(int i=0;i<nfa[*it].a[0].size();i++)
				res.insert(nfa[*it].a[0][i]);
		}
	}
	else{
		for(set<int>::iterator it=si.begin();it!=si.end();it++){
			for(int i=0;i<nfa[*it].a[1].size();i++)
				res.insert(nfa[*it].a[1][i]);
		}
	}
	return res;
}

void nfa_to_dfa(int start_state){
	set<int> si,temp;
	queue<set<int> > q;
	map<set<int>,int> mp;
	mp[si]=-1;
	int p=0,ct=0;
	si.insert(start_state);
	epsilon_closure(start_state,si);
	mp[si]=ct++;
	q.push(si);
	while(!q.empty()){
		dfa.push_back(init_dfa_state);
		bool f=false;
		si=q.front();
		for(set<int>::iterator it=si.begin();it!=si.end();it++){
			if(nfa[*it].f){
				f=true;
				break;
			}
		}
		dfa[p].f=f;
		for(int k=0;k<2;k++){
			si=q.front();
			temp=state_change(si,k);
			si=temp;
			for(set<int>::iterator it=si.begin();it!=si.end();it++)
				epsilon_closure(*it,si);
			if(mp.find(si)==mp.end()){
				mp[si]=ct;
				dfa[p].a[k]=ct;
				q.push(si);
				ct++;
			}else
				dfa[p].a[k]=mp[si];
		}
		q.pop();
		p++;	
	}
	dfa.push_back(init_dfa_state);
	dfa[p].a[0]=p;
	dfa[p].a[1]=p;
	for(int i=0;i<dfa.size();i++){
		if(dfa[i].a[0]==-1)
			dfa[i].a[0]=p;
		if(dfa[i].a[1]==-1)
			dfa[i].a[1]=p;
	}
}

void displayDFA(){
	cout << "State\t|\ta\t|\tb\t|\tFinal State" << endl;
	for(int i=0;i<dfa.size();i++){
		cout << i << "\t|\t";
		cout << dfa[i].a[0]; 
		cout << "\t|\t";
		cout << dfa[i].a[1];
		cout << "\t|\t";
		if(dfa[i].f)
			cout << "yes";
		else
			cout << "no"; 
		cout << endl;
	}
}

int main(){
	string regex;
	cin >> regex;
	string conCat=insert_concat(regex);
	cout << "After inserting concat(.) symbol: " << conCat << endl;
	string post=postFix(conCat);
	cout << "After converting to postfix: " << post << endl;
	reg_to_nfa(post);
	int final_state=st.top();
	st.pop();
	int start_state=st.top();
	st.pop();
	nfa[final_state].f=true;
	cout << "NFA: " << endl;
	displayNFA();
	cout << endl;
	cout << "DFA: " << endl;
	nfa_to_dfa(start_state);
	displayDFA();
	return 0;
}
