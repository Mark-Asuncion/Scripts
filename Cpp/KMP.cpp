#include <iostream>
using namespace std;

// KMP Algo

void LPS(string pat,int container[]){
    // Finds if there is a suffix and prefix that are the same
    // and stores the next index so it doesn't have to check again 
    int l = 0;
    int r = 1;
    int size = pat.size();
    while(r<size){
        if (pat[l] == pat[r]) {
            container[r] = l+1;
            r++;
            l++;
        }
        else{
            if (l == 0){
                container[r] = 0;
                r++;
            }
            else{
                l = container[l-1];
            }
        }
    }
}
int findPattern(string pat, string text){
    // Find the index of pattern using KMP Algorithm
    // get LPS first
    int lps[pat.size()] = {0};
    LPS(pat,lps);
    int text_idx = 0;
    int pat_idx = 0;
    // for (int i =0;i<pat.size();i++) cout << lps[i] << ' ';
    // cout << '\n';
    // check text and pat by there index
    // increase both index if it match
    // else 
    //  if pat index is 0 increase text index
    //  else get pat_idx = lps[pat_idx - 1]
    // remember that LPS stores the if it has the same suffix and prefix
    // lps elements points to next index of where to check
    // example:
    //  abcaby
    //  000120
    //  if it doesn't match at index 4 means that index - 1 is equal to a
    //  so get the lps[i-1] and make that the new pat_idx for checking
    while (text_idx < text.size()){
        if (text[text_idx] == pat[pat_idx]){
            text_idx++;
            pat_idx++;
        }
        else {
            if(pat_idx == 0) {
                text_idx++;
            }
            else {
                pat_idx = lps[pat_idx-1];
            }
        }
        if (pat_idx >= pat.size()) return text_idx - pat_idx;
    }
    return -1;
}

int main(){
    cout << findPattern("abcaby","abxabcabcaby") << '\n';
    cout << findPattern("ABABCABAB","ABABDABACDABABCABAB") << '\n';
    cout << findPattern("ABABAB","ABXBABABABC") << '\n';
    cout << findPattern("ababa","ababbababa") << '\n';
    return 0;
}