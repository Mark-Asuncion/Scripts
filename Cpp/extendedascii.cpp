#include <iostream>
#include <iomanip>

int main(){
    std::cout <<'+' << std::setfill('-') << std::setw(11) << '-' << '+' << std::endl;
	for(int i=1;i<256;i++){
        if(i >= 7 && i <= 10 || i == 13) continue;
        std::cout <<'|' << std::setfill(' ') << std::setw(5) << char(i) << '|';
        std::cout << std::setfill(' ') << std::setw(5) << i << '|';
        std::cout << std::endl;
    }
    std::cout <<'+' << std::setfill('-') << std::setw(11) << '-' << '+' << std::endl;
    return 0;
}
