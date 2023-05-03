#include <iostream>
#include "ccs2203.h"
using namespace std;
using namespace Memory;
using namespace Color;
using Format::ss;

void p_process(deque<Process> &process,vector<int>& to_update,Table& process_table, int &curr_time){
    int min_arrive=0;
    for(int i=0;i<process.size();i++){
            if(process[i].isDone) continue;
            if(process[min_arrive].isDone) min_arrive = i;
            else if(process[min_arrive].arrival_time > process[i].arrival_time) min_arrive = i;
            if(curr_time < process[i].arrival_time) continue;
            bool is = false;
            for(int j=0;j<to_update.size();j++){
                int jj = to_update[j];
                if(process[i].id == process[jj].id) {
                    is = true;
                    break;
                }
            }
            
            if (is) continue;
            to_update.push_back(i);
            
    }
    string p_name="",p_ct="";
    
    if(!to_update.empty()){
        auto p_sort = [&process](int a, int b) -> bool{
            bool c = process[a].burst_time < process[b].burst_time;
            if (c) {
                return c;
            }
            else if (process[a].burst_time == process[b].burst_time){
                return process[a].arrival_time < process[b].arrival_time;
            }
            return false;
            // return (c)? c:process[a].arrival_time < process[b].arrival_time;
        };
        sort(to_update.begin(),to_update.end(),p_sort);
        // for(auto i : to_update) cout << i+1 << ' ';
        // cout << '\n';
        int cct = curr_time + process[to_update[0]].burst_time;
        process[to_update[0]].completion_time = cct;
        curr_time = cct;
        p_name += to_string(process[to_update[0]].id);
        p_ct += to_string(cct);
        process[to_update[0]].isDone = true;
        to_update.erase(to_update.begin());
    }
    else {
        // to_update empty means there is no jobs to process in this current time
        // update curr_time to the lowest arrival_time
        if (process[min_arrive].isDone) return;
        p_name = "idle";
        p_ct = to_string(process[min_arrive].arrival_time);
        curr_time = process[min_arrive].arrival_time;
    }
    if(p_ct.size() > process_table.getWidth()) process_table.setWidth(p_ct.size()); // set new table cell width
    // update table
    process_table.push_col({p_name,p_ct});
}

int main(){
    act_name = "FL-M5: ACT2 - SJN";
    // input n-process
    string ms = "";
    int n_process = input<int>("Enter number of process: ",ms,false);
    deque<Process> process;
    Table table({"Processes","Arrival","Burst","CT","TAT","WT"},BOLD);
    // conts();
    // enter arrival time
    ss << "Arrival Time\n";
    ms += ss.str();
    ss.str(string());
    for(int i=0;i<n_process;i++){
        int arrival_time = input<int>("Enter arrival time of P" + to_string(i+1) + ": ",ms,false);
        ss << "Enter arrival time of P" << to_string(i+1) << ": " << arrival_time << '\n';
        ms += ss.str();
        process.push_back({.id = i+1,.arrival_time = arrival_time, .isDone = false});
        ss.str(string());
    }
    // conts();
    // enter burst time
    ms = "";
    ss << "Burst Time\n";
    ms += ss.str();
    ss.str(string());
    for(int i=0;i<n_process;i++){
        int burst_time = input<int>("Enter burst time of " + to_string(i+1) + ": ",ms,false);
        ss << "Enter burst time of " << to_string(i+1) << ": " << burst_time << '\n';
        ms += ss.str();
        process[i].burst_time = burst_time;
        ss.str(string());
    }
    
    // Table process_table(new vector<string>{"Process","Arrival","Burst"},{BOLD});
    // process_table.setWidth(6);
    for (deque<Process>::iterator i=process.begin();i!=process.end();i++){
        table.push_row( {
                        to_string(i->id),
                        to_string(i->arrival_time), 
                        to_string(i->burst_time) 
        } );
    }
    label();
    table.print();
    conts();
    // process_table.print();
    Table process_table;
    process_table.push_header("");
    process_table.push_row({"0"});
    // process_table.setWidth(5);
    process_table.print();
    vector<int> to_update; // idx
    int curr_time = 0;
    auto isDone = [&process]() -> bool {
        deque<Process>::iterator i;
        for(i=process.begin();i!=process.end();i++){
            if(!i->isDone) return false;
        }
        return true;
    };
    auto str_center = [](string str, int width) -> string{
        width -= str.size();
        width /= 2;
        return string(width,' ') + str;
    };
    while(1){
        if(isDone()) break;
        clrscr();
        label();
        p_process(process,to_update,process_table,curr_time);
        
        string n = to_string(process_table.colSize());
        int w = (process_table.getWidth()+1) * process_table.rowSize() + n.size();

        cout << '+' << setfill('-') << setw(w) << '-' << "+\n";
        cout << '|' << setfill(' ') << setw(w) << std::left << str_center("Timeline",w) << "|\n";
        process_table.print();
        conts();
    }
    label();
    // calc tat, wt here
    // TAT and WT
    float ave_tat = 0.0, ave_wt = 0.0;
    for(int i=0;i<process.size();i++){
        int tat = process[i].completion_time - process[i].arrival_time;
        int wt = tat - process[i].burst_time;

        // process[i].tat = tat;
        // process[i].wt = wt;
        int colidx = table.rowSize();
        table.edit_col(i+1,colidx-3,to_string(process[i].completion_time));
        table.edit_col(i+1,colidx-2,to_string(tat));
        table.edit_col(i+1,colidx-1,to_string(wt));
        
        ave_tat += tat;
        ave_wt += wt;
    }
    ave_tat /= process.size();
    ave_wt /= process.size();
    table.print();
    cout << '\n';
    process_table.print();
    cout << '\n';
    ave_tat = Format::truncate(ave_tat,2);
    ave_wt = Format::truncate(ave_wt,2);
    cout << "Average Turn Around Time: " << Format::toString(ave_tat) << '\n';
    cout << "Average Waiting Time: " << Format::toString(ave_wt) << '\n';

    return 0;
}