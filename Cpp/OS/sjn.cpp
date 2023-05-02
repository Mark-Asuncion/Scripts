#include <iostream>
#include "ccs2203.h"
using namespace std;
using namespace Memory;
using namespace Color;
using Format::ss;

void p_process(deque<Process> &process,vector<int>& to_update,Table& process_table, int &curr_time){
    int min_arrive=0;
    if (to_update.empty()){
        for(int i=0;i<process.size();i++){
            if(process[min_arrive].arrival_time < process[i].arrival_time) min_arrive = i;
            if(curr_time > process[i].arrival_time) continue;
            to_update.push_back(i);
        }
    }
    string p_name="",p_ct="";
    
    if(to_update.empty()){
        // to_update empty means there is no jobs to process in this current time
        // update curr_time to the lowest arrival_time
        p_ct = process[min_arrive].arrival_time;
        curr_time = process[min_arrive].arrival_time;
    }
    else{
         auto p_sort = [&process](int a, int b) -> bool{
            bool c = process[a].burst_time < process[b].burst_time;
            return (c)? c:process[a].arrival_time < process[b].arrival_time;
        };
        sort(to_update.begin(),to_update.end(),p_sort);
        int cct = curr_time + process[to_update[0]].burst_time;
        process[to_update[0]].completion_time = cct;
        curr_time = cct;
        p_name += to_string(process[to_update[0]].id);
        p_ct += to_string(cct);

        to_update.erase(to_update.begin());
    }
    if(p_ct.size() > process_table.getWidth()) process_table.setWidth(p_ct.size()); // set new table cell width
    // update table
    process_table.push_col({p_name,p_ct});
}

int main(){
    act_name = "FL-M4: ACT2 - SJN";
    // input n-process
    string ms = "";
    int n_process = input<int>("Enter number of process: ",ms,false);
    deque<Process> process;
    Table table({"Processes","Arrival","Burst","CT","TAT","WT"},BOLD);
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
        process.push_back({i+1,arrival_time,0,0,false});
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
                        "",
                        to_string(i->arrival_time), 
                        to_string(i->burst_time) 
        } );
    }
    label();
    table.print();
    // process_table.print();
    Table process_table;
    process_table.push_header("");
    process_table.push_row({"0"});
    process_table.setWidth(3);
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
    while(1){
        if(isDone()) break;
        clrscr();
        label();
        p_process(process,to_update,process_table,curr_time);
        process_table.print();
        conts();
    }
    label();
    // calc tat, tat, wt here

    float ave_tat, ave_wt;
    table.print();
    cout << '\n';
    process_table.print();
    ave_tat = Format::truncate(ave_tat,2);
    ave_wt = Format::truncate(ave_wt,2);
    cout << "Average Turn Around Time: " << Format::toString(ave_tat) << '\n';
    cout << "Average Waiting Time: " << Format::toString(ave_wt) << '\n';

    return 0;
}
