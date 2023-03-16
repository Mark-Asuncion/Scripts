// Asuncion, Mark Anthony
// CSAD
#include <iostream>
#include <sstream>
#include <limits>
#include <cstring>

void ascsort(float x[],int size){
	for(int i=0;i<size-1;i++){
		for(int j=0;j<size-(i+1);j++){
			if(x[j] > x[j + 1]){
				float tmp = x[j];
				x[j] = x[j+1];
				x[j+1] = tmp;
			}
		}
	}
}
void dscsort(float x[], int size){
	for(int i=0;i<size-1;i++){
		for(int j=0;j<size-(i+1);j++){
			if(x[j] < x[j + 1]){
				float tmp = x[j];
				x[j] = x[j+1];
				x[j+1] = tmp;
			}
		}
	}
}

void clrscr(){
    for(int i=0;i<50;i++){
        std::cout << std::endl;
    }
}

void cont(){
	std::cout << std::endl;
    std::cout << "Press Enter to continue";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.peek();
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clrscr();
}

void printarr(float x[], int size){
	for(int i =0;i<size;i++){
		std::cout << x[i] << " ";
	}
	std::cout << std::endl;
}

std::string label = "PL-M1: ACT2 - Sorting 2\nAsuncion, Mark Anthony.\n\n";
int sinput(){
	int size;
	while(true){
		clrscr();
		std::cout << label;
		std::cout << "Enter size: ";
		if(std::cin.peek() == 10){
			std::cout << "Wrong Input. Please Input a number." << std::endl;
			cont();
			continue;
		}
		std::cin >> size;
		
		if(std::cin.fail()){
			std::cout << "Wrong Input. Please Input a number." << std::endl;
			cont();
			continue;
		}
		else if(size <= 0){
			std::cout << "Wrong Input. Please input a positive number." << std::endl;
			cont();
			continue;
		}
		break;
	}
	return size;
}
float ainput(int curr, int total){
	float num;
	while(true){
		clrscr();
		std::cout << label;
		std::cout << "Input "<< curr+1 << '/' << total << " number: ";
		if(std::cin.peek() == 10){
			std::cout << "Wrong Input. Please Input a number." << std::endl;
			cont();
			continue;
		}
		std::cin >> num;
		if(std::cin.fail()){
			std::cout << "Wrong Input. Please Input a number." << std::endl;
			cont();
			continue;
		}
		break;
	}
	return num;
}

int main(){
	int size = sinput();
	cont();
	float arr[size],ascarr[size],dscarr[size];
	for(int i=0;i<size;i++){
		arr[i] = ainput(i,size);
		ascsort(arr, i+1);
		std::cout << "Ascending Array: ";
		printarr(arr, i+1);
		if(i == size-1){
			memcpy(ascarr,arr,size * sizeof(int));
		}
		
		dscsort(arr,i+1);
		std::cout << "Descending Array: ";
		printarr(arr, i+1);
		if(i == size-1){
			memcpy(dscarr,arr,size * sizeof(int));
		}
		cont();
	}
	std::cout << "Final Array" << std::endl;
	std::cout << "Ascending Array: ";
	printarr(ascarr, size);
	std::cout << "Descending Array: ";
	printarr(dscarr, size);
	std::cout << std::endl;
	std::cout << "Program has ended." << std::endl;
}
