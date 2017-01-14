#include <iostream>

using namespace std;

class weightedQU{
	private:
		int size,*arr,*sz;
		int findRoot(int i){ // depth of any node is atmost log n
			while(i!=arr[i]){
				arr[i]=arr[arr[i]]; // to flatten the tree
				i=arr[i];
			}
		}
	public:
		weightedQU(int size){
			this->size=size+1;
			arr = new int[this->size];
			sz = new int[this->size];
			init();
		}
		void init(){
			for(int i=1;i<=size;i++){
			arr[i]=i;
			sz[i]=1;
			}
		}
		bool isConnected(int p,int q){  // log n complexity
			return (findRoot(p)==findRoot(q));
		}
		void addUnion(int p,int q){
			int proot=findRoot(p);
			int qroot=findRoot(q);
			if(sz[proot]<sz[qroot]){
				arr[proot]=qroot;
				sz[qroot]+=sz[proot];
			}
			else{
				arr[qroot]=proot;
				sz[proot]+=sz[qroot];
			}
		}
};


int main(){
	int n,p,q;
	int ch;
	cout << "Enter no elements: ";
	cin >> n;
	weightedQU qf(n);
	do{
		cout << endl <<  "1. Add Union" << endl << "2. Check connectivity" << endl << "3. Exit" << endl << "Enter choice: ";
		cin >> ch;
		switch(ch){
			case 1: 
				cout << "Enter the two elements to be connected: ";
				cin >> p >> q;
				qf.addUnion(p,q);
				break;
			case 2:
				cout << "Enter the two elements to be tested: ";
				cin >> p >> q;
				if(qf.isConnected(p,q))
				cout << "True" << endl;
				else
				cout << "False" << endl;
				break;
		}
	}while(ch!=3);
	return 0;
}
