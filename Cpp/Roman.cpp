#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

string roman(int n){
	vector<int> romnums = {1,4,5,9,10,40,50,90,100,400,500,900,1000};
	vector<string> romchar = {"I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M"};
	string final = "";
	while(n){
	    for(int i=romchar.size()-1;i>=0;i--){
	        if(romnums.at(i) > n) continue;
	        n -=romnums.at(i);
	        final += romchar.at(i);
	    }
	}
	return final;
}

int main(){
	int x;
	cout << "Input: ";
	cin >> x;
	cout << roman(x);
}
