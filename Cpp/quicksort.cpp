#include <iostream>
using namespace std;

struct quicksort{
	quicksort(int arr[], int low, int high) {
		sort(arr, low, high);
	}
	void sort(int arr[],int low,int high){
		if(low>=high) return;
		int pi = partition(arr,low,high);
		sort(arr,low,pi-1);
		sort(arr,pi+1,high);
	}
	int partition(int arr[],int low,int high){
		int pivot = arr[high];
		int curr = low;
		for(int i = low;i<high;i++){
			if(arr[i] <= pivot){
				swap(arr[i],arr[curr]);
				curr++;
			}
		}
		swap(arr[curr],arr[high]);
		return curr;
	}
};


int main(){

	// int arr[]= {10, 7, 8, 9, 1, 5};
	int arr[]= {9, 8, 7, 5, 1, 10};
	quicksort(arr,0,5);
	for(int i =0;i<6;i++) cout << arr[i] << ' ';
	return 0;
}