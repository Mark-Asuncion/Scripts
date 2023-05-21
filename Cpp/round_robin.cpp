#include <iostream>
#include <string>
#include "ccs2203.h"
using namespace std;
using namespace Memory;
using namespace Color;
using Format::ss;

class TimelinePrinter : public Table {
    public:
    void print(){
        auto center = [](string val,int w) -> string{
            w -= val.size();
            w/=2;
            return string(w,' ') + val;
        };
        auto center_num = [](string val) -> string {
            return string(val.size()/2,' ') + val;
        };
        int w = getWidth()*rowSize()+rowSize()-1;
        int ww = getWidth();
        std::cout <<'+'<< std::setfill('-')<<std::setw(w)<< '-' << "+\n";
        std::cout<<'|';
        for(int i=0;i<rowSize();i++){
            std::cout<<std::setfill(' ')<<std::setw(ww)<<std::left<< center(Header()[i],getWidth())<<'|';
        }
        std::cout<<"\n+";
        for(int i=0;i<rowSize();i++){
            std::cout << std::setfill('-')<<std::setw(ww)<< '-' << '+';
        }
        std::cout <<"\n0";
        for(int i=0;i<1;i++){
            for(int j=0;j<rowSize();j++){
                std::cout <<std::setfill(' ')<<std::setw(ww+1)<<std::right<< center_num(Rows()[i][j]);
            }
            std::cout << '\n';
        }
        std::cout<<'+';
        for(int i=0;i<rowSize();i++){
            std::cout << std::setfill('-')<<std::setw(ww)<< '-' << '+';
        }
    }
};
void p_process(vector<Process> &process,deque<int>& process_queue,TimelinePrinter& process_table, int &curr_time, int &time_slice){
    string p_name="",p_ct="";
    int min = -1;
    for(int i=0;i<process.size();i++){
        if(process[i].isDone) continue;
        if(min == -1 || process[min].arrival_time > process[i].arrival_time) { min = i; }
        if(find(process_queue.begin(),process_queue.end(),i) != process_queue.end()) continue;
        process_queue.push_back(i);
    }
    if(process_queue.empty() && min != -1){
        curr_time = process[min].arrival_time;
        p_name = "idle";
        p_ct = to_string(curr_time);
    }
    else{
        int vt_time = curr_time + time_slice;
        int act_time = curr_time + process[process_queue.front()].remaining_time;
        if(vt_time > act_time){
            curr_time += time_slice;
            process[process_queue.front()].remaining_time -= time_slice;
            process_queue.push_back(process_queue.front());
        }
        else{
            curr_time += process[process_queue.front()].remaining_time;
            process[process_queue.front()].isDone = true;
            process[process_queue.front()].remaining_time = 0;
        }
        p_name = to_string(process[process_queue.front()].id);
        p_name = to_string(curr_time);

        process_queue.pop_front();
    }
    if (p_name != "idle") p_name = 'P' + p_name;
    process_table.push_col({p_name,p_ct});
}

int main(){
    act_name = "FL-M5: ACT4 - Round Robin";
    // input n-process
    string ms = "";
    int n_process = input<int>("Enter number of process: ",ms,false);
    vector<Process> process;
    Table table({"Processes","Arrival","Burst","CT","TAT","WT"},BOLD);
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
        process[i].remaining_time = burst_time;
        ss.str(string());
    }
    ms = "";
    int time_slice = input<int>("Enter Time Slice: ", ms, false);
    for (vector<Process>::iterator i=process.begin();i!=process.end();i++){
        table.push_row( {
                        'P' + to_string(i->id),
                        to_string(i->arrival_time), 
                        to_string(i->burst_time) 
        } );
    }
    label();
    TimelinePrinter process_table;
    deque<int> process_queue;
    int curr_time = 0;
    auto isDone = [&process]() -> bool {
        for(vector<Process>::iterator i=process.begin();i!=process.end();i++){ if(!i->isDone) return false; }
        return true;
    };
    auto str_center = [](string str, int width) -> string{
        if (width < 0) return str;
        width -= str.size();
        width /= 2;
        return string(width,' ') + str;
    };
    while(1){
        if(isDone()) break;
        clrscr();
        label();
        p_process(process,process_queue,process_table,curr_time,time_slice);
        int w = (process_table.getWidth() * process_table.rowSize()) + process_table.rowSize() - 1;
        cout << '+' << setfill('-') << setw(w) << '-' << "+\n";
        cout << '|' << setfill(' ') << setw(w) << std::left << str_center("Timeline",w) << "|\n";
        process_table.print();
        conts();
    }
    label();
    // calc tat, wt here
    float ave_tat = 0.0, ave_wt = 0.0;
    int colidx = table.rowSize();
    for(int i=0;i<process.size();i++){
        int tat = process[i].completion_time - process[i].arrival_time;
        int wt = tat - process[i].burst_time;
    
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
    cout << "Average Turn Around Time: " << Format::toString(Format::fround(ave_tat,2)) << '\n';
    cout << "Average Waiting Time: " << Format::toString(Format::fround(ave_wt,2)) << '\n';

    return 0;
}
