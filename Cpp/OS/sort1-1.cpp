// CSAD
// Asuncion, Mark Anthony
#include <iostream>
#include <sstream>
#include <limits>

using namespace std;

void ascsort(float x[], int size){
    for(int i=0;i<size;i++){
        for(int j=i;j<size;j++){
            if(x[i] > x[j]){
                float tmp = x[i];
                x[i] = x[j];
                x[j] = tmp;
            }
        }   
    }
}

// use ascsort first
void dscsort(float x[], int size){
    cout << "{";
	for(int i=size-1;i>=0;i--){
        cout << x[i];
		if(i > 0){
			cout << ", ";
		}
	}
	cout << "}";
}

void clrscr(){
    for(int i=0;i<50;i++){
        cout << endl;
    }
}
void printarr(float x[], int size){
	cout << "{";
    for(int i=0;i<size;i++){
        cout << x[i];
		if(i < size-1){
			cout << ", ";
		}
    }
	cout << "}";
}

void cont(){
	cout << endl;
    cout << "Press Enter to continue";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.peek();
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clrscr();
}


string name = "Asuncion, Mark Anthony\nCSAD\n\n";
int main()
{
    float arr[10];
	string c = name;
    for(int i =0;i<10;i++){		
		clrscr();
        while(true){
			stringstream cache;
			cout << c;
            cout << "Enter number " << i+1 << '/' << 10 << ": ";
            if(cin.peek() == 10){
                cout << "Wrong Input. Please Input a number." << endl;
                cont();
                continue;
            }
            cin >> arr[i];
            
            if (cin.fail()){
                cout << "Wrong Input. Please Input a number." << endl;
                cont();
                continue;
            }
			cache << "Enter number " << i+1 << '/' << 10 << ": " << arr[i] << endl;
			string cc;
			while(cache >> cc){
				c += cc + ' ';				
			}
			c += '\n';
			cache.clear();
            break;
        }
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
	cout << endl;
	ascsort(arr, 10);
	cout << "Ascending Sort: ";
	printarr(arr, 10);
	cout << endl;
	cout << "Descending Sort: "; 
	dscsort(arr, 10);

    return 0;
}
