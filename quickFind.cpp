#include <iostream>

using namespace std;

class quickFind{
	private:
	int size,*arr;
	public:
	quickFind(int size){
		this->size=size+1;
		arr = new int[this->size];
		init();
	}
	void init(){
		for(int i=1;i<size;i++)
		arr[i]=i;
	}
	bool isConnected(int p,int q){
		return (arr[p]==arr[q]);
	}
	void addUnion(int p,int q){
		if(isConnected(p,q))
		return;
		int pid=arr[p];
		int qid=arr[q];
		for(int i=1;i<size;i++)
			if(arr[i]==qid)
				arr[i]=pid;
	}
};

int main(){
	int n,p,q;
	int ch;
	cout << "Enter no elements: ";
	cin >> n;
	quickFind qf(n);
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
