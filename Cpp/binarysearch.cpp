#include <algorithm>
#include <iostream>
#include <vector>

int binarySearch(std::vector<int>* arr,int n) {
    int start = 0;
    int end = arr->size();
    while (start <= end) {
        int half = (start + end) / 2;
        int curr = (*arr)[half];
        if (curr == n) return half;
        else if (curr < n) start = half + 1;
        else end = half - 1;
    }
    return -1;
}
int main() {
    std::vector<int> arr = {1,3,4,7,11,16,20,23,25,30};
    int find = 23;

    std::cout << "[ ";
    for (auto& i : arr) {
         std::cout << i << ", ";
    }
    std::cout << "]\n";

    std::cout << "Index of " << find << " is " << binarySearch(&arr, find) << '\n';
    return 0;
}
