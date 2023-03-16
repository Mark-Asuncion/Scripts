// CSAD
// Asuncion, Mark Anthony
#include <iostream>
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


void dscsort(float x[], int size){
    for(int i=size-1;i>=0;i--){
        cout << x[i] << " ";
    }
}

void clrscr(){
    for(int i=0;i<50;i++){
        cout << endl;
    }
}
void printarr(float x[], int size){
    for(int i=0;i<size;i++){
        cout << x[i] << " ";
    }
}

void cont(){
    cout << "Press Enter to continue";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.peek();
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clrscr();
}

string name = "CSAD\nAsuncion, Mark Anthony\n\n";
int main()
{
    clrscr();
    float arr[10];
    // string c = name;
    for(int i =0;i<10;i++){
        // bool isValid = true;
        int currsize = i+1;
        while(true){
            cout << name;
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
            break;
        }
        cout << endl;
        ascsort(arr,currsize);
        cout << "Ascending Sort: ";
        printarr(arr, currsize);
        cout << endl;
        cout << "Descending Sort: "; 
        dscsort(arr, currsize);
        cout << endl;
        
        cont();
    }

    return 0;
}
