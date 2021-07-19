#include <iostream>
#include <vector>

using namespace std;

vector<int> preProcess(string pattern){
	int N = pattern.size();
	vector<int> lps(N);
	cout << N << endl;
	lps[0]=0;
	int j=0,i=1;
	while(i<N){
		if(pattern[i]==pattern[j]){
			lps[i]=j+1;
			j++;
			i++;
		}else{
			if(j!=0)
				j=lps[j-1];
			else{
				lps[i]=0;
				i++;
			}
		}
	}
	for(int i=0;i<lps.size();i++)
		cout << lps[i] << " ";
	cout << endl;
	return lps;
}

vector<int> kmp(string text, string pattern){
	vector<int> lps = preProcess(pattern);
	int i=0,j=0;
	vector<int> ans;
	while(i<text.size()){
		if(text[i]==pattern[j]){
			i++;
			j++;
		}else{
			if(j!=0)
				j=lps[j-1]; // move one character back uptill where it matched
			else
				i++;
		}
		if(j==pattern.size()){
			ans.push_back(i-pattern.size());
			// there was no mismatch
			// backtrack to lps[N-1]
			j=lps[j-1]; 
		}	
	}
	return ans;
}

int main(){
	string text="abcabcab";
	string pattern = "abcab";
	vector<int> positions = kmp(text,pattern);
	for(int i=0;i<positions.size();i++)
		cout << positions[i] << " ";
	cout << endl;
	return 0;
}