#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> vint;
struct MergeSort{
    vint sort(vint val){
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
    vint merge(vint first_half, vint second_half){
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
int main()
{
    // vint x = {5,42,12,4,2,1,4};
    // MergeSort a;
    // x = a.sort(x);
    // for(auto i : x){
    // 	cout << i << " ";
    // }
    // int cs[5] = {2,1,0,1,0};
    int cs[5] = {0,2,1,2,0};
    sort012(cs,5);
    for(int i=0;i<5;i++){
        cout << cs[i] << " ";
    }

    return 0;
}
