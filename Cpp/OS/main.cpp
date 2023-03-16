#include <iostream>
// #include <windows.h>
#include "ccs2203.h"
using namespace std;

// memcpy(destination, source, size to copy (in Bytes))

// Cursor Methods
// HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
// COORD cpos;
// CONSOLE_SCREEN_BUFFER_INFO cbsi;
// COORD getXY(){
//     GetConsoleScreenBufferInfo(h, &cbsi);
//     return cbsi.dwCursorPosition;
// }
// void setXY(int x, int y) { 
//     cpos.X = x; 
//     cpos.Y = y; 
//     SetConsoleCursorPosition(h,cpos); 
// }
// void showCursor(bool x){
//     CONSOLE_CURSOR_INFO cursor;
//     GetConsoleCursorInfo(h,&cursor);
//     cursor.bVisible = x;
//     SetConsoleCursorInfo(h,&cursor);
// }
// end


int main(){
    clrscr();
    Table table(new vector<string>{"Hello","World"},Color::BOLD);
    table.push_row({"1","2"});
    table.print();
    cout << endl;
    table.del_header(1);
    table.print();
    cout << endl;
    table.push_header(Color::BOLD,"world");
    table.push_header(Color::BOLD,"asd");
    table.print();
    cout << endl;

}