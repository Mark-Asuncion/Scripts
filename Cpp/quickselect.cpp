#include <iostream>

using namespace std;

struct quickselect {
    // Time Complexity - O(n) | Worst-case of O(n^2)
    // Auxilliary Space - O(1)
    int sort(int arr[],int low, int high, int k){
        if(k-1 < 0 || k-1 > high) return -1;
        int pi = partition(arr,low,high);
        if (pi == k-1) return arr[pi];
        if(pi > k-1) return sort(arr,low,pi - 1,k);
        return sort(arr,pi + 1, high,k);
    }
    int partition(int arr[], int low, int high){
        int pivot = arr[high];
        int l = low;
        for(int i = low;i<high;i++){
            if(arr[i] > pivot) continue;
            swap(arr[l],arr[i]);
            l++;
        }
        swap(arr[l],arr[high]);
        return l;
    }
};

int main()
{
    int arr[] = { 10, 4, 5, 8, 6, 11, 26 };
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 3;
    quickselect obj;
    cout << obj.sort(arr,0,n-1,k);

    return 0;
}
