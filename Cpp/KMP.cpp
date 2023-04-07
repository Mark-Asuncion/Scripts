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
    return 0;
}