#include <iostream>

using namespace std;

struct Node{
	int data;
	Node *next;
};

class LinkedList{
	Node *first,*last;
	void deleteNode(Node *ptr,Node *prev){
		Node *temp=ptr;
		if(first==last)
			first=last=NULL;
		else if(ptr==first)
			first=ptr->next;
		else if(ptr==last)
			last=prev;
		delete temp;
	}
	Node* createNode(int data,Node *next){
		Node *temp = new Node();
		temp->data=data;
		temp->next=next;
		return temp;
	}
	public:
		LinkedList(){
			first=NULL;
			last=NULL;
		}
		void insertAtFront(int data){
			Node *newNode = createNode(data,first);
			if(first==NULL)
			last=newNode;
			first=newNode;
		}
		void insertAtEnd(int data){
			Node* newNode = createNode(data,NULL);
			if(last)
			last->next=newNode;
			else
				first=newNode;
			last=newNode;
		}
		void deleteAtFront(){
			if(first){
				Node *temp=first;
				if(first==last){
					first=last=NULL;
					delete temp;
					return;
				}
				first=first->next;
				delete temp;
			}
		}
		void deleteAtEnd(){
			if(last){
				Node *temp=last;
				if(first==last){
					last=first=NULL;
					delete temp;
					return;
				}
				Node *itr=first;
				while(itr->next!=last)
				itr=itr->next;
				itr->next=NULL;
				last=itr;
				delete temp;
			}
		}
		void deleteAtkPosition(int k){
			Node *itr=first,*prev=NULL,*next=NULL;
			while(--k && itr){
				prev=itr;
				itr=itr->next;
			}
			if(itr){
				next=itr->next;
				deleteNode(itr,prev);
			}
			if(prev)
			prev->next=next;
			
		}
		void deleteNodeAtData(int data){
			Node *itr=first,*prev=NULL,*temp=NULL;
			while(itr){
				temp=itr;
				itr=itr->next;
				if(temp->data==data){
					deleteNode(temp,prev);
					if(prev)
					prev->next=itr;
				}
				else
					prev=temp;
			}
		}
		void printList(){
			Node *itr=first;
			cout << "start->";
			while(itr){
				cout << itr->data << "->";
				itr=itr->next;
			}
			cout << "end" << endl;
			cout << "First Node: ";
			first?cout << (first->data) << endl:cout << "NULL" << endl;
			cout << "Last Node: ";
			last?cout << (last->data) << endl:cout << "NULL" << endl;
		}
};

int main(){
	LinkedList l1;
	l1.printList();
	l1.insertAtEnd(1);
	l1.printList();
	l1.insertAtFront(2);
	l1.printList();
	l1.insertAtEnd(3);
	l1.printList();
	l1.insertAtFront(4);
	l1.printList();
	l1.insertAtEnd(4);
	l1.printList();
	l1.insertAtEnd(5);
	l1.printList();
	l1.deleteNodeAtData(4);
	l1.printList();
	l1.insertAtEnd(6);
	l1.deleteAtkPosition(1);
	l1.printList();
	l1.deleteAtFront();
	l1.deleteAtEnd();
	l1.printList();
	l1.deleteAtFront();
	l1.printList();
	l1.deleteAtFront();
	l1.printList();
	return 0;
}
