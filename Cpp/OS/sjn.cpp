#include <iostream>
#include "ccs2203.h"
using namespace std;
using namespace Memory;
using namespace Color;
using Format::ss;

void calc_tat(vector<Process*> &process, Table& table);
void calc_wt(vector<Process*> &process, Table& table);

void allocate(vector<Process*> &process, Table& table, int& curr_time){
    
    for (int i=0;i<process.size();i++){
        if (curr_time != process[i]->arrival_time) continue;
        
    }
}

int main(){
    act_name = "FL-M4: ACT2 - SJN";
    // input n-process
    string ms = "";
    int n_process = input<int>("Enter number of process: ",ms,false);
    deque<Process> process;
    Table table({"Processes","Arrival","Burst","CT","TAT","WT"},{BOLD});
    // table.push_row({""});
    table.setWidth(11);
    // table.print();
    // conts();
    // enter arrival time
    ss << "Arrival Time\n";
    ms += ss.str();
    ss.str(string());
    for(int i=0;i<n_process;i++){
        int arrival_time = input<int>("Enter arrival time of P" + to_string(i+1) + ": ",ms,false);
        ss << "Enter arrival time of P" << to_string(i+1) << ": " << arrival_time << '\n';
        ms += ss.str();
        process.push_back({i+1,arrival_time,0,0});
        ss.str(string());
    }
    // conts();
    // enter burst time
    ms = "";
    ss << "Burst Time\n";
    ms += ss.str();
    ss.str(string());
    for (int i=0;i<n_process;i++){
        int burst_time = input<int>("Enter burst time of " + to_string(i+1) + ": ",ms,false);
        ss << "Enter burst time of " << to_string(i+1) << ": " << burst_time << '\n';
        ms += ss.str();
        process[i].burst_time = burst_time;
        ss.str(string());
    }
    
    // Table process_table(new vector<string>{"Process","Arrival","Burst"},{BOLD});
    // process_table.setWidth(6);
    vector<Process*> arrival_sorted_process;
    for (deque<Process>::iterator i=process.begin();i!=process.end();i++){
        table.push_row( {
                        to_string(i->id),
                        "",
                        to_string(i->arrival_time), 
                        to_string(i->burst_time) 
        } );
        arrival_sorted_process.push_back(&(*i));
    }
    label();
    table.print();
    // process_table.print();
    Table process_table;
    process_table.push_header("");
    process_table.push_row({"0"});
    process_table.setWidth(3);
    process_table.print();
    int curr_time = 0;

    return 0;
}