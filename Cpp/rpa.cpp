/*
* Implementation of RUSSIAN PEASANT ALGORITHM (RPA) Algorithm
* This program is created for Numerical Analysis for ITE class in my 4th Year 1st Sem
*/

#include <exception>
#include <iostream>
#include <limits>
#include <string>
#include <tuple>
#include <vector>
#include <cassert>

#define CLEAR "\x1B[2K\x1B[1A\x1B[2K"
#define NORMAL "\x1B[0m"
#define FORE_GREEN "\x1B[0;32m"
// #define CLEAR "\x1B[2J\x1B[H"

using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::tuple;
using std::make_tuple;

void get_input_int(const char* prompt, int& ret);
string center(std::string v, int width);
void print_answer(int answer);
void print(int index, bool is_odd = false);
void press_enter_to_continue();

#define ID_MULTIPLICAND 0
#define ID_MULTIPLIER 1

int cell_width   = 0;
int multiplicand = 0;
int multiplier   = 0;
vector<tuple<int, int>> res{0};

int main() {
    cout << "Asuncion, Mark Anthony" << '\n'
        << "P-M1: ACT4 - RPA Implementation " << "\n\n";

    get_input_int("Input Multiplicand:", multiplicand);
    get_input_int("Input Multiplier:", multiplier);
    res.push_back(make_tuple(multiplicand, multiplier));

    bool is_multiplicand_gt = multiplicand > multiplier;

    while (1) {
        int multiplicand = std::get<ID_MULTIPLICAND>(res.back());
        int multiplier = std::get<ID_MULTIPLIER>(res.back());

        if (is_multiplicand_gt) {
            int half = multiplier / 2;
            int dbl = multiplicand * 2;
            res.push_back(make_tuple(dbl, half));
            if (half == 1) {
                break;
            }
        }
        else {
            int half = multiplicand / 2;
            int dbl = multiplier * 2;
            res.push_back(make_tuple(half, dbl));
            if (half == 1) {
                break;
            }
        }
    }

    int sum_odd = 0;
    for (size_t i=0;i<res.size();i++) {
        bool is_odd;
        int num;

        int multiplicand = std::get<ID_MULTIPLICAND>(res[i]);
        int multiplier = std::get<ID_MULTIPLIER>(res[i]);

        if (is_multiplicand_gt) {
            is_odd = multiplier % 2 != 0;
            num = multiplicand;
        }
        else {
            is_odd = multiplicand % 2 != 0;
            num = multiplier;
        }

        if (is_odd) {
            sum_odd += num;
        }

        print(i, is_odd);
        press_enter_to_continue();
    }
    print_answer(sum_odd);
    cout << multiplicand << " * " << multiplier << " = " << sum_odd << '\n';

    return 0;
}

void get_input_int(const char* prompt, int& ret) {
    while (1) {
        string in = "";
        cout << prompt << ' ';
        char peek = cin.peek();
        if (peek == '\n') {
            cout << CLEAR;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        cin >> in;

        try {
            ret = std::stoi(in);
            break;
        }
        catch (std::exception e){
            // cout << "[ERROR]" << e.what() << '\n';
            // cout << CLEAR;
        }

        cout << CLEAR;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void print_answer(int answer) {
    cout << string(cell_width * 2 + 3, '-') << '\n'
        << '|' << center(std::to_string(answer), cell_width * 2 + 1)
        << "|\n" << string(cell_width * 2 + 3, '-') << '\n';
}

void print(int index, bool is_odd) {
    int multiplicand = std::get<ID_MULTIPLICAND>(res[index]);
    int multiplier = std::get<ID_MULTIPLIER>(res[index]);

    string str_multiplicand = std::to_string(multiplicand);
    string str_multiplier = std::to_string(multiplier);

    if (cell_width == 0 && index == 0) {
        cell_width = std::max(
            str_multiplicand.size(),
            str_multiplier.size()
        ) + 10;
    }

    string odd_str = "";
    string color = "";
    if (is_odd) {
        odd_str = "<<";
        color = FORE_GREEN;
    }

    cout << string(cell_width * 2 + 3, '-') << '\n'
        << '|' << color << center(str_multiplicand, cell_width) << NORMAL
        << '|' << color << center(str_multiplier, cell_width)
        << NORMAL "| " << color << odd_str << NORMAL << '\n';
}

string center(std::string v, int width) {
    int ws = width - v.size();
    int lpad = ws / 2;
    return string(lpad, ' ') + v + string(ws - lpad, ' ');
}

void press_enter_to_continue() {
    cout << "Press Enter To Continue...";
    char peek = cin.peek();
    if (peek == '\n' || peek == ' ') {
        cout << CLEAR;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
