#include <iostream>

using namespace std;

void swap(int *a,int *b){
	int temp=(*a);
	(*a)=(*b);
	(*b)=temp;
}

void selectionSort(int arr[],int length){
	for(int i=0;i<length;i++){
		int pos=i;
		for(int j=i+1;j<length;j++)
			if(arr[j]<arr[pos])
				pos=j;
		swap(&arr[i],&arr[pos]);
	}
}

void bubbleSort(int arr[],int length){
	for(int i=0;i<length-1;i++)
		for(int j=0;j<length-i-1;j++)
			if(arr[j]>arr[j+1])
				swap(&arr[j],&arr[j+1]);
}

void insertionSort(int arr[],int length){
	for(int i=1;i<length;i++){
		int j=i,temp=arr[j];
		while(j>=1 && temp<arr[j-1])
			arr[j--]=arr[j];
		arr[j]=temp;
	}
}



int main(){
	int arr[]={7,3,56,1,11,3,2,10},size=sizeof(arr)/sizeof(int);
	insertionSort(arr,size);
	for(int i=0;i<size;i++)
		cout << arr[i] << endl;
	return 0;
}
