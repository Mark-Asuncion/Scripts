// moors voting algorithm
#include <iostream>
#include <vector>
using namespace std;

int majorityElement(vector<int>&& arr){
    int votes = 0;
    int candidate = -1;
    // finds the major candidate
    // meaning the value that has the most highest repeat count
    for(int i = 0;i<arr.size();i++){
        if (votes == 0) { 
            candidate = arr[i]; 
            votes = 1;
        }
        else {
            if (arr[i] == candidate) { votes++; }
            else { votes--; }
        }
    }
    // count the repeat of the candidate
    // return if candidate if it satisfies the condition
    // else return -1;
    int condition = arr.size()/2;
    int count = 0;
    for (int i = 0;i<arr.size();i++){
        if(arr[i] == candidate) count++;
        if(count > condition) return candidate;
    }
    return -1;
}


int main(){
    cout << majorityElement({3,1,3,3,2});
    cout << majorityElement({3,1,3,2,3});
    return 0;
}