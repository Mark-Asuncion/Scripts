#include <iostream>
#include <vector>
#include <limits.h>
#include <cstring>
using namespace std;

struct MergeSort{
    typedef vector<int> vint;
    static vint sort(vint& val){
        if (val.size() == 1){
            return val;
        }
        vint first_half(val.begin(),val.begin() + (val.size()/2));
        // for(int i = 0;i<val.size()/2;i++){
        //     first_half.push_back(val[i]);
        // }
        vint second_half(val.begin() + (val.size()/2),val.end());
        // for(int i = val.size() / 2;i<val.size();i++){
        //     second_half.push_back(val[i]);
        // }
        first_half = sort(first_half);
        second_half = sort(second_half);
        return  merge(first_half,second_half);
    }
    static vint merge(vint first_half, vint second_half){
        int idx1 = 0;
        int idx2 = 0;
        vint ret;
        while(idx1 < first_half.size() && idx2 < second_half.size()){
            if(first_half[idx1] < second_half[idx2]){
                ret.push_back(first_half[idx1]);
                idx1++;
            }
            else{
                ret.push_back(second_half[idx2]);
                idx2++;
            }
        }

        while(idx1 < first_half.size()){
            ret.push_back(first_half[idx1]);
            idx1++;
        }
        while(idx2 < second_half.size()){
            ret.push_back(second_half[idx2]);
            idx2++;
        }
        return ret;
    }
};
//only works with known range of element
// in this case 0 - 2
void sort012(int a[], int n)
{
    //counting sort
    int k[3] = {0};
    for(int i=0;i<n;i++){
        k[a[i]]++;
    }
    //cumulative sum
    for(int i=1;i<3;i++){
        k[i] += k[i-1];
    }
    //shift right
    for(int i=2;i>0;i--){
        k[i] = k[i-1];
    }
    k[0] = 0;
    int f[n];
    memcpy(f,a,n*sizeof(int));
    for(int i=0;i<n;i++){
        a[k[f[i]]] = f[i];
        k[f[i]]++;
    }
}

int getMax(vector<int>& input)
{
    int m = INT_MIN;
    for (auto& i : input)
        m = max(i,m);
    return m;
}

void countSort(vector<int>& arr, int d)
{
    vector<int> out(arr.size(),0);
    int count[10] = {0};
    int i=0;
    // count occurences
    for (auto& j: arr)
    {
        count[( j / d ) % 10]++;
        ++i;
    }

    // obtain cumulative sum
    for(i=1;i<10;++i)
        count[i] += count[i-1];

    // shift right
    for(i=9;i>=1;--i)
        count[i] = count[i-1];
    count[0] = 0;

    for(auto& j: arr)
    {
        out[count[( j / d ) % 10]] = j;
        count[( j / d ) % 10]++;
    }

    for(i=0;i<arr.size();++i)
        arr[i] = out[i];
}

void radix(vector<int>& arr)
{
    int m = getMax(arr);
    for(int i=1; m / i > 0;i*=10)
        countSort(arr,i);
}

void print(vector<int>& arr)
{
    cout << "{ ";
    for (auto& i: arr)
        cout << i << ' ';
    cout << '}' << endl;
}
void print(int arr[], int size)
{
    cout << "{ ";
    for(int i=0;i<size;i++)
        cout << arr[i] << ' ';
    cout << '}' << endl;
}
int main()
{
    cout << "Merge Sort" << endl;
    vector<int> merge1 = {5,42,12,4,2,1,4};
    print(merge1);
    merge1 = MergeSort::sort(merge1);
    print(merge1);
    cout << string(10,'=') << endl;

    cout << "Count Sort (0,1,2)" << endl;
    int csin1[5] = {0,2,1,2,0};
    int csin2[5] = {2,1,0,1,0};
    cout << "Input 1" << endl;
    print(csin1,5);
    sort012(csin1,5);
    print(csin1,5);
    cout << "Input 2" << endl;
    print(csin2,5);
    sort012(csin2,5);
    print(csin2,5);
    cout << string(10,'=') << endl;

    cout << "Radix Sort" << endl;
    vector<int> radix1{ 543, 986, 217, 765, 329 };
    vector<int> radix2{ 170, 45, 75, 90, 802, 24, 2, 66 };
    vector<int> radix3{ 232, 45, 32, 90, 1032, 24, 0, 88 };
    cout << "Input 1" << endl;
    print(radix1);
    radix(radix1);
    print(radix1);

    cout << "Input 2" << endl;
    print(radix2);
    radix(radix2);
    print(radix2);

    cout << "Input 3" << endl;
    print(radix3);
    radix(radix3);
    print(radix3);
    cout << string(10,'=') << endl;
    return 0;
}
